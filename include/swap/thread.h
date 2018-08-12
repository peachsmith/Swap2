#ifndef JEP_THREAD_H
#define JEP_THREAD_H

#include "swap/object.h"

#if defined(_WIN32)
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <process.h>

#define JEP_THREAD_PROC WINAPI

typedef struct DuhThread {
	uintptr_t thread_ptr;
	void* proc;
	void* args;
	int started;
} jep_thread;

typedef HANDLE jep_mutex;
typedef DWORD jep_mutex_result;

#endif // _WIN32

#if defined (__linux__) || defined(__CYGWIN__)

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define JEP_THREAD_PROC

typedef struct DuhThread {
	pthread_t thread_ptr;
	void* proc;
	void* args;
	int started;
} jep_thread;

typedef pthread_mutex_t* jep_mutex;
typedef int jep_mutex_result;

#endif

typedef struct ThreadArguments {
	struct Object* proc;
	struct Object* args;
	struct Object* list;
} jep_thread_args;

/**
* creates a new thread
*/
jep_thread jep_thread_create(void* proc, void* args);

/**
* starts a thread
*/
void jep_thread_start(jep_thread* t);

/**
* creates a mutex
*/
jep_mutex jep_mutex_create();

/**
* destroys a mutex
*/
void jep_mutex_destroy(jep_mutex m);

/**
* attempts to obtain a mutex lock
*/
jep_mutex_result jep_mutex_lock(jep_mutex m);

/**
* releases a mutex lock
*/
int jep_mutex_release(jep_mutex m);

void jep_thread_sleep(int ms);

#endif // !JEP_THREAD_H
