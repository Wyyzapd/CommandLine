#ifndef _Array__
#define _Array__

#include <cstdint>
#include <cstring>
#include <memory>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

#ifndef _CONSTEXPR20
#if _HAS_CXX20
#define _CONSTEXPR20 constexpr
#else
#define _CONSTEXPR20 inline
#endif
#endif

#pragma pack(push, 8)

template <class _Ty>
class Array {
public:
    using value_type      = _Ty;
    using size_type       = std::uint32_t;
    using difference_type = std::ptrdiff_t;
    using reference       = _Ty&;
    using const_reference = const _Ty&;
    using pointer         = _Ty*;
    using const_pointer   = const _Ty*;

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = _Ty;
        using difference_type   = std::ptrdiff_t;
        using pointer           = _Ty*;
        using reference         = _Ty&;

        _CONSTEXPR20 iterator() noexcept : _Ptr(nullptr) {}
        _CONSTEXPR20 explicit iterator(pointer _P) noexcept : _Ptr(_P) {}

        _CONSTEXPR20 reference operator*() const noexcept { return *_Ptr; }
        _CONSTEXPR20 pointer operator->() const noexcept { return _Ptr; }
        _CONSTEXPR20 reference operator[](difference_type _Off) const noexcept { return _Ptr[_Off]; }

        _CONSTEXPR20 iterator& operator++() noexcept { ++_Ptr; return *this; }
        _CONSTEXPR20 iterator operator++(int) noexcept { iterator _Tmp = *this; ++_Ptr; return _Tmp; }
        _CONSTEXPR20 iterator& operator--() noexcept { --_Ptr; return *this; }
        _CONSTEXPR20 iterator operator--(int) noexcept { iterator _Tmp = *this; --_Ptr; return _Tmp; }

        _CONSTEXPR20 iterator& operator+=(difference_type _Off) noexcept { _Ptr += _Off; return *this; }
        _CONSTEXPR20 iterator operator+(difference_type _Off) const noexcept { return iterator(_Ptr + _Off); }
        _CONSTEXPR20 friend iterator operator+(difference_type _Off, const iterator& _It) noexcept { return iterator(_It._Ptr + _Off); }
        _CONSTEXPR20 iterator& operator-=(difference_type _Off) noexcept { _Ptr -= _Off; return *this; }
        _CONSTEXPR20 iterator operator-(difference_type _Off) const noexcept { return iterator(_Ptr - _Off); }
        _CONSTEXPR20 difference_type operator-(const iterator& _Right) const noexcept { return _Ptr - _Right._Ptr; }

        _CONSTEXPR20 bool operator==(const iterator& _Right) const noexcept { return _Ptr == _Right._Ptr; }
        _CONSTEXPR20 bool operator!=(const iterator& _Right) const noexcept { return _Ptr != _Right._Ptr; }
        _CONSTEXPR20 bool operator<(const iterator& _Right) const noexcept { return _Ptr < _Right._Ptr; }
        _CONSTEXPR20 bool operator>(const iterator& _Right) const noexcept { return _Ptr > _Right._Ptr; }
        _CONSTEXPR20 bool operator<=(const iterator& _Right) const noexcept { return _Ptr <= _Right._Ptr; }
        _CONSTEXPR20 bool operator>=(const iterator& _Right) const noexcept { return _Ptr >= _Right._Ptr; }

        _CONSTEXPR20 pointer _Unwrapped() const noexcept { return _Ptr; }

    private:
        pointer _Ptr;
    };

    class const_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = _Ty;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const _Ty*;
        using reference         = const _Ty&;

        _CONSTEXPR20 const_iterator() noexcept : _Ptr(nullptr) {}
        _CONSTEXPR20 explicit const_iterator(const _Ty* _P) noexcept : _Ptr(_P) {}
        _CONSTEXPR20 const_iterator(const iterator& _It) noexcept : _Ptr(_It._Unwrapped()) {}

        _CONSTEXPR20 reference operator*() const noexcept { return *_Ptr; }
        _CONSTEXPR20 pointer operator->() const noexcept { return _Ptr; }
        _CONSTEXPR20 reference operator[](difference_type _Off) const noexcept { return _Ptr[_Off]; }

        _CONSTEXPR20 const_iterator& operator++() noexcept { ++_Ptr; return *this; }
        _CONSTEXPR20 const_iterator operator++(int) noexcept { const_iterator _Tmp = *this; ++_Ptr; return _Tmp; }
        _CONSTEXPR20 const_iterator& operator--() noexcept { --_Ptr; return *this; }
        _CONSTEXPR20 const_iterator operator--(int) noexcept { const_iterator _Tmp = *this; --_Ptr; return _Tmp; }

        _CONSTEXPR20 const_iterator& operator+=(difference_type _Off) noexcept { _Ptr += _Off; return *this; }
        _CONSTEXPR20 const_iterator operator+(difference_type _Off) const noexcept { return const_iterator(_Ptr + _Off); }
        _CONSTEXPR20 friend const_iterator operator+(difference_type _Off, const const_iterator& _It) noexcept { return const_iterator(_It._Ptr + _Off); }
        _CONSTEXPR20 const_iterator& operator-=(difference_type _Off) noexcept { _Ptr -= _Off; return *this; }
        _CONSTEXPR20 const_iterator operator-(difference_type _Off) const noexcept { return const_iterator(_Ptr - _Off); }
        _CONSTEXPR20 difference_type operator-(const const_iterator& _Right) const noexcept { return _Ptr - _Right._Ptr; }

        _CONSTEXPR20 bool operator==(const const_iterator& _Right) const noexcept { return _Ptr == _Right._Ptr; }
        _CONSTEXPR20 bool operator!=(const const_iterator& _Right) const noexcept { return _Ptr != _Right._Ptr; }
        _CONSTEXPR20 bool operator<(const const_iterator& _Right) const noexcept { return _Ptr < _Right._Ptr; }
        _CONSTEXPR20 bool operator>(const const_iterator& _Right) const noexcept { return _Ptr > _Right._Ptr; }
        _CONSTEXPR20 bool operator<=(const const_iterator& _Right) const noexcept { return _Ptr <= _Right._Ptr; }
        _CONSTEXPR20 bool operator>=(const const_iterator& _Right) const noexcept { return _Ptr >= _Right._Ptr; }

        _CONSTEXPR20 const _Ty* _Unwrapped() const noexcept { return _Ptr; }

    private:
        const _Ty* _Ptr;
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    template <class, class = void>
    struct _Is_iterator_impl : std::false_type {};
    template <class _Iter>
    struct _Is_iterator_impl<_Iter, std::void_t<typename std::iterator_traits<_Iter>::iterator_category>> : std::true_type {};
    template <class _Iter>
    static constexpr bool _Is_iterator_v = _Is_iterator_impl<_Iter>::value;

    static constexpr size_type _Minimum_allocation = 4;

    static size_type _Calculate_growth(size_type _NewSize, size_type _Capacity) noexcept {
        const size_type _Geometric = _Capacity + _Capacity / 2;
        if (_Geometric < _NewSize) return _NewSize;
        return _Geometric;
    }

    static size_type _Convert_size(size_type _NewSize) noexcept {
        return _NewSize < _Minimum_allocation ? _Minimum_allocation : _NewSize;
    }

    static void _Copy_trivial(pointer _Dst, const_pointer _Src, size_type _Count) noexcept {
#if _HAS_CXX20
        if (std::is_constant_evaluated()) {
            for (size_type i = 0; i < _Count; ++i) _Dst[i] = _Src[i];
            return;
        }
#endif
        std::memcpy(_Dst, _Src, _Count * sizeof(_Ty));
    }

    static void _Zero_trivial(pointer _Dst, size_type _Count) noexcept {
#if _HAS_CXX20
        if (std::is_constant_evaluated()) {
            for (size_type i = 0; i < _Count; ++i) ::new (static_cast<void*>(_Dst + i)) _Ty();
            return;
        }
#endif
        std::memset(_Dst, 0, _Count * sizeof(_Ty));
    }

    static void _Fill_trivial(pointer _Dst, size_type _Count, const _Ty& _Val) noexcept {
#if _HAS_CXX20
        if (std::is_constant_evaluated()) {
            for (size_type i = 0; i < _Count; ++i) _Dst[i] = _Val;
            return;
        }
#endif
        for (size_type i = 0; i < _Count; ++i) std::memcpy(_Dst + i, &_Val, sizeof(_Ty));
    }

    _CONSTEXPR20 void _Reallocate(size_type _NewCap) {
        pointer _NewBlock = static_cast<pointer>(::operator new(_NewCap * sizeof(_Ty)));
        size_type _OldSize = _MySize;
        if (_OldSize > _NewCap) _OldSize = _NewCap;
        if constexpr (std::is_trivially_copyable_v<_Ty>) {
            _Copy_trivial(_NewBlock, _Mypair, _OldSize);
        } else {
            size_type _Moved = 0;
            try {
                for (; _Moved < _OldSize; ++_Moved)
                    ::new (static_cast<void*>(_NewBlock + _Moved)) _Ty(std::move(_Mypair[_Moved]));
            } catch (...) {
                for (size_type i = 0; i < _Moved; ++i) _NewBlock[i].~_Ty();
                ::operator delete(_NewBlock);
                throw;
            }
            for (size_type i = 0; i < _OldSize; ++i) _Mypair[i].~_Ty();
        }
        ::operator delete(_Mypair);
        _Mypair = _NewBlock;
        _MyCap = _NewCap;
    }

    _CONSTEXPR20 void _Destroy_range(pointer _First, pointer _Last) noexcept {
        if constexpr (!std::is_trivially_destructible_v<_Ty>) {
            for (; _First != _Last; ++_First) _First->~_Ty();
        }
    }

public:
    _CONSTEXPR20 Array() noexcept : _Mypair(nullptr), _MySize(0), _MyCap(0) {}

    _CONSTEXPR20 explicit Array(size_type _Count) : _Mypair(nullptr), _MySize(0), _MyCap(0) {
        if (_Count > 0) {
            size_type _Cap = _Convert_size(_Count);
            _Mypair = static_cast<pointer>(::operator new(_Cap * sizeof(_Ty)));
            _MyCap = _Cap;
            _MySize = _Count;
            if constexpr (std::is_trivial_v<_Ty>)
                _Zero_trivial(_Mypair, _Count);
            else
                for (size_type i = 0; i < _Count; ++i) ::new (static_cast<void*>(_Mypair + i)) _Ty();
        }
    }

    _CONSTEXPR20 Array(size_type _Count, const _Ty& _Val) : _Mypair(nullptr), _MySize(0), _MyCap(0) {
        if (_Count > 0) {
            size_type _Cap = _Convert_size(_Count);
            _Mypair = static_cast<pointer>(::operator new(_Cap * sizeof(_Ty)));
            _MyCap = _Cap;
            _MySize = _Count;
            if constexpr (std::is_trivially_copyable_v<_Ty>)
                _Fill_trivial(_Mypair, _Count, _Val);
            else
                for (size_type i = 0; i < _Count; ++i) ::new (static_cast<void*>(_Mypair + i)) _Ty(_Val);
        }
    }

    template <class _Iter, std::enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20 Array(_Iter _First, _Iter _Last) : _Mypair(nullptr), _MySize(0), _MyCap(0) {
        for (; _First != _Last; ++_First) push_back(*_First);
    }

    _CONSTEXPR20 Array(std::initializer_list<_Ty> _Ilist) : _Mypair(nullptr), _MySize(0), _MyCap(0) {
        size_type _Count = static_cast<size_type>(_Ilist.size());
        if (_Count > 0) {
            size_type _Cap = _Convert_size(_Count);
            _Mypair = static_cast<pointer>(::operator new(_Cap * sizeof(_Ty)));
            _MyCap = _Cap;
            _MySize = _Count;
            if constexpr (std::is_trivially_copyable_v<_Ty>)
                _Copy_trivial(_Mypair, const_cast<const_pointer>(_Ilist.begin()), _Count);
            else
                for (size_type i = 0; i < _Count; ++i) ::new (static_cast<void*>(_Mypair + i)) _Ty(_Ilist.begin()[i]);
        }
    }

    _CONSTEXPR20 Array(const Array& _Right) : _Mypair(nullptr), _MySize(0), _MyCap(0) {
        size_type _Count = _Right._MySize;
        if (_Count > 0) {
            size_type _Cap = _Convert_size(_Count);
            _Mypair = static_cast<pointer>(::operator new(_Cap * sizeof(_Ty)));
            _MyCap = _Cap;
            _MySize = _Count;
            if constexpr (std::is_trivially_copyable_v<_Ty>)
                _Copy_trivial(_Mypair, _Right._Mypair, _Count);
            else
                for (size_type i = 0; i < _Count; ++i) ::new (static_cast<void*>(_Mypair + i)) _Ty(_Right._Mypair[i]);
        }
    }

    _CONSTEXPR20 Array(Array&& _Right) noexcept : _Mypair(_Right._Mypair), _MySize(_Right._MySize), _MyCap(_Right._MyCap) {
        _Right._Mypair = nullptr;
        _Right._MySize = 0;
        _Right._MyCap = 0;
    }

    _CONSTEXPR20 ~Array() noexcept {
        _Destroy_range(_Mypair, _Mypair + _MySize);
        ::operator delete(_Mypair);
    }

    _CONSTEXPR20 Array& operator=(const Array& _Right) {
        if (this != std::addressof(_Right)) assign(_Right.begin(), _Right.end());
        return *this;
    }

    _CONSTEXPR20 Array& operator=(Array&& _Right) noexcept {
        if (this != std::addressof(_Right)) {
            _Destroy_range(_Mypair, _Mypair + _MySize);
            ::operator delete(_Mypair);
            _Mypair = _Right._Mypair;
            _MySize = _Right._MySize;
            _MyCap = _Right._MyCap;
            _Right._Mypair = nullptr;
            _Right._MySize = 0;
            _Right._MyCap = 0;
        }
        return *this;
    }

    _CONSTEXPR20 Array& operator=(std::initializer_list<_Ty> _Ilist) {
        assign(_Ilist);
        return *this;
    }

    _CONSTEXPR20 void assign(size_type _Count, const _Ty& _Val) {
        clear();
        if (_Count > _MyCap) {
            ::operator delete(_Mypair);
            size_type _Cap = _Convert_size(_Count);
            _Mypair = static_cast<pointer>(::operator new(_Cap * sizeof(_Ty)));
            _MyCap = _Cap;
        }
        _MySize = _Count;
        for (size_type i = 0; i < _Count; ++i) ::new (static_cast<void*>(_Mypair + i)) _Ty(_Val);
    }

    template <class _Iter, std::enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20 void assign(_Iter _First, _Iter _Last) {
        clear();
        for (; _First != _Last; ++_First) push_back(*_First);
    }

    _CONSTEXPR20 void assign(std::initializer_list<_Ty> _Ilist) {
        assign(_Ilist.begin(), _Ilist.end());
    }

    _CONSTEXPR20 reference at(size_type _Pos) {
        if (_Pos >= _MySize) _Xran();
        return _Mypair[_Pos];
    }

    _CONSTEXPR20 const_reference at(size_type _Pos) const {
        if (_Pos >= _MySize) _Xran();
        return _Mypair[_Pos];
    }

    _CONSTEXPR20 reference operator[](size_type _Pos) noexcept { return _Mypair[_Pos]; }
    _CONSTEXPR20 const_reference operator[](size_type _Pos) const noexcept { return _Mypair[_Pos]; }

    _CONSTEXPR20 reference front() noexcept { return _Mypair[0]; }
    _CONSTEXPR20 const_reference front() const noexcept { return _Mypair[0]; }

    _CONSTEXPR20 reference back() noexcept { return _Mypair[_MySize - 1]; }
    _CONSTEXPR20 const_reference back() const noexcept { return _Mypair[_MySize - 1]; }

    _CONSTEXPR20 pointer data() noexcept { return _Mypair; }
    _CONSTEXPR20 const_pointer data() const noexcept { return _Mypair; }

    _CONSTEXPR20 iterator begin() noexcept { return iterator(_Mypair); }
    _CONSTEXPR20 const_iterator begin() const noexcept { return const_iterator(_Mypair); }
    _CONSTEXPR20 const_iterator cbegin() const noexcept { return const_iterator(_Mypair); }

    _CONSTEXPR20 iterator end() noexcept { return iterator(_Mypair + _MySize); }
    _CONSTEXPR20 const_iterator end() const noexcept { return const_iterator(_Mypair + _MySize); }
    _CONSTEXPR20 const_iterator cend() const noexcept { return const_iterator(_Mypair + _MySize); }

    _CONSTEXPR20 reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    _CONSTEXPR20 const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    _CONSTEXPR20 const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    _CONSTEXPR20 reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    _CONSTEXPR20 const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    _CONSTEXPR20 const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    _CONSTEXPR20 _NODISCARD bool empty() const noexcept { return _MySize == 0; }
    _CONSTEXPR20 size_type size() const noexcept { return _MySize; }
    _CONSTEXPR20 size_type max_size() const noexcept { return static_cast<size_type>(-1) / sizeof(_Ty); }
    _CONSTEXPR20 size_type capacity() const noexcept { return _MyCap; }

    _CONSTEXPR20 void reserve(size_type _NewCap) {
        if (_NewCap > _MyCap) _Reallocate(_NewCap);
    }

    _CONSTEXPR20 void shrink_to_fit() {
        if (_MySize < _MyCap) {
            if (_MySize == 0) {
                clear();
            } else {
                pointer _NewBlock = static_cast<pointer>(::operator new(_MySize * sizeof(_Ty)));
                if constexpr (std::is_trivially_copyable_v<_Ty>) {
                    _Copy_trivial(_NewBlock, _Mypair, _MySize);
                } else {
                    size_type _Moved = 0;
                    try {
                        for (; _Moved < _MySize; ++_Moved)
                            ::new (static_cast<void*>(_NewBlock + _Moved)) _Ty(std::move(_Mypair[_Moved]));
                    } catch (...) {
                        for (size_type i = 0; i < _Moved; ++i) _NewBlock[i].~_Ty();
                        ::operator delete(_NewBlock);
                        throw;
                    }
                }
                _Destroy_range(_Mypair, _Mypair + _MySize);
                ::operator delete(_Mypair);
                _Mypair = _NewBlock;
                _MyCap = _MySize;
            }
        }
    }

    _CONSTEXPR20 void clear() noexcept {
        _Destroy_range(_Mypair, _Mypair + _MySize);
        _MySize = 0;
    }

    _CONSTEXPR20 iterator insert(const_iterator _Where, const _Ty& _Val) { return emplace(_Where, _Val); }
    _CONSTEXPR20 iterator insert(const_iterator _Where, _Ty&& _Val) { return emplace(_Where, std::move(_Val)); }

    _CONSTEXPR20 iterator insert(const_iterator _Where, size_type _Count, const _Ty& _Val) {
        difference_type _Off = _Where._Unwrapped() - _Mypair;
        size_type _OldSize = _MySize;
        if (_Count == 0) return iterator(_Mypair + _Off);
        if (_MySize + _Count > _MyCap) {
            size_type _NewCap = _Convert_size(_Calculate_growth(_OldSize + _Count, _MyCap));
            pointer _NewBlock = static_cast<pointer>(::operator new(_NewCap * sizeof(_Ty)));
            if constexpr (std::is_trivially_copyable_v<_Ty>) {
                _Copy_trivial(_NewBlock, _Mypair, static_cast<size_type>(_Off));
                _Copy_trivial(_NewBlock + _Off + _Count, _Mypair + _Off, _OldSize - static_cast<size_type>(_Off));
            } else {
                for (size_type i = 0; i < static_cast<size_type>(_Off); ++i)
                    ::new (static_cast<void*>(_NewBlock + i)) _Ty(std::move(_Mypair[i]));
                for (size_type i = static_cast<size_type>(_Off); i < _OldSize; ++i)
                    ::new (static_cast<void*>(_NewBlock + i + _Count)) _Ty(std::move(_Mypair[i]));
            }
            for (size_type i = 0; i < _Count; ++i)
                ::new (static_cast<void*>(_NewBlock + _Off + i)) _Ty(_Val);
            _Destroy_range(_Mypair, _Mypair + _OldSize);
            ::operator delete(_Mypair);
            _Mypair = _NewBlock;
            _MyCap = _NewCap;
            _MySize = _OldSize + _Count;
        } else {
            _MySize = _OldSize + _Count;
            if (static_cast<size_type>(_Off) < _OldSize) {
                for (size_type i = _OldSize; i > static_cast<size_type>(_Off); --i)
                    _Mypair[i + _Count - 1] = std::move(_Mypair[i - 1]);
            }
            for (size_type i = 0; i < _Count; ++i) _Mypair[_Off + i] = _Val;
        }
        return iterator(_Mypair + _Off);
    }

    template <class _Iter, std::enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20 iterator insert(const_iterator _Where, _Iter _First, _Iter _Last) {
        difference_type _Off = _Where._Unwrapped() - _Mypair;
        size_type _Count = static_cast<size_type>(std::distance(_First, _Last));
        if (_Count == 0) return iterator(_Mypair + _Off);
        size_type _OldSize = _MySize;
        if (_MySize + _Count > _MyCap) {
            size_type _NewCap = _Convert_size(_Calculate_growth(_OldSize + _Count, _MyCap));
            pointer _NewBlock = static_cast<pointer>(::operator new(_NewCap * sizeof(_Ty)));
            if constexpr (std::is_trivially_copyable_v<_Ty>) {
                _Copy_trivial(_NewBlock, _Mypair, static_cast<size_type>(_Off));
                _Copy_trivial(_NewBlock + _Off + _Count, _Mypair + _Off, _OldSize - static_cast<size_type>(_Off));
            } else {
                for (size_type i = 0; i < static_cast<size_type>(_Off); ++i)
                    ::new (static_cast<void*>(_NewBlock + i)) _Ty(std::move(_Mypair[i]));
                for (size_type i = static_cast<size_type>(_Off); i < _OldSize; ++i)
                    ::new (static_cast<void*>(_NewBlock + i + _Count)) _Ty(std::move(_Mypair[i]));
            }
            pointer _Tmp = _NewBlock + _Off;
            for (auto _It = _First; _It != _Last; ++_It, ++_Tmp) ::new (static_cast<void*>(_Tmp)) _Ty(*_It);
            _Destroy_range(_Mypair, _Mypair + _OldSize);
            ::operator delete(_Mypair);
            _Mypair = _NewBlock;
            _MyCap = _NewCap;
            _MySize = _OldSize + _Count;
        } else {
            _MySize = _OldSize + _Count;
            if (static_cast<size_type>(_Off) < _OldSize) {
                for (size_type i = _OldSize; i > static_cast<size_type>(_Off); --i)
                    _Mypair[i + _Count - 1] = std::move(_Mypair[i - 1]);
            }
            pointer _Tmp = _Mypair + _Off;
            for (auto _It = _First; _It != _Last; ++_It, ++_Tmp) *_Tmp = *_It;
        }
        return iterator(_Mypair + _Off);
    }

    _CONSTEXPR20 iterator insert(const_iterator _Where, std::initializer_list<_Ty> _Ilist) {
        return insert(_Where, _Ilist.begin(), _Ilist.end());
    }

    template <class... _Valty>
    _CONSTEXPR20 iterator emplace(const_iterator _Where, _Valty&&... _Val) {
        difference_type _Off = _Where._Unwrapped() - _Mypair;
        size_type _OldSize = _MySize;
        if (_MySize >= _MyCap) {
            size_type _NewCap = _Calculate_growth(_OldSize + 1, _MyCap);
            if (_NewCap < _OldSize + 1) _NewCap = _OldSize + 1;
            _NewCap = _Convert_size(_NewCap);
            pointer _NewBlock = static_cast<pointer>(::operator new(_NewCap * sizeof(_Ty)));
            if constexpr (std::is_trivially_copyable_v<_Ty>) {
                _Copy_trivial(_NewBlock, _Mypair, static_cast<size_type>(_Off));
                _Copy_trivial(_NewBlock + _Off + 1, _Mypair + _Off, _OldSize - static_cast<size_type>(_Off));
            } else {
                for (size_type i = 0; i < static_cast<size_type>(_Off); ++i)
                    ::new (static_cast<void*>(_NewBlock + i)) _Ty(std::move(_Mypair[i]));
                for (size_type i = static_cast<size_type>(_Off); i < _OldSize; ++i)
                    ::new (static_cast<void*>(_NewBlock + i + 1)) _Ty(std::move(_Mypair[i]));
            }
            ::new (static_cast<void*>(_NewBlock + _Off)) _Ty(std::forward<_Valty>(_Val)...);
            _Destroy_range(_Mypair, _Mypair + _OldSize);
            ::operator delete(_Mypair);
            _Mypair = _NewBlock;
            _MyCap = _NewCap;
            _MySize = _OldSize + 1;
        } else {
            if (static_cast<size_type>(_Off) < _OldSize) {
                ::new (static_cast<void*>(_Mypair + _OldSize)) _Ty(std::move(_Mypair[_OldSize - 1]));
                for (size_type i = _OldSize - 1; i > static_cast<size_type>(_Off); --i)
                    _Mypair[i] = std::move(_Mypair[i - 1]);
                _Mypair[_Off].~_Ty();
            } else {
                _Mypair[_Off].~_Ty();
            }
            ::new (static_cast<void*>(_Mypair + _Off)) _Ty(std::forward<_Valty>(_Val)...);
            ++_MySize;
        }
        return iterator(_Mypair + _Off);
    }

    template <class... _Valty>
    _CONSTEXPR20 reference emplace_back(_Valty&&... _Val) {
        if (_MySize >= _MyCap) {
            size_type _NewCap = _Calculate_growth(_MySize + 1, _MyCap);
            if (_NewCap < _MySize + 1) _NewCap = _MySize + 1;
            _Reallocate(_Convert_size(_NewCap));
        }
        ::new (static_cast<void*>(_Mypair + _MySize)) _Ty(std::forward<_Valty>(_Val)...);
        ++_MySize;
        return _Mypair[_MySize - 1];
    }

    _CONSTEXPR20 void push_back(const _Ty& _Val) { emplace_back(_Val); }
    _CONSTEXPR20 void push_back(_Ty&& _Val) { emplace_back(std::move(_Val)); }

    _CONSTEXPR20 void pop_back() noexcept { _Mypair[--_MySize].~_Ty(); }

    _CONSTEXPR20 iterator erase(const_iterator _Where) noexcept {
        difference_type _Off = _Where._Unwrapped() - _Mypair;
        if (static_cast<size_type>(_Off) < _MySize - 1) {
            for (size_type i = static_cast<size_type>(_Off); i < _MySize - 1; ++i)
                _Mypair[i] = std::move(_Mypair[i + 1]);
        }
        _Mypair[--_MySize].~_Ty();
        return iterator(_Mypair + _Off);
    }

    _CONSTEXPR20 iterator erase(const_iterator _First, const_iterator _Last) noexcept {
        difference_type _Off = _First._Unwrapped() - _Mypair;
        difference_type _Count = _Last._Unwrapped() - _First._Unwrapped();
        if (_Count > 0) {
            for (size_type i = static_cast<size_type>(_Off); i + static_cast<size_type>(_Count) < _MySize; ++i)
                _Mypair[i] = std::move(_Mypair[i + _Count]);
            for (difference_type i = 0; i < _Count; ++i)
                _Mypair[--_MySize].~_Ty();
        }
        return iterator(_Mypair + _Off);
    }

    _CONSTEXPR20 void resize(size_type _NewSize) {
        if (_NewSize < _MySize) {
            _Destroy_range(_Mypair + _NewSize, _Mypair + _MySize);
            _MySize = _NewSize;
        } else if (_NewSize > _MySize) {
            resize(_NewSize, _Ty());
        }
    }

    _CONSTEXPR20 void resize(size_type _NewSize, const _Ty& _Val) {
        if (_NewSize < _MySize) {
            _Destroy_range(_Mypair + _NewSize, _Mypair + _MySize);
            _MySize = _NewSize;
        } else if (_NewSize > _MySize) {
            if (_NewSize > _MyCap) reserve(_NewSize);
            for (size_type i = _MySize; i < _NewSize; ++i)
                ::new (static_cast<void*>(_Mypair + i)) _Ty(_Val);
            _MySize = _NewSize;
        }
    }

    _CONSTEXPR20 void swap(Array& _Right) noexcept {
        std::swap(_Mypair, _Right._Mypair);
        std::swap(_MySize, _Right._MySize);
        std::swap(_MyCap, _Right._MyCap);
    }

private:
    [[noreturn]] static void _Xran() { throw std::out_of_range("invalid Array subscript"); }

protected:
    pointer   _Mypair;
    size_type _MySize;
    size_type _MyCap;
};

template <class _Ty>
_CONSTEXPR20 _NODISCARD bool operator==(const Array<_Ty>& _Left, const Array<_Ty>& _Right) {
    if (_Left.size() != _Right.size()) return false;
    for (typename Array<_Ty>::size_type i = 0; i < _Left.size(); ++i)
        if (_Left[i] != _Right[i]) return false;
    return true;
}

template <class _Ty>
_CONSTEXPR20 _NODISCARD bool operator!=(const Array<_Ty>& _Left, const Array<_Ty>& _Right) { return !(_Left == _Right); }

template <class _Ty>
_CONSTEXPR20 _NODISCARD bool operator<(const Array<_Ty>& _Left, const Array<_Ty>& _Right) {
    return std::lexicographical_compare(_Left.begin(), _Left.end(), _Right.begin(), _Right.end());
}

template <class _Ty>
_CONSTEXPR20 _NODISCARD bool operator>(const Array<_Ty>& _Left, const Array<_Ty>& _Right) { return _Right < _Left; }

template <class _Ty>
_CONSTEXPR20 _NODISCARD bool operator<=(const Array<_Ty>& _Left, const Array<_Ty>& _Right) { return !(_Right < _Left); }

template <class _Ty>
_CONSTEXPR20 _NODISCARD bool operator>=(const Array<_Ty>& _Left, const Array<_Ty>& _Right) { return !(_Left < _Right); }

template <class _Ty>
_CONSTEXPR20 void swap(Array<_Ty>& _Left, Array<_Ty>& _Right) noexcept { _Left.swap(_Right); }

template <class _Ty, class _Pr>
_CONSTEXPR20 typename Array<_Ty>::size_type erase_if(Array<_Ty>& _Cont, _Pr _Pred) {
    auto _It = std::remove_if(_Cont.begin(), _Cont.end(), _Pred);
    auto _Count = static_cast<typename Array<_Ty>::size_type>(_Cont.end() - _It);
    _Cont.erase(_It, _Cont.end());
    return _Count;
}

template <class _Ty, class _Uty>
_CONSTEXPR20 typename Array<_Ty>::size_type erase(Array<_Ty>& _Cont, const _Uty& _Val) {
    auto _It = std::remove(_Cont.begin(), _Cont.end(), _Val);
    auto _Count = static_cast<typename Array<_Ty>::size_type>(_Cont.end() - _It);
    _Cont.erase(_It, _Cont.end());
    return _Count;
}

template <class _Ty>
_CONSTEXPR20 typename Array<_Ty>::size_type erase(Array<_Ty>& _Cont, const _Ty& _Val) {
    auto _It = std::remove(_Cont.begin(), _Cont.end(), _Val);
    auto _Count = static_cast<typename Array<_Ty>::size_type>(_Cont.end() - _It);
    _Cont.erase(_It, _Cont.end());
    return _Count;
}

#pragma pack(pop)
#endif
