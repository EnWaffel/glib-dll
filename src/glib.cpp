#include "glib/glib.h"
#include "glib/utils/glibGlobals.h"
#include <iostream>

using namespace glib;

Instance* __glib_instance = nullptr;
int __GLIB_ERROR_CODE = -1;

GINSTANCE gInstance = nullptr;
GWINDOW gWindow = nullptr;
GWINDOW gWindow1 = nullptr;
GWINDOW gWindow2 = nullptr;
GWINDOW gWindow3 = nullptr;

void glib_print_error()
{
	if (__GLIB_ERROR_CODE == -1)
	{
		std::cout << "glib Error: INVALID ERROR CODE" << std::endl;
	}
	else
	{
		std::cout << "glib Error: 0x" << std::hex << __GLIB_ERROR_CODE << std::endl;
	}
	__GLIB_ERROR_CODE = -1;
}

Instance* glib::Init()
{
	if (__glib_instance != nullptr) return __glib_instance;
	__glib_instance = new Instance;

	if (__GLIB_ERROR_CODE != -1)
	{
		glib_print_error();
		delete __glib_instance;
		return nullptr;
	}

	return __glib_instance;
}

void glib::Terminate()
{
	if (__glib_instance == nullptr) return;
	delete __glib_instance;
}
