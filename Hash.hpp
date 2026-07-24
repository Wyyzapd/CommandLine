#ifndef _FNVHash_
#define _FNVHash_

#include <cstdint>
#include <type_traits>
#include <cstddef>
#include <cstring>

#ifndef _FORCEINLINE
#ifdef _MSC_VER
#define _FORCEINLINE __forceinline
#else
#define _FORCEINLINE inline __attribute__((always_inline))
#endif
#endif

class Hash {
private:
	static _FORCEINLINE constexpr char Tolower(char str) {
		auto byte = str;

		if (byte >= 'A' && byte <= 'Z') {
			byte += 32;
		}
		return byte;
	}

	static _FORCEINLINE constexpr wchar_t Tolower(wchar_t str) {
		auto byte = str;

		if ((byte >= L'A' && byte <= L'Z') || (byte >= 0xC0 && byte <= 0xD6)) {
			return byte | 32;
		}

		return byte;
	}

public:
	static _FORCEINLINE constexpr std::uint32_t fnv1_hash_constexpr(const char* str, std::uint32_t hash = 2166136261u) {
		return (*str == '\0') ? hash : fnv1_hash_constexpr(str + 1, (hash ^ *str) * 16777619u);
	}

	static _FORCEINLINE constexpr std::uint64_t fnv1_hash_64_constexpr(const char* str, std::uint64_t hash = 14695981039346656037ULL) {
		return (*str == '\0') ? hash : fnv1_hash_64_constexpr(str + 1, (hash ^ *str) * 1099511628211ULL);
	}

	static _FORCEINLINE constexpr std::uint32_t fnv1a_hash_constexpr(const char* str, std::uint32_t hash = 2166136261u) {
		return (*str == '\0') ? hash : fnv1a_hash_constexpr(str + 1, (hash ^ Tolower(*str)) * 16777619u);
	}

	static _FORCEINLINE constexpr std::uint64_t fnv1a_hash_64_constexpr(const char* str, std::uint64_t hash = 14695981039346656037ULL) {
		return (*str == '\0') ? hash : fnv1a_hash_64_constexpr(str + 1, (hash ^ Tolower(*str)) * 1099511628211ULL);
	}
	
	static _FORCEINLINE constexpr std::uint32_t fnv1_hash_constexpr(const wchar_t* str, std::uint32_t hash = 2166136261u) {
		return (*str == '\0') ? hash : fnv1_hash_constexpr(str + 1, (hash ^ *str) * 16777619u);
	}

	static _FORCEINLINE constexpr std::uint64_t fnv1_hash_64_constexpr(const wchar_t* str, std::uint64_t hash = 14695981039346656037ULL) {
		return (*str == '\0') ? hash : fnv1_hash_64_constexpr(str + 1, (hash ^ *str) * 1099511628211ULL);
	}

	static _FORCEINLINE constexpr std::uint32_t fnv1a_hash_constexpr(const wchar_t* str, std::uint32_t hash = 2166136261u) {
		return (*str == '\0') ? hash : fnv1a_hash_constexpr(str + 1, (hash ^ Tolower(*str)) * 16777619u);
	}

	static _FORCEINLINE constexpr std::uint64_t fnv1a_hash_64_constexpr(const wchar_t* str, std::uint64_t hash = 14695981039346656037ULL) {
		return (*str == '\0') ? hash : fnv1a_hash_64_constexpr(str + 1, (hash ^ Tolower(*str)) * 1099511628211ULL);
	}

	static _FORCEINLINE constexpr std::uint32_t Elf_hash_constexpr(const char* str) {
		std::uint32_t HashValue = 0;
		std::uint32_t x = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = *str;

			HashValue = (HashValue << 4) + byte;

			if ((x = HashValue & 0xF0000000) != 0) {
				HashValue ^= (x >> 24);
				HashValue &= ~x;
			}


			++str;
		} while (*str);


		return HashValue;
	}

	static _FORCEINLINE constexpr std::uint32_t Elfa1_hash_constexpr(const char* str) {
		std::uint32_t HashValue = 0;
		std::uint32_t x = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = Tolower(*str);
			HashValue = (HashValue << 4) + byte;

			if ((x = HashValue & 0xF0000000) != 0) {
				HashValue ^= (x >> 24);
				HashValue &= ~x;
			}


			++str;
		} while (*str);


		return HashValue;
	}
	
	static _FORCEINLINE constexpr std::uint32_t Elf_hash_constexpr(const wchar_t* str) {
		std::uint32_t HashValue = 0;
		std::uint32_t x = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = *str;

			HashValue = (HashValue << 4) + byte;

			if ((x = HashValue & 0xF0000000) != 0) {
				HashValue ^= (x >> 24);
				HashValue &= ~x;
			}


			++str;
		} while (*str);


		return HashValue;
	}

	static _FORCEINLINE constexpr std::uint32_t Elfa1_hash_constexpr(const wchar_t* str) {
		std::uint32_t HashValue = 0;
		std::uint32_t x = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = Tolower(*str);
			HashValue = (HashValue << 4) + byte;

			if ((x = HashValue & 0xF0000000) != 0) {
				HashValue ^= (x >> 24);
				HashValue &= ~x;
			}


			++str;
		} while (*str);


		return HashValue;
	}

	static _FORCEINLINE constexpr std::uint32_t HashBytesELF(const std::uint8_t* data, std::size_t len) {
		std::uint32_t HashValue = 0;
		std::uint32_t x = 0;

		while (len != 0) {
			auto byte = *data;

			HashValue = (HashValue << 4) + byte;

			if ((x = HashValue & 0xF0000000) != 0) {
				HashValue ^= (x >> 24);
				HashValue &= ~x;
			}

			++data;
			--len;
		}

		return HashValue;
	}

	static _FORCEINLINE constexpr std::uint64_t Sip_hash(const char* data, std::size_t len) {
		std::uint64_t m = 0;
		std::uint64_t S = 0;
		std::uint64_t X = 0;
		std::uint64_t S3 = 0;
		std::uint64_t S2 = 0;
		const char* end = data + (len - (len % 8));

		S = 65539 ^ 0x736f6d6570736575ULL;
		X = 65539 ^ 0x646f72616e646f6dULL;
		S3 = 65535 ^ 0x6c7967656e657261ULL;
		S2 = 65535 ^ 0x7465646279746573ULL;

		for (; data != end; data += 8) {
			std::memcpy(&m, data, sizeof(m));
			S2 ^= m;
			for (int i = 0; i < 2; ++i) {
				S += X; X = (X << 13) | (X >> 51);
				X ^= S; S = (S << 32) | (S >> 32);
				S3 += S2; S2 = (S2 << 16) | (S2 >> 48);
				S2 ^= S3; S += S2;
				S2 = (S2 << 21) | (S2 >> 43);
				S2 ^= S; S3 += X;
				X = (X << 17) | (X >> 47);
				X ^= S3; S3 = (S3 << 32) | (S3 >> 32);
			}

			S ^= m;
		}

		m = 0;
		for (int i = 0; i < len % 8; ++i) {
			m |= static_cast<std::uint64_t>(data[i]) << (8 * i);
		}

		S2 ^= m;
		S += X; X = (X << 13) | (X >> 51);
		X ^= S; S = (S << 32) | (S >> 32);
		S3 += S2; S2 = (S2 << 16) | (S2 >> 48);
		S2 ^= S3; S += S2;
		S2 = (S2 << 21) | (S2 >> 43);
		S2 ^= S; S3 += X;
		X = (X << 17) | (X >> 47);
		X ^= S3; S3 = (S3 << 32) | (S3 >> 32);
		S ^= m;
		S3 ^= 0xff;

		for (int i = 0; i < 4; ++i) {
			S += X; X = (X << 13) | (X >> 51);
			X ^= S; S = (S << 32) | (S >> 32);
			S3 += S2; S2 = (S2 << 16) | (S2 >> 48);
			S2 ^= S3; S += S2;
			S2 = (S2 << 21) | (S2 >> 43);
			S2 ^= S; S3 += X;
			X = (X << 17) | (X >> 47);
			X ^= S3; S3 = (S3 << 32) | (S3 >> 32);
		}

		return S ^ X ^ S3 ^ S2;
	}

	static _FORCEINLINE constexpr std::uint32_t HashStringSDBM(const char* str) {
		std::uint32_t HashValue = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = *str;
			HashValue = 65599 * HashValue + byte;
			++str;
		} while (*str);

		return HashValue;
	}

	static _FORCEINLINE constexpr std::uint32_t HashStringSDBMA(const char* str) {
		std::uint32_t HashValue = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = Tolower (*str);
			HashValue = 65599 * HashValue + byte;
			++str;
		} while (*str);

		return HashValue;
	}
	
	static _FORCEINLINE constexpr std::uint32_t HashStringSDBM(const wchar_t* str) {
		std::uint32_t HashValue = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = *str;
			HashValue = 65599 * HashValue + byte;
			++str;
		} while (*str);

		return HashValue;
	}

	static _FORCEINLINE constexpr std::uint32_t HashStringSDBMA(const wchar_t* str) {
		std::uint32_t HashValue = 0;

		if (!*str) {
			return HashValue;
		}

		do {
			auto byte = Tolower(*str);
			HashValue = 65599 * HashValue + byte;
			++str;
		} while (*str);

		return HashValue;
	}

};

#define SDBM(str) (std::integral_constant<std::uint32_t, Hash::HashStringSDBM(str)>::value)
#define SDBMA(str) (std::integral_constant<std::uint32_t, Hash::HashStringSDBMA(str)>::value)

#define Sip(str) (std::integral_constant<std::uint64_t, Hash::Sip_hash(str,sizeof(str)-1)>::value)

#define FNV(str) (std::integral_constant<std::uint32_t, Hash::fnv1_hash_constexpr(str)>::value)
#define FNV64(str) (std::integral_constant<std::uint64_t, Hash::fnv1_hash_64_constexpr(str)>::value)

#define FNVA(str) (std::integral_constant<std::uint32_t, Hash::fnv1a_hash_constexpr(str)>::value)
#define FNVA64(str) (std::integral_constant<std::uint64_t, Hash::fnv1a_hash_64_constexpr(str)>::value)

#define ELF(str) (std::integral_constant<std::uint32_t, Hash::Elf_hash_constexpr(str)>::value)
#define ELFA(str) (std::integral_constant<std::uint32_t, Hash::Elfa1_hash_constexpr(str)>::value)


#endif