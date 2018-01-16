#pragma once

// Макрос TODO
#define STRINGIZE_2( n ) #n
#define STRINGIZE( n ) STRINGIZE_2( n )
#define TODO(msg) __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : TODO: " msg))

#if (defined( _MSC_VER ) && (_MSC_VER == 1800))
#	define NORETURN  __declspec(noreturn)
#else
#	define NORETURN  [[noreturn]]
#endif

#if (defined( _MSC_VER ) && (_MSC_VER >= 1900)) || defined(__APPLE__) || defined(__GNUC__)
#	define ALIGNAS( _align ) alignas( _align )
#else
#	define ALIGNAS( _align ) __declspec(align(_align))
#endif

#define MAKEFOURCC(_a, _b, _c, _d) ( ( (uint32_t)(_a) | ( (uint32_t)(_b) << 8) | ( (uint32_t)(_c) << 16) | ( (uint32_t)(_d) << 24) ) )

// http://cnicholson.net/2011/01/stupid-c-tricks-a-better-sizeof_array/
template<typename T, size_t N>
char(&COUNTOF_REQUIRES_ARRAY_ARGUMENT(const T(&)[N]))[N];
#define COUNTOF(x) sizeof(COUNTOF_REQUIRES_ARRAY_ARGUMENT(x) )

inline uint32_t endianSwap(uint32_t in)
{
	return (in >> 24) | (in << 24)
		| ((in & 0x00ff0000) >> 8) | ((in & 0x0000ff00) << 8)
		;
}

template <typename T>
inline T toHostEndian(const T in, bool fromLittleEndian)
{
	return fromLittleEndian ? in : endianSwap(in);
}