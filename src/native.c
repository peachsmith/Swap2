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
	char* app_path = jep_get_path();
	if (app_path != NULL)
	{
		printf("app path: %s\n", app_path);
		free(app_path);
	}
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

/**
* gets the full path to the executable
*/
char* jep_get_path()
{
	errno = 0;
	int app_path_size = 1024;
	char* app_path = malloc(app_path_size);

#if defined(_WIN32) || defined(__CYGWIN__)

	/* get the path to the application */
	GetModuleFileName(NULL, app_path, app_path_size);

	if (errno)
	{
		free(app_path);
		return NULL;
	}

	/* remove the application name from the path */
	int i = strlen(app_path);
	for (; app_path[i] != '\\' && app_path[i] != '/' && i > 0; i--)
	{
		app_path[i] = '\0';
	}

#elif defined(__linux__)
	ssize_t len;

	len = readlink("/proc/self/exe", app_path, app_path_size - 1);
	if (len == -1)
	{
		free(app_path);
		return NULL;
	}

	app_path[len] = '\0';

	/* remove the application name from the path */
	int i = strlen(app_path);
	for (; app_path[i] != '/' && i > 0; i--)
	{
		app_path[i] = '\0';
	}
#endif

	return app_path;
}
