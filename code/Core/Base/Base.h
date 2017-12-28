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