#ifndef _Stream_
#define _Stream_

#include <Windows.h>
#include <string>
#include <Array.hpp>
#include <AString.hpp>
#include <cstdint>


class IOStream {
public:
    enum class Io : DWORD {
        R = 0x80000000L,//只读
        W = 0x40000000L,//写
        RW = R | W      //读写
    };

    enum class OpenIo : DWORD {
        Open = 0x00000003, //打开 (文件需已存在)
        Create = 0x00000002  //创建 (文件存在则错误)
    };

    enum class IoPtr : DWORD {
        Curr = 1, //当前位置
        Begin = 0, //起始位置
        End = 2  //结束位置
    };
private:
    enum class ShareIo : DWORD {
        N = 0,          //私有 文件(独立占用不共享)
        R = 0x00000001, //共享 读
        W = 0x00000002, //共享 写
        RW = R | W      //共享 读写
    };
public:
    IOStream() noexcept :
        _MyHandle(0),
        _MyAccess(0),
        _MyShareAccess(0),
        _MyOpenMode(0),
        _MyOpen(false) {
    }

    IOStream(const AString& lpFileName) noexcept {
        TryOpen(lpFileName);
    }

    IOStream(const WString& lpFileName) noexcept {
        TryOpen(lpFileName);
    }

    IOStream( const AString& lpFileName , Io Access , OpenIo OpenMode ) noexcept {
        TryOpen( lpFileName, Access , OpenMode );
    }

    IOStream( const WString& lpFileName , Io Access , OpenIo OpenMode ) noexcept {
        TryOpen( lpFileName , Access , OpenMode );
    }

    ~IOStream() noexcept {

        if (_MyHandle) {
            CloseHandle(_MyHandle);
        }

        _MyHandle = 0;
        _MyAccess = 0;
        _MyShareAccess = 0;
        _MyOpenMode = 0;
        _MyOpen = false;
    }

    virtual bool TryOpen(const AString& lpFileName) noexcept {
        return TryCreateFile(lpFileName, Io::R, ShareIo::R, OpenIo::Open);
    }

    virtual bool TryOpen(const WString& lpFileName) noexcept {
        return TryCreateFile(lpFileName, Io::R, ShareIo::R, OpenIo::Open);
    }

    virtual bool TryOpen(const AString& lpFileName, Io Access, OpenIo OpenMode) noexcept {
        ShareIo ShareMode = ShareIo::R;

        if (Access == Io::RW || Access == Io::W) {
            ShareMode = ShareIo::N;
        }

        return TryCreateFile(lpFileName, Access, ShareMode, OpenMode);
    }

    virtual bool TryOpen(const WString& lpFileName, Io Access, OpenIo OpenMode) noexcept {
        ShareIo ShareMode = ShareIo::R;

        if (Access == Io::RW || Access == Io::W) {
            ShareMode = ShareIo::N;
        }

        return TryCreateFile(lpFileName, Access, ShareMode, OpenMode);
    }

    virtual bool TryCreate(const AString& lpFileName) noexcept {
        return TryCreateFile(lpFileName, Io::RW, ShareIo::N, OpenIo::Create);
    }

    virtual bool TryCreate(const WString& lpFileName) noexcept {
        return TryCreateFile(lpFileName, Io::RW, ShareIo::N, OpenIo::Create);
    }

    virtual bool Mov(const std::size_t Offset, IoPtr IoOffset = IoPtr::Begin) noexcept {
        LARGE_INTEGER ToMove = { 0 };

        if (!_MyOpen) {
            return false;
        }

        ToMove.QuadPart = Offset;
        return SetFilePointerEx(_MyHandle, ToMove, 0, static_cast<DWORD>(IoOffset));
    }

    virtual bool ReadBuffer(Array<std::uint8_t>& Buffer, std::uint32_t BuffSize, std::size_t Offset = -1) {
        if (!_MyOpen) {
            return false;
        }

        Buffer.resize(BuffSize);
        return ReadBuffer(Buffer.data(), BuffSize, Offset);
    }

    virtual bool ReadBuffer(Array<std::uint8_t>& Buffer, std::size_t Offset = -1) {
        LARGE_INTEGER FileSize = { 0 };
        if (!_MyOpen) {
            return false;
        }

        FileSize.LowPart = GetFileSize(_MyHandle, 0);
        if (!FileSize.LowPart) {
            return false;
        }

        Buffer.resize(FileSize.LowPart);
        return ReadBuffer(Buffer.data(), FileSize.LowPart, Offset);
    }

    virtual bool ReadBuffer(void* Buffer, std::uint32_t BuffSize, std::size_t Offset = -1) noexcept {

        if (!_MyOpen) {
            return false;
        }

        if (Offset != -1) {
            if (!Mov(Offset)) {
                return false;
            }
        }

        auto ReadSize = 0ul;
        auto IsRead = ReadFile(_MyHandle, Buffer, BuffSize, &ReadSize, 0);
        return IsRead && ReadSize == BuffSize;
    }

    virtual bool WriteBuffer(Array<std::uint8_t>& Buffer, std::size_t Offset = -1) {
        if (!_MyOpen || Buffer.empty()) {
            return false;
        }

        return WriteBuffer(Buffer.data(), Buffer.size(), Offset);
    }

    virtual bool WriteBuffer(void* Buffer, std::uint32_t BuffSize, std::size_t Offset = -1) noexcept {

        if (!_MyOpen) {
            return false;
        }

        if (_MyAccess != static_cast<DWORD>(Io::RW) && _MyAccess != static_cast<DWORD>(Io::W)) {
            return false;
        }

        if (Offset != -1) {
            if (!Mov(Offset)) {
                return false;
            }
        }

        auto WriteSize = 0ul;
        auto IsWrite = WriteFile(_MyHandle, Buffer, BuffSize, &WriteSize, 0);
        return IsWrite && WriteSize == BuffSize;
    }

    virtual bool Is_Open() const noexcept {
        return _MyOpen;
    }

    virtual std::uint64_t Size() const noexcept {
        LARGE_INTEGER FileSize = { 0 };

        if (!_MyOpen) {
            return 0;
        }

        auto IsSize = GetFileSizeEx(_MyHandle, &FileSize);
        return IsSize ? static_cast<std::uint64_t>(FileSize.QuadPart) : 0;
    }

    virtual bool Size(std::uint64_t* OutSize) const noexcept {
        if (!OutSize || !_MyOpen) {
            return false;
        }

        LARGE_INTEGER FileSize = { 0 };

        auto IsSize = GetFileSizeEx(_MyHandle, &FileSize);
        if (IsSize) {
            *OutSize = static_cast<std::uint64_t>(FileSize.QuadPart);
        }

        return IsSize;
    }

    virtual WString& GetFileName() noexcept {
        return _MyName;
    }

    virtual bool Close() noexcept {
        if (!_MyOpen) {
            return false;
        }

        _MyAccess = 0;
        _MyShareAccess = 0;
        _MyOpenMode = 0;

        if (_MyHandle) {
            CloseHandle(_MyHandle);
        }

        _MyHandle = 0;
        _MyOpen = false;

        _MyName.Clear();

        return true;
    }

    virtual HANDLE GetHandle()const noexcept {
        return _MyHandle;
    }

protected:
    /// <summary>
    /// 文件句柄
    /// </summary>
    HANDLE _MyHandle;
    /// <summary>
    /// 文件名称
    /// </summary>
    WString _MyName;
    /// <summary>
    /// 打开权限
    /// </summary>
    DWORD   _MyAccess;
    /// <summary>
    /// 共享模式
    /// </summary>
    DWORD   _MyShareAccess;
    /// <summary>
    /// 打开模式
    /// </summary>
    DWORD   _MyOpenMode;
    /// <summary>
    /// 是否已经打开
    /// </summary>
    bool    _MyOpen;
private:

    virtual bool TryCreateFile(const WString& lpFileName, Io Access, ShareIo ShareMode, OpenIo OpenMode) noexcept {
        if (lpFileName.IsEmpty()) return false;
        if (_MyOpen) return true;

        DWORD _Acc = static_cast<DWORD>(Access);
        DWORD _Shr = static_cast<DWORD>(ShareMode);
        DWORD _Mod = static_cast<DWORD>(OpenMode);
        HANDLE _H = CreateFileW(lpFileName.c_str(), _Acc, _Shr, 0, _Mod, FILE_ATTRIBUTE_NORMAL, 0);
        if (_H == 0 || _H == INVALID_HANDLE_VALUE) return false;

        _MyName = lpFileName;
        _MyAccess = _Acc;
        _MyShareAccess = _Shr;
        _MyOpenMode = _Mod;
        _MyHandle = _H;
        _MyOpen = true;
        return true;
    }
    
    virtual bool TryCreateFile(const AString& lpFileName, Io Access, ShareIo ShareMode, OpenIo OpenMode) noexcept {
        std::uint32_t length = 0;

        if (lpFileName.IsEmpty()) {
            return false;
        }

        length = MultiByteToWideChar(CP_UTF8, 0, lpFileName.c_str(), static_cast<int>(lpFileName.length()), nullptr, 0);
        if (!length) {
            return false;
        }

        WString wide(length, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, lpFileName.c_str(), static_cast<int>(lpFileName.length()), &wide[0], length);


        return TryCreateFile(wide, Access, ShareMode, OpenMode);
    }
    
};


#endif