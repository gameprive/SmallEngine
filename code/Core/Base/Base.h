#pragma once

#define STRINGIZE_2( n ) #n
#define STRINGIZE( n ) STRINGIZE_2( n )
#define TODO(msg) __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : TODO: " msg))