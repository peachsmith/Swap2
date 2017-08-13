/*
    Opens a file for tokenization
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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__) || defined(__MACH__)
/* TODO: implement mac functionality */
#endif

#include "import.h"
#include "native.h"

/**
 * gets the path of an import
 */
char *jep_get_import(const char *path)
{
	errno = 0;
	char app_path[1024];
	char *import_path = NULL;

	if (path == NULL)
	{
		return import_path;
	}

	size_t p_len = strlen(path);

	if (p_len < 1)
	{
		return import_path;
	}

#if defined(_WIN32) || defined(__CYGWIN__)

	/* get the path to the application */
	GetModuleFileName(NULL, app_path, 1024);

	if (errno)
	{
		return import_path;
	}

	/* remove the application name from the path */
	int i = strlen(app_path);
	for (; app_path[i] != '\\' && app_path[i] != '/' && i > 0; i--)
	{
		app_path[i] = '\0';
	}

	size_t a_len = strlen(app_path);

	const int i_len = a_len + p_len + 10;
	import_path = malloc(i_len);
	memset(import_path, '\0', i_len);
	strcpy(import_path, app_path);
	strcat(import_path, "imports\\");
	strcat(import_path, path);

#elif defined(__linux__)

	ssize_t len;

	len = readlink("/proc/self/exe", app_path, sizeof(app_path) - 1);
	if (len == -1)
	{
		return import_path;
	}

	app_path[len] = '\0';

	/* remove the application name from the path */
	int i = strlen(app_path);
	for (; app_path[i] != '/' && i > 0; i--)
	{
		app_path[i] = '\0';
	}

	size_t a_len = strlen(app_path);

	const int i_len = a_len + p_len + 10;
	import_path = malloc(i_len);
	memset(import_path, '\0', i_len);
	strcpy(import_path, app_path);
	strcat(import_path, "imports/");
	strcat(import_path, path);

#elif defined(__APPLE__) || defined(__MACH__)
	/* TODO: implement mac functionality */
	import_path = malloc(p_len);
	memset(import_path, '\0', p_len);
	strcpy(import_path, path);
#endif

	return import_path;
}
