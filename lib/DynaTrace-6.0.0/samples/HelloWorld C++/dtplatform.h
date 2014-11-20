#ifndef __DTPLATFORM_H__
#define __DTPLATFORM_H__

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

/** Platform specific defines */
#ifdef _WIN32
  #define dt_proc_t void
  #define dt_thread HANDLE
  #define dt_thread_create(thread, proc, data) \
	thread = (HANDLE)_beginthread(proc, 0, (void*)data)
  #define dt_thread_exit(rc) _endthread()
  #define dt_thread_join(thread) \
	WaitForSingleObject(thread, INFINITE)
#else
  #define dt_proc_t void*
  #define dt_thread pthread_t
  #define dt_thread_create(thread, proc, data) \
	pthread_create(&thread, NULL, proc, (void*)data)
  #define dt_thread_exit(rc) pthread_exit(rc); return 0
  #define dt_thread_join(thread) pthread_join(thread, NULL);
#endif

#endif // __DTPLATFORM_H__
