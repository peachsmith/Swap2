/*
	Natively implemented functions
	Copyright (C) 2016 John Powell

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef JEP_NATIVE_H
#define JEP_NATIVE_H

#include "object.h"

#if defined(_WIN32) || defined(__CYGWIN__)
#	include <windows.h>
#	define SWAP_NATIVE_LIB "SwapNative.dll"
	typedef HINSTANCE jep_lib;
	typedef jep_obj* (__cdecl *jep_func)(jep_obj*);
#elif defined(__linux__) || defined(__unix__)
#	include <unistd.h>
#	include <dlfcn.h>
#	define SWAP_NATIVE_LIB "libSwapNative.so"
	typedef void* jep_lib;
	typedef jep_obj* (*jep_func)(jep_obj*);
#endif

/**
 * calls a native function
 */
jep_obj* jep_call_native(const char* ident, jep_obj* args);

/**
 * loads a shared library
 */
jep_lib jep_load_lib(const char* lib_name);

/**
* frees the memory occupied by a shared library
*/
void jep_free_lib(jep_lib lib);

/**
* loads a function from a shared library
*/
jep_func jep_get_func(jep_lib lib, const char* func_name);

/**
 * gets the full path to the executable
 */
char* jep_get_path();

#endif
