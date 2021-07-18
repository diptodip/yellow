#ifndef YELLOW_RAND
#define YELLOW_RAND
#include "types.h"
#include <cstdio>

#ifdef _WIN32 //WINDOWS
#include <windows.h>
#include <bcrypt.h>
inline u32 read_entropy() {
	u32 entropy;
	NTSTATUS result = BCryptGenRandom(NULL, (PUCHAR) &entropy, 4, 0);
	if (entropy < 1) {
		entropy += 1;
	}
	return entropy;
}
#else //UNIX
#include <bsd/stdlib.h>
inline u32 read_entropy() {
	u32 entropy;
	arc4random_buf((void *) &entropy, 4);
	if (entropy < 1) {
		entropy += 1;
	}
	return entropy;
}
#endif //_WIN32

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
