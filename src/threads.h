#ifndef YELLOW_THREADS
#define YELLOW_THREADS
#include "types.h"
#include "materials.h"
#include "cameras.h"

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
	std::vector<Traceable> world;
	Camera camera;
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

u64 locked_increment(volatile u64 *x, u64 by) {
	// Increment value with a lock and return the previous value
	u64 prev_x = *x;
	*x += by;
	return prev_x;
}
#endif //YELLOW_THREADS
