#ifndef YELLOW_THREADS
#define YELLOW_THREADS
#include "types.h"

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#else
#include <pthread.h>

typedef pthread_t ThreadHandle;
/*
inline ThreadHandle create_thread() {
	ThreadHandle thread;
	i32 result = pthread_create(&thread);
	if (result == 0) {
		return thread;
	} else {
		return NULL;
	}
}
*/

inline void join_thread(ThreadHandle thread) {
}
#endif //_WIN32

struct RenderJob {
	u32 row_min;
	u32 row_max;
	u32 col_min;
	u32 col_max;
};

struct RenderQueue {
	u32 num_tiles;
	RenderJob *jobs;
	volatile u32 next_job_index;
	volatile u32 tile_rendered_count;
	volatile u64 ray_count;
};
#endif //YELLOW_THREADS
