#ifndef YELLOW_THREADS
#define YELLOW_THREADS
#include "types.h"
#include "materials.h"
#include "cameras.h"

#ifdef _WIN32 // WINDOWS
#include <windows.h>

inline u32 core_count() {
	_SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	return sys_info.dwNumberOfProcessors;
}

typedef HANDLE ThreadHandle;
#define threaded DWORD WINAPI

inline ThreadHandle create_thread(LPTHREAD_START_ROUTINE function, void *args) {
	DWORD thread_id;
	ThreadHandle thread = CreateThread(0, 0, function, args, 0, &thread_id);
	return thread;
}

inline void join_thread(ThreadHandle thread) {
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}

inline f64 tick() {
	LARGE_INTEGER current_ticks;
	LARGE_INTEGER tick_frequency;
	BOOL res = QueryPerformanceCounter(&current_ticks);
	if (!res) {
		return 0;
	}
	res = QueryPerformanceFrequency(&tick_frequency);
	return (f64) current_ticks.QuadPart / (f64) tick_frequency.QuadPart;
}
#else // UNIX
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <ctime>

inline u32 core_count() {
	return sysconf(_SC_NPROCESSORS_ONLN);
}

typedef pthread_t ThreadHandle;
typedef void * threaded;

inline ThreadHandle create_thread(void *(*function)(void*), void *args) {
	ThreadHandle thread;
	i32 result = pthread_create(&thread, NULL, function, args);
	if (result == 0) {
		return thread;
	} else {
		return NULL;
	}
}

inline void join_thread(ThreadHandle thread) {
	pthread_join(thread, NULL);
}

inline f64 tick() {
	struct timespec ts;
	u32 res = clock_gettime(CLOCK_MONOTONIC, &ts);
	if (res == -1) {
		return 0;
	}
	return ((f64) ((ts.tv_sec * 1e9) + ts.tv_nsec)) / (f64) 1.0e9;
}
#endif //_WIN32

u64 locked_increment(volatile u64 *x, u64 by) {
	// increment value with a lock and return the previous value
	// NOTE(dd): we're using a gcc/clang compiler extension to do this
	// because mutexes and InterlockedExchangeAdd were for some reason
	// slower
	return __sync_fetch_and_add(x, by);
}

struct RenderJob {
	World *world;
	Camera *camera;
	u32 rows;
	u32 cols;
	u32 row_min;
	u32 row_max;
	u32 col_min;
	u32 col_max;
	u32 num_samples;
	u32 *out;
};

struct RenderQueue {
	u32 num_tiles;
	RenderJob *jobs;
	volatile u64 next_job_index;
	volatile u64 tile_rendered_count;
	volatile u64 ray_count;
};
#endif //YELLOW_THREADS
