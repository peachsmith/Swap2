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
#include "native.h"

/* calls a native function */
jep_obj *jep_call_native(const char *ident, jep_obj *args)
{
	jep_obj *o = NULL;

	jep_lib lib = jep_load_lib(SWAP_NATIVE_LIB);
	if (lib != NULL)
	{
		size_t ident_len = strlen(ident);
		char* native_ident = malloc(ident_len + 6);
		strcpy(native_ident, "jep_");
		strcat(native_ident, ident);
		jep_func func = jep_get_func(lib, native_ident);

		if (func != NULL)
		{
			return func(args);
		}
		else
		{
			printf("could not load function %s from shared library\n", native_ident);
		}

		free(native_ident);
		jep_free_lib(lib);
	}
	else
	{
		printf("could not load shared library\n");
	}

	return o;
}

/**
* loads a shared library
*/
jep_lib jep_load_lib(const char* lib_name)
{
	jep_lib lib = NULL;

#if defined(_WIN32) || defined(__CYGWIN__)
	lib = LoadLibrary(TEXT(lib_name));
#endif

	return lib;
}

/**
* frees the memory occupied by a shared library
*/
void jep_free_lib(jep_lib lib)
{

#if defined(_WIN32) || defined(__CYGWIN__)
	FreeLibrary(lib);
#endif

}

/**
* loads a function from a shared library
*/
jep_func jep_get_func(jep_lib lib, const char* func_name)
{
	jep_func func = NULL;

#if defined(_WIN32) || defined(__CYGWIN__)
	func = (jep_func)GetProcAddress(lib, func_name);
#endif

	return func;
}
