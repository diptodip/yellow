#ifndef YELLOW_RAND
#define YELLOW_RAND
#include <ctime>
#include "types.h"

static thread_local u32 global_rng_state = (u32) time(NULL);

static inline u32 xor_shift32() {
	u32 x = global_rng_state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	global_rng_state = x;
	return x;
}

f32 unit_uniform() {
	return xor_shift32() / (f32) UINT32_MAX;
}

f32 uniform(f32 start, f32 end) {
	return (unit_uniform() * (end - start)) + start;
}
#endif //YELLOW_RAND
