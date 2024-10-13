#pragma once

// The DLL Defines

#ifdef GLIBDLL_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif