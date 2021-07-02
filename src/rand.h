#ifndef YELLOW_RAND
#define YELLOW_RAND
#include "types.h"

struct PRNGState {
	u32 entropy;
};

inline u32 xor_shift32(PRNGState *prng_state) {
	u32 x = prng_state->entropy;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	prng_state->entropy = x;
	return x;
}

inline f32 unit_uniform(PRNGState *prng_state) {
	return xor_shift32(prng_state) / (f32) UINT32_MAX;
}

inline f32 uniform(PRNGState *prng_state, f32 start, f32 end) {
	return (unit_uniform(prng_state) * (end - start)) + start;
}
#endif //YELLOW_RAND
