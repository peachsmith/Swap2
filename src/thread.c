#include "swap/thread.h"

jep_thread jep_thread_create(void* proc, void* args)
{
	jep_thread t;
	t.proc = proc;
	t.args = args;
	t.started = 0;

	return t;
}

void jep_thread_start(jep_thread* t)
{
#if defined(_WIN32)
	t->thread_ptr = _beginthreadex(NULL, 0, t->proc, t->args, NULL, NULL);
#elif defined (__linux__) || defined(__CYGWIN__)
	pthread_create(&(t->thread_ptr), NULL, t->proc, t->args);
#endif

	t->started = 1;
}

jep_mutex jep_mutex_create()
{
	jep_mutex m;

#if defined(_WIN32)
	m = CreateMutex(NULL, FALSE, NULL);
#elif defined(__linux__) || defined(__CYGWIN__)
	m = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m, NULL);
#endif

	return m;
}

void jep_mutex_destroy(jep_mutex m)
{
#if defined(_WIN32)
	CloseHandle(m);
#elif defined(__linux__) || defined(__CYGWIN__)
	pthread_mutex_destroy(m);
	free(m);
#endif
}

jep_mutex_result jep_mutex_lock(jep_mutex m)
{
	jep_mutex_result result;

#if defined(_WIN32)
	result = WaitForSingleObject(m, INFINITE);
#elif defined(__linux__) || defined(__CYGWIN__)
	result = pthread_mutex_lock(m);
#endif

	return result;
}

int jep_mutex_release(jep_mutex m)
{
	int result;

#if defined(_WIN32)
	result = ReleaseMutex(m);
#elif defined(__linux__) || defined(__CYGWIN__)
	result = pthread_mutex_unlock(m);
#endif

	return result;
}

void jep_thread_sleep(int ms)
{
#if defined(_WIN32)
	Sleep(ms);
#elif defined(__linux__) || defined(__CYGWIN__)
	sleep(ms / 1000);
#endif
}
