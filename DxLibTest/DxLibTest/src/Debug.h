#pragma once
#if _DEBUG
#include <Windows.h>
#include <utility>
#include <iostream>

class debug_console
{
	std::FILE*	console_;
public:

	debug_console() :
		console_( nullptr )
	{
		AllocConsole();
		freopen_s( &console_, "CONOUT$", "w", stdout );
		freopen_s( &console_, "CONIN$",  "r", stdin );
	}

	~debug_console()
	{
		FreeConsole();
	}

	template< typename T >
	static void print( T&& t )
	{
		std::cout << t << std::endl;
	}

	template< typename Head, typename... Tail >
	static void print( Head&& h, Tail&&... tail )
	{
		print( h );
		print( tail... );
	}

	template< typename Head, typename... Tail >
	static void print( bool&& h, Tail&&... tail )
	{
		std::cout << std::boolalpha << h << std::endl;
		print( tail... );
	}
};
#else

class debug_console
{
public:

	debug_console() = default;
	~debug_console() = default;

	static void print( ... ) {}
};

#endif
