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
#include "swap/native.h"

/* calls a native function */
jep_obj *jep_call_native(const char *ident, jep_obj *args)
{
	jep_obj *o = NULL;

	char *app_path = jep_get_app_path();
	if (app_path == NULL)
	{
		return NULL;
	}

	size_t a_len = strlen(app_path);
	size_t l_len = strlen(SWAP_NATIVE_LIB);

	char *lib_path = malloc(a_len + l_len + 1);
	strcpy(lib_path, app_path);
	strcat(lib_path, SWAP_NATIVE_LIB);
	lib_path[a_len + l_len] = '\0';

	jep_lib lib = jep_load_lib(lib_path);
	if (lib != NULL)
	{
		size_t ident_len = strlen(ident);
		char *native_ident = malloc(ident_len + 6);
		strcpy(native_ident, "jep_");
		strcat(native_ident, ident);
		jep_func func = jep_get_func(lib, native_ident);

		if (func != NULL)
		{
			o = func(args, NULL);
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

	free(app_path);
	free(lib_path);

	return o;
}

/**
* calls a function from a shared library
*/
jep_obj *jep_call_shared(jep_lib lib, const char *ident, jep_obj *args, jep_obj *list)
{
	jep_obj *o = NULL;
	size_t ident_len = strlen(ident);
	char *native_ident = malloc(ident_len + 6);
	strcpy(native_ident, "jep_");
	strcat(native_ident, ident);
	jep_func func = jep_get_func(lib, native_ident);

	if (func != NULL)
	{
		o = func(args, list);
	}
	else
	{
		printf("could not load function %s from shared library\n", native_ident);
	}

	free(native_ident);

	return o;
}

/**
* loads a shared library
*/
jep_lib jep_load_lib(const char *lib_name)
{
	jep_lib lib = NULL;

#if defined(_WIN32) || defined(__CYGWIN__)
	lib = LoadLibrary(TEXT(lib_name));
#elif defined(__linux__) || defined(__unix__) || defined(__MACH__)
	lib = dlopen(lib_name, RTLD_LAZY);
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
#elif defined(__linux__) || defined(__unix__) || defined(__MACH__)
	dlclose(lib);
#endif
}

/**
* loads a function from a shared library
*/
jep_func jep_get_func(jep_lib lib, const char *func_name)
{
	jep_func func = NULL;

#if defined(_WIN32) || defined(__CYGWIN__)
	func = (jep_func)GetProcAddress(lib, func_name);
#elif defined(__linux__) || defined(__unix__) || defined(__MACH__)
	func = dlsym(lib, func_name);
#endif

	return func;
}

/**
* gets the full path to the executable
*/
char *jep_get_app_path()
{
	errno = 0;
	int app_path_size = 1024;
	char *app_path = malloc(app_path_size);

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
#elif defined(__MACH__) && defined(__APPLE__)

	uint32_t size = (uint32_t)app_path_size;

	int res = _NSGetExecutablePath(app_path, &size);

	if (res)
	{
		app_path = realloc(app_path, size);
		if (app_path == NULL)
		{
			return NULL;
		}
		_NSGetExecutablePath(app_path, &size);
	}
#endif

	return app_path;
}
