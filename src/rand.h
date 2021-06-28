#ifndef YELLOW_RAND
#define YELLOW_RAND
#include <ctime>
#include "types.h"

static thread_local u64 global_rng_state = (u64) time(NULL);

static inline u64 xor_shift64() {
	u64 x = global_rng_state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	global_rng_state = x;
	return x;
}

f64 unit_uniform() {
	return xor_shift64() / (f64) UINT64_MAX;
}

f64 uniform(f64 start, f64 end) {
	return (unit_uniform() * (end - start)) + start;
}
#endif //YELLOW_RAND
