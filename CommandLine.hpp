#ifndef _CommandLine_
#define _CommandLine_

#include <cstdint>
#include <cstring>
#include <AString.hpp>
#include <unordered_map>
#include <lazy_importer.hpp>
#include <Stream.hpp>
#include <Hash.hpp>
#include <windows.h>
#include <Array.hpp>

enum class CommonDataType : std::uint8_t {
	None = 0 ,      // No value (flag only)
	Bool ,          // Boolean value
	Int32 ,         // Signed 32-bit integer
	UInt32 ,        // Unsigned 32-bit integer
	Int64 ,         // Signed 64-bit integer
	UInt64 ,        // Unsigned 64-bit integer
	Float ,         // Single precision floating point
	Double ,        // Double precision floating point
	String          // String value
};


class CommonData {
public:
	CommonData( ) noexcept : m_String( ) , m_Type( CommonDataType::None ) , m_Value( ) {}

	CommonDataType GetType( ) const noexcept { return m_Type; }
	
	bool IsBool( ) const noexcept { return m_Type == CommonDataType::Bool; }
	
	bool IsInt( ) const noexcept { return m_Type == CommonDataType::Int32 || m_Type == CommonDataType::Int64 || m_Type == CommonDataType::UInt32 || m_Type == CommonDataType::UInt64; }

	bool IsFloat( ) const noexcept { return m_Type == CommonDataType::Float || m_Type == CommonDataType::Double; }

	bool IsString( ) const noexcept { return m_Type == CommonDataType::String; }

	bool IsNone( ) const noexcept { return m_Type == CommonDataType::None; }


	bool GetBool( bool defaultValue = false ) const noexcept {
		if ( m_Type == CommonDataType::Bool ) return m_Value.b;
		return defaultValue;
	}

	std::int32_t GetInt32( std::int32_t defaultValue = 0 ) const noexcept {
		switch ( m_Type ) {
			case CommonDataType::Int32: return m_Value.i32;
			case CommonDataType::Int64: return static_cast< std::int32_t >( m_Value.i64 );
			case CommonDataType::UInt32: return static_cast< std::int32_t >( m_Value.u32 );
			case CommonDataType::UInt64: return static_cast< std::int32_t >( m_Value.u64 );
			default: return defaultValue;
		}
	}

	std::int64_t GetInt64( std::int64_t defaultValue = 0 ) const noexcept {
		switch ( m_Type ) {
			case CommonDataType::Int32: return static_cast< std::int64_t >( m_Value.i32 );
			case CommonDataType::Int64: return m_Value.i64;
			case CommonDataType::UInt32: return static_cast< std::int64_t >( m_Value.u32 );
			case CommonDataType::UInt64: return static_cast< std::int64_t >( m_Value.u64 );
			default: return defaultValue;
		}
	}

	std::uint32_t GetUInt32( std::uint32_t defaultValue = 0 ) const noexcept {
		switch ( m_Type ) {
			case CommonDataType::Int32: return static_cast< std::uint32_t >( m_Value.i32 );
			case CommonDataType::Int64: return static_cast< std::uint32_t >( m_Value.i64 );
			case CommonDataType::UInt32: return m_Value.u32;
			case CommonDataType::UInt64: return static_cast< std::uint32_t >( m_Value.u64 );
			default: return defaultValue;
		}
	}

	std::uint64_t GetUInt64( std::uint64_t defaultValue = 0 ) const noexcept {
		switch ( m_Type ) {
			case CommonDataType::Int32: return static_cast< std::uint64_t >( m_Value.i32 );
			case CommonDataType::Int64: return static_cast< std::uint64_t >( m_Value.i64 );
			case CommonDataType::UInt32: return static_cast< std::uint64_t >( m_Value.u32 );
			case CommonDataType::UInt64: return m_Value.u64;
			default: return defaultValue;
		}
	}

	float GetFloat( float defaultValue = 0.0f ) const noexcept {
		switch ( m_Type ) {
			case CommonDataType::Float: return m_Value.f32;
			case CommonDataType::Double: return static_cast< float >( m_Value.f64 );
			default: return defaultValue;
		}
	}

	double GetDouble( double defaultValue = 0.0 ) const noexcept {
		switch ( m_Type ) {
			case CommonDataType::Float: return static_cast< double >( m_Value.f32 );
			case CommonDataType::Double: return m_Value.f64;
			default: return defaultValue;
		}
	}

	const WString& GetString( ) const noexcept {
		return m_String;
	}


	void SetBool( bool value ) noexcept {
		m_Type = CommonDataType::Bool;
		m_Value.b = value;
		m_String.Clear( );
	}

	void SetInt32( std::int32_t value ) noexcept {
		m_Type = CommonDataType::Int32;
		m_Value.i32 = value;
		m_String.Clear( );
	}

	void SetInt64( std::int64_t value ) noexcept {
		m_Type = CommonDataType::Int64;
		m_Value.i64 = value;
		m_String.Clear( );
	}

	void SetUInt32( std::uint32_t value ) noexcept {
		m_Type = CommonDataType::UInt32;
		m_Value.u32 = value;
		m_String.Clear( );
	}

	void SetUInt64( std::uint64_t value ) noexcept {
		m_Type = CommonDataType::UInt64;
		m_Value.u64 = value;
		m_String.Clear( );
	}

	void SetFloat( float value ) noexcept {
		m_Type = CommonDataType::Float;
		m_Value.f32 = value;
		m_String.Clear( );
	}

	void SetDouble( double value ) noexcept {
		m_Type = CommonDataType::Double;
		m_Value.f64 = value;
		m_String.Clear( );
	}

	void SetString( const WString& value ) {
		m_Type = CommonDataType::String;
		m_String = value;
		m_Value.u64 = 0;
	}

	void SetString( WString&& value ) noexcept {
		m_Type = CommonDataType::String;
		m_String = std::move( value );
		m_Value.u64 = 0;
	}

	void SetNone( ) noexcept {
		m_Type = CommonDataType::None;
		m_String.Clear( );
		m_Value.u64 = 0;
	}

	void Clear( ) noexcept {
		SetNone( );
	}
protected:
	WString m_String;
	CommonDataType m_Type;

	union ValueUnion {
		bool b;
		std::int32_t i32;
		std::int64_t i64;
		std::uint32_t u32;
		std::uint64_t u64;
		float f32;
		double f64;

		ValueUnion( ) : u64( 0 ) {}
	} m_Value;
};

/**
 * @brief Parse command line arguments into a structured format with automatic type detection
 *
 * @details
 * This function parses Windows command line arguments and automatically detects value types.
 *
 * SUPPORTED FORMATS:
 * - Single dash:     -key=value, -key value, -key
 * - Double dash:     --key=value, --key value, --key
 * - Slash:          /key (flag only)
 * - Config file:    @filepath (load arguments from file)
 *
 * VALUE TYPES (Auto-detected):
 * - Bool:    true, false, yes, no, on, off, 1, 0
 * - Int32:   -2147483648 to 2147483647
 * - UInt32:  0 to 4294967295, 0xFF (hex)
 * - Int64:   Values outside Int32 range
 * - UInt64:  Values outside UInt32 range, numeric suffixes
 * - Float:   3.14f, 0.5F
 * - Double:  3.14159, 1.23e10
 * - String:  Any text
 *
 * NUMERIC SUFFIXES:
 * - K/KB:  Kilobytes (1024)
 * - M/MB:  Megabytes (1024²)
 * - G/GB:  Gigabytes (1024³)
 * - T/TB:  Terabytes (1024⁴)
 * Example: -memory=2G → 2147483648
 *
 * ESCAPE SEQUENCES:
 * - \n, \t, \r, \\, \", \', \0
 * - \xHH:    Hexadecimal character (e.g., \x41 = 'A')
 * - \uXXXX:  Unicode character (e.g., \u4E2D = '中')
 *
 * ENVIRONMENT VARIABLES:
 * - Windows: %VARNAME%
 * - Unix:    ${VARNAME}
 * Example: -path=%TEMP%\data
 *
 * CONFIG FILES (@filepath):
 * - One argument per line or space-separated
 * - Comments: # or //
 * - Supports all command line formats
 * - Max size: 1MB
 * Example: program.exe @config.txt -verbose
 *
 * QUOTES:
 * - Single quotes: 'text'
 * - Double quotes: "text"
 * - Can be nested: -msg="It's \"cool\""
 *
 * LIMITATIONS:
 * - Keys are case-sensitive (-Key ≠ -key)
 * - Duplicate keys: last value wins
 * - Hash collision: theoretically possible but extremely rare (FNV-1a)
 *
 * @param Command Output map: Hash(key) -> CommonData
 *                Keys are hashed using Hash::fnv1a_hash_constexpr
 *
 * @note This function skips the program name automatically
 * @note Use CommonData getter methods to retrieve typed values
 *
 * @example
 * std::unordered_map<std::uint32_t, CommonData> cmd;
 * Common::common_wincmdln(cmd);
 *
 * int port = cmd[FNVA( L"port" )].GetInt32(8080); // Default: 8080
 *
 * const WString& path = cmd[FNVA( L"path" )].GetString();
 *
 * @see CommonData for value access methods
 * @see CommonDataType for type enumeration
 */
class CommandLine {
public:
	using Map = std::unordered_map<std::uint32_t , CommonData>;

	static void Get( Map& Command ) {
		Command.clear( );
		Command.reserve( 16 );

		wchar_t* command_line = LI_FN( GetCommandLineW )( );

		if ( !command_line || *command_line == L'\0' ) {
			return;
		}

		SkipProgramName( command_line );

		WString CommandLine = command_line;
		const wchar_t* p = CommandLine.c_str( );

		auto ParseValue = [ ] ( const WString& value ) -> CommonData {
			CommonData data;

			if ( value.IsEmpty( ) ) {
				data.SetBool( true );
				return data;
			}

			WString expanded_value = ExpandEnvironmentVariables( value );

			if ( !expanded_value.CompareI( L"true" ) || !expanded_value.CompareI( L"yes" ) || !expanded_value.CompareI( L"on" ) || !expanded_value.CompareI( L"1" ) ) {
				data.SetBool( true );
				return data;
			}
			if ( !expanded_value.CompareI( L"false" ) || !expanded_value.CompareI( L"no" ) || !expanded_value.CompareI( L"off" ) || !expanded_value.CompareI( L"0" ) ) {
				data.SetBool( false );
				return data;
			}

			const wchar_t* str = expanded_value.c_str( );
			wchar_t* end = nullptr;

			std::uint64_t suffix_val = 0;
			if ( ParseNumericSuffix( expanded_value , suffix_val ) ) {
				if ( suffix_val <= UINT32_MAX ) {
					data.SetUInt32( static_cast< std::uint32_t >( suffix_val ) );
				}
				else {
					data.SetUInt64( suffix_val );
				}
				return data;
			}

			errno = 0;
			double f64_val = wcstod( str , &end );

			if ( end && end != str && errno != ERANGE ) {
				bool has_dot = expanded_value.contains( L'.' );
				bool has_exp = expanded_value.contains( L'e' ) || expanded_value.contains( L'E' );
				bool has_f_suffix = false;

				if ( end && ( *end == L'f' || *end == L'F' ) ) {
					has_f_suffix = true;
					++end;
				}

				if ( *end == L'\0' && ( has_dot || has_exp || has_f_suffix ) ) {
					if ( has_f_suffix || ( f64_val >= -FLT_MAX && f64_val <= FLT_MAX && !has_exp ) ) {
						data.SetFloat( static_cast< float >( f64_val ) );
					}
					else {
						data.SetDouble( f64_val );
					}
					return data;
				}
			}

			end = nullptr;
			errno = 0;
			std::int64_t i64_val = _wcstoi64( str , &end , 0 );

			if ( end && end != str && *end == L'\0' && errno != ERANGE ) {
				if ( i64_val >= INT32_MIN && i64_val <= INT32_MAX ) {
					data.SetInt32( static_cast< std::int32_t >( i64_val ) );
				}
				else {
					data.SetInt64( i64_val );
				}
				return data;
			}

			end = nullptr;
			errno = 0;
			std::uint64_t u64_val = _wcstoui64( str , &end , 0 );

			if ( end && end != str && *end == L'\0' && errno != ERANGE ) {
				if ( u64_val <= UINT32_MAX ) {
					data.SetUInt32( static_cast< std::uint32_t >( u64_val ) );
				}
				else {
					data.SetUInt64( u64_val );
				}
				return data;
			}

			// Not a valid number, treat as string
			// Remove any remaining surrounding quotes (in case they survived tokenization)
			WString final_string = expanded_value;
			if ( final_string.length( ) >= 2 ) {
				if ( ( final_string [ 0 ] == L'"' && final_string [ final_string.length( ) - 1 ] == L'"' ) ||
					 ( final_string [ 0 ] == L'\'' && final_string [ final_string.length( ) - 1 ] == L'\'' ) ) {
					final_string = final_string.substr( 1 , final_string.length( ) - 2 );
				}
			}
			data.SetString( final_string );
			return data;
			};

		Array<WString> tokens;
		tokens.reserve( 16 );

		while ( *p ) {
			while ( *p == L' ' || *p == L'\t' ) ++p;
			if ( !*p ) break;

			bool in_double_quotes = false;
			bool in_single_quotes = false;
			const wchar_t* start = p;

			// Extract token
			while ( *p && ( in_double_quotes || in_single_quotes || ( *p != L' ' && *p != L'\t' ) ) ) {
				if ( *p == L'"' && !in_single_quotes ) {
					std::size_t backslash_count = CountBackslashes( start , p );
					if ( backslash_count % 2 == 0 ) {
						in_double_quotes = !in_double_quotes;
					}
				}
				else if ( *p == L'\'' && !in_double_quotes ) {
					std::size_t backslash_count = CountBackslashes( start , p );
					if ( backslash_count % 2 == 0 ) {
						in_single_quotes = !in_single_quotes;
					}
				}
				++p;
			}

			if ( p > start ) {
				WString token;
				token.Assign( start , static_cast< std::size_t >( p - start ) );
				tokens.push_back( std::move( token ) );
			}
		}

		Array<WString> expanded_tokens;
		expanded_tokens.reserve( static_cast< Array<WString>::size_type >( tokens.size( ) ) );

		for ( Array<WString>::size_type i = 0; i < tokens.size( ); ++i ) {
			if ( tokens [ i ].BeginsWith( L"@" ) ) {
				WString filepath = tokens [ i ].substr( 1 );
				filepath = ExpandEnvironmentVariables( filepath );

				Array<WString> file_args;
				if ( LoadResponseFile( filepath , file_args ) ) {
					for ( Array<WString>::size_type j = 0; j < file_args.size( ); ++j ) {
						expanded_tokens.push_back( std::move( file_args [ j ] ) );
					}
				}
				else {
					expanded_tokens.push_back( std::move( tokens [ i ] ) );
				}
			}
			else {
				expanded_tokens.push_back( std::move( tokens [ i ] ) );
			}
		}

		tokens = std::move( expanded_tokens );

		for ( Array<WString>::size_type i = 0; i < tokens.size( ); ++i ) {
			const WString& token = tokens [ i ];

			if ( token.IsEmpty( ) ) continue;

			if ( token.BeginsWith( L"--" ) ) {
				WString key = token.substr( 2 );

				if ( key.IsEmpty( ) ) continue;

				auto pos = FindEqualSignOutsideQuotes( key );

				if ( pos != WString::npos ) {
					WString k = key.substr( 0 , pos );
					WString v = key.substr( pos + 1 );

					if ( k.IsEmpty( ) ) continue;

					v = RemoveQuotesAndEscapes( v );
					std::uint32_t hash = Hash::fnv1a_hash_constexpr( k.c_str( ) );

					Command [ hash ] = ParseValue( v );
				}
				else {
					if ( i + 1 < tokens.size( ) ) {
						const WString& next_raw = tokens [ i + 1 ];
						if ( !next_raw.BeginsWith( L"-" ) && !next_raw.BeginsWith( L"/" ) &&
							!next_raw.BeginsWith( L"\"--" ) && !next_raw.BeginsWith( L"\"-" ) &&
							!next_raw.BeginsWith( L"'--" ) && !next_raw.BeginsWith( L"'-" ) ) {
							
							++i;
							WString next_token = RemoveQuotesAndEscapes( next_raw );
							std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
							Command [ hash ] = ParseValue( next_token );
						}
						else {
							std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
							Command.emplace( hash , ParseValue( WString( ) ) );
						}
					}
					else {
						std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
						Command.emplace( hash , ParseValue( WString( ) ) );
					}
				}
			}
			else if ( token.BeginsWith( L"-" ) && !token.BeginsWith( L"--" ) ) {
				WString key = token.substr( 1 );

				if ( key.IsEmpty( ) ) continue;

				auto pos = FindEqualSignOutsideQuotes( key );

				if ( pos != WString::npos ) {
					// Format: -key=value
					WString k = key.substr( 0 , pos );
					WString v = key.substr( pos + 1 );

					if ( k.IsEmpty( ) ) continue;

					v = RemoveQuotesAndEscapes( v );
					std::uint32_t hash = Hash::fnv1a_hash_constexpr( k.c_str( ) );
					Command [ hash ] = ParseValue( v );
				}
				else {
					if ( i + 1 < tokens.size( ) ) {
						const WString& next_raw = tokens [ i + 1 ];
						if ( !next_raw.BeginsWith( L"-" ) && !next_raw.BeginsWith( L"/" ) &&
							!next_raw.BeginsWith( L"\"--" ) && !next_raw.BeginsWith( L"\"-" ) &&
							!next_raw.BeginsWith( L"'--" ) && !next_raw.BeginsWith( L"'-" ) ) {

							++i;
							WString next_token = RemoveQuotesAndEscapes( next_raw );
							std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
							Command [ hash ] = ParseValue( next_token );
						}
						else {
							std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
							Command.emplace( hash , ParseValue( WString( ) ) );
						}
					}
					else {
						std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
						Command.emplace( hash , ParseValue( WString( ) ) );
					}
				}
			}

			else if ( token.BeginsWith( L"/" ) ) {
				WString key = token.substr( 1 );

				if ( key.IsEmpty( ) ) continue;

				std::uint32_t hash = Hash::fnv1a_hash_constexpr( key.c_str( ) );
				Command.emplace( hash , ParseValue( WString( ) ) );
			}
		}
	}

private:
	static void SkipProgramName( wchar_t*& command_line ) noexcept {
		bool in_quotes = false;
		while ( *command_line > L' ' || ( *command_line != L'\0' && in_quotes ) ) {
			if ( *command_line == L'"' ) in_quotes = !in_quotes;
			++command_line;
		}

		while ( *command_line != L'\0' && *command_line <= L' ' ) {
			++command_line;
		}
	}

	static std::size_t CountBackslashes( const wchar_t* str , const wchar_t* pos ) noexcept {
		if ( pos <= str ) return 0;

		std::size_t count = 0;
		--pos;
		while ( pos >= str && *pos == L'\\' ) {
			++count;
			--pos;
		}
		return count;
	}

	static bool ParseNumericSuffix( const WString& str , std::uint64_t& result ) {
		if ( str.IsEmpty( ) ) return false;

		const wchar_t* p = str.c_str( );
		wchar_t* end = nullptr;

		errno = 0;
		std::uint64_t base_val = _wcstoui64( p , &end , 0 );

		if ( !end || end == p || errno == ERANGE ) {
			return false;
		}

		std::uint64_t multiplier = 1;

		if ( *end != L'\0' ) {
			wchar_t suffix_upper = towupper( *end );
			++end;

			bool has_b = false;
			if ( *end == L'B' || *end == L'b' ) {
				has_b = true;
				++end;
			}

			switch ( suffix_upper ) {
				case L'K': multiplier = 1024; break;
				case L'M': multiplier = 1024ULL * 1024; break;
				case L'G': multiplier = 1024ULL * 1024 * 1024; break;
				case L'T': multiplier = 1024ULL * 1024 * 1024 * 1024; break;
				default:
					return false;
			}

			
			if ( *end != L'\0' ) {
				return false;
			}
		}

		if ( base_val > UINT64_MAX / multiplier ) {
			return false;
		}

		result = base_val * multiplier;
		return true;
	}

	static WString ExpandEnvironmentVariables( const WString& input ) {
		if ( !input.contains( L'%' ) && !input.contains( L'$' ) ) {
			return input;
		}

		WString result;
		const wchar_t* p = input.c_str( );
		const wchar_t* end = p + input.length( );

		while ( p < end ) {
			if ( *p == L'%' ) {
				const wchar_t* var_start = p + 1;
				const wchar_t* var_end = var_start;

				while ( var_end < end && *var_end != L'%' && *var_end != L'\0' ) {
					++var_end;
				}

				if ( *var_end == L'%' ) {
					WString var_name;
					var_name.Assign( var_start , static_cast< std::size_t >( var_end - var_start ) );

					DWORD size = LI_FN( GetEnvironmentVariableW ) ( var_name.c_str( ) , nullptr , 0 );

					if ( size > 0 ) {
						Array<wchar_t> buffer( size );
						DWORD actual_size = LI_FN( GetEnvironmentVariableW ) ( var_name.c_str( ) , buffer.data( ) , size );

						if ( actual_size > 0 && actual_size < size ) {
							result.Append( buffer.data( ) );
							p = var_end + 1;
							continue;
						}
					}

					result.Append( L'%' );
					result.Append( var_name );
					result.Append( L'%' );
					p = var_end + 1;
					continue;
				}
			}
			else if ( *p == L'$' && ( p + 1 ) < end && *( p + 1 ) == L'{' ) {
				const wchar_t* var_start = p + 2;
				const wchar_t* var_end = var_start;

				while ( var_end < end && *var_end != L'}' && *var_end != L'\0' ) {
					++var_end;
				}

				if ( *var_end == L'}' ) {
					WString var_name;
					var_name.Assign( var_start , static_cast< std::size_t >( var_end - var_start ) );

					DWORD size = LI_FN( GetEnvironmentVariableW ) ( var_name.c_str( ) , nullptr , 0 );

					if ( size > 0 ) {
						Array<wchar_t> buffer( size );
						DWORD actual_size = LI_FN( GetEnvironmentVariableW ) ( var_name.c_str( ) , buffer.data( ) , size );

						if ( actual_size > 0 && actual_size < size ) {
							result.Append( buffer.data( ) );
							p = var_end + 1;
							continue;
						}
					}

					result.Append( L'$' );
					result.Append( L'{' );
					result.Append( var_name );
					result.Append( L'}' );
					p = var_end + 1;
					continue;
				}
			}

			result.Append( *p );
			++p;
		}

		return result;
	}

	static bool LoadResponseFile( const WString& filepath , Array<WString>& args ) {
		IOStream stream( filepath );

		if ( !stream.Is_Open( ) ) {
			return false;
		}

		std::uint64_t fileSize = stream.Size( );

		const std::uint64_t MAX_CONFIG_SIZE = 1024 * 1024;
		if ( fileSize == 0 || fileSize > MAX_CONFIG_SIZE ) {
			return false;
		}

		Array<std::uint8_t> fileData;
		if ( !stream.ReadBuffer( fileData ) ) {
			return false;
		}

		const std::uint8_t* data_ptr = fileData.data( );
		std::size_t data_size = fileData.size( );

		WString content;

		if ( data_size >= 2 ) {
			if ( data_ptr [ 0 ] == 0xFF && data_ptr [ 1 ] == 0xFE ) {
				const wchar_t* wchar_data = reinterpret_cast< const wchar_t* >( data_ptr + 2 );
				std::size_t wchar_count = ( data_size - 2 ) / sizeof( wchar_t );
				content.Assign( wchar_data , wchar_count );
			}
			else if ( data_ptr [ 0 ] == 0xFE && data_ptr [ 1 ] == 0xFF ) {
				return false;
			}
			else if ( data_size >= 3 && data_ptr [ 0 ] == 0xEF && data_ptr [ 1 ] == 0xBB && data_ptr [ 2 ] == 0xBF ) {
				const char* char_data = reinterpret_cast< const char* >( data_ptr + 3 );
				std::size_t char_count = data_size - 3;

				int required = MultiByteToWideChar( CP_UTF8 , 0 , char_data , static_cast< int >( char_count ) , nullptr , 0 );
				if ( required > 0 ) {
					Array<wchar_t> wbuffer( required + 1 );
					MultiByteToWideChar( CP_UTF8 , 0 , char_data , static_cast< int >( char_count ) , wbuffer.data( ) , required );
					wbuffer [ required ] = L'\0';
					content.Assign( wbuffer.data( ) , required );
				}
			}
			else {
				const char* char_data = reinterpret_cast< const char* >( data_ptr );
				std::size_t char_count = data_size;

				int required = MultiByteToWideChar( CP_UTF8 , 0 , char_data , static_cast< int >( char_count ) , nullptr , 0 );
				if ( required > 0 ) {
					Array<wchar_t> wbuffer( required + 1 );
					int result = MultiByteToWideChar( CP_UTF8 , 0 , char_data , static_cast< int >( char_count ) , wbuffer.data( ) , required );
					if ( result > 0 ) {
						wbuffer [ required ] = L'\0';
						content.Assign( wbuffer.data( ) , required );
					}
					else {
						required = MultiByteToWideChar( CP_ACP , 0 , char_data , static_cast< int >( char_count ) , nullptr , 0 );
						if ( required > 0 ) {
							Array<wchar_t> wbuffer2( required + 1 );
							MultiByteToWideChar( CP_ACP , 0 , char_data , static_cast< int >( char_count ) , wbuffer2.data( ) , required );
							wbuffer2 [ required ] = L'\0';
							content.Assign( wbuffer2.data( ) , required );
						}
					}
				}
			}
		}

		if ( content.IsEmpty( ) ) {
			return false;
		}

		const wchar_t* p = content.c_str( );
		const wchar_t* end = p + content.length( );

		while ( p < end ) {
			while ( p < end && ( *p == L' ' || *p == L'\t' || *p == L'\r' || *p == L'\n' ) ) ++p;
			if ( p >= end ) break;

			if ( *p == L'#' || ( *p == L'/' && ( p + 1 ) < end && *( p + 1 ) == L'/' ) ) {
				while ( p < end && *p != L'\n' ) ++p;
				continue;
			}

			bool in_double_quotes = false;
			bool in_single_quotes = false;
			const wchar_t* token_start = p;

			while ( p < end ) {
				if ( *p == L'"' && !in_single_quotes ) {
					std::size_t backslash_count = CountBackslashes( token_start , p );
					if ( backslash_count % 2 == 0 ) {
						in_double_quotes = !in_double_quotes;
					}
				}
				else if ( *p == L'\'' && !in_double_quotes ) {
					std::size_t backslash_count = CountBackslashes( token_start , p );
					if ( backslash_count % 2 == 0 ) {
						in_single_quotes = !in_single_quotes;
					}
				}
				else if ( !in_double_quotes && !in_single_quotes &&
						( *p == L' ' || *p == L'\t' || *p == L'\r' || *p == L'\n' ) ) {
					break;
				}
				++p;
			}

			if ( p > token_start ) {
				WString arg;
				arg.Assign( token_start , static_cast< std::size_t >( p - token_start ) );
				args.push_back( std::move( arg ) );
			}
		}

		return true;
	}

	static WString ProcessEscapeSequences( const WString& input ) {
		if ( !input.contains( L'\\' ) ) {
			return input;
		}

		WString result;
		const wchar_t* p = input.c_str( );
		const wchar_t* end = p + input.length( );

		while ( p < end ) {
			if ( *p == L'\\' && ( p + 1 ) < end ) {
				++p;
				switch ( *p ) {
					case L'n':  result.Append( L'\n' ); break;
					case L't':  result.Append( L'\t' ); break;
					case L'r':  result.Append( L'\r' ); break;
					case L'\\': result.Append( L'\\' ); break;
					case L'"':  result.Append( L'"' ); break;
					case L'\'': result.Append( L'\'' ); break;
					case L'0':  result.Append( L'\0' ); break;
					case L'x': {
							
							if ( ( p + 2 ) < end ) {
								wchar_t hex [ 3 ] = { *( p + 1 ), *( p + 2 ), 0 };
								wchar_t* hex_end = nullptr;
								unsigned long val = wcstoul( hex , &hex_end , 16 );
								if ( hex_end == hex + 2 && val <= 0xFFFF ) {
									result.Append( static_cast< wchar_t >( val ) );
									p += 2;
									break;
								}
							}
							
							result.Append( L'\\' );
							result.Append( L'x' );
							break;
						}
					case L'u': {
							
							if ( ( p + 4 ) < end ) {
								wchar_t unicode [ 5 ] = { *( p + 1 ), *( p + 2 ), *( p + 3 ), *( p + 4 ), 0 };
								wchar_t* unicode_end = nullptr;
								unsigned long val = wcstoul( unicode , &unicode_end , 16 );
								if ( unicode_end == unicode + 4 && val <= 0xFFFF ) {
									result.Append( static_cast< wchar_t >( val ) );
									p += 4;
									break;
								}
							}
							
							result.Append( L'\\' );
							result.Append( L'u' );
							break;
						}
					default:
						
						result.Append( L'\\' );
						result.Append( *p );
						break;
				}
				++p;
			}
			else {
				result.Append( *p );
				++p;
			}
		}

		return result;
	}

	static WString RemoveQuotesAndEscapes( const WString& str ) {
		WString result = str;

		// First pass: remove surrounding quotes
		if ( result.length( ) > 1 ) {
			if ( ( result.BeginsWith( L'"' ) && result.EndsWith( L'"' ) ) ||
				( result.BeginsWith( L'\'' ) && result.EndsWith( L'\'' ) ) ) {
				result = result.substr( 1 , result.length( ) - 2 );
			}
		}

		// Process escape sequences
		result = ProcessEscapeSequences( result );
		
		// Second pass: remove quotes again in case escape processing revealed them
		if ( result.length( ) > 1 ) {
			if ( ( result [ 0 ] == L'"' && result [ result.length( ) - 1 ] == L'"' ) ||
				 ( result [ 0 ] == L'\'' && result [ result.length( ) - 1 ] == L'\'' ) ) {
				result = result.substr( 1 , result.length( ) - 2 );
			}
		}
		
		return result;
	}

	static std::size_t FindEqualSignOutsideQuotes( const WString& str ) {
		bool in_double_quotes = false;
		bool in_single_quotes = false;
		const wchar_t* p = str.c_str( );
		std::size_t index = 0;

		while ( *p ) {
			if ( *p == L'"' && !in_single_quotes ) {
				in_double_quotes = !in_double_quotes;
			}
			else if ( *p == L'\'' && !in_double_quotes ) {
				in_single_quotes = !in_single_quotes;
			}
			else if ( *p == L'=' && !in_double_quotes && !in_single_quotes ) {
				return index;
			}
			++p;
			++index;
		}

		return WString::npos;
	}
};








#endif
