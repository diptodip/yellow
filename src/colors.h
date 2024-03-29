#ifndef YELLOW_COLORS
#define YELLOW_COLORS
#include <cmath>
#include "types.h"
#include "linalg.h"

struct RGBA {
	f32 r;
	f32 g;
	f32 b;
	f32 a;
};

inline f32 clamp_color_component(f32 c, f32 min, f32 max) {
	if (c < min) {
		return min;
	}
	if (c > max) {
		return max;
	}
	return c;
}

inline RGBA max_normalize(RGBA *a) {
	f32 max = 0.0;
	if (a->r > max) {
		max = a->r;
	}
	if (a->g > max) {
		max = a->g;
	}
	if (a->b > max) {
		max = a->b;
	}
	return (RGBA) {a->r / max, a->g / max, a->b / max, a->a};
}

inline f32 linear_to_srgb(f32 l) {
	l = clamp_color_component(l, 0.0, 1.0);
	f32 s = l * 12.92;
	if (l > 0.0031308) {
		s = 1.055 * powf(l, 1.0 / 2.4) - 0.055;
	}
	return s;
}

inline RGBA linear_to_srgb(RGBA *linear) {
	RGBA srgb = {};
	srgb.r = linear_to_srgb(linear->r);
	srgb.g = linear_to_srgb(linear->g);
	srgb.b = linear_to_srgb(linear->b);
	srgb.a = linear->a;
	return srgb;
}

// binary rgba ops
inline RGBA operator*(const RGBA& a, const RGBA& b) {
	return (RGBA) {a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a};
}

inline RGBA operator*(const f32 b, const RGBA& a) {
	return (RGBA) {a.r * b, a.g * b, a.b * b, a.a * b};
}

inline RGBA operator*(const RGBA& a, const f32 b) {
	return (RGBA) {a.r * b, a.g * b, a.b * b, a.a * b};
}

inline RGBA operator/(const RGBA& a, const RGBA& b) {
	return (RGBA) {a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a};
}

inline RGBA operator/(const RGBA& a, const f32 b) {
	return (RGBA) {a.r / b, a.g / b, a.b / b, a.a / b};
}

inline RGBA operator+(const RGBA& a, const RGBA& b) {
	return (RGBA) {a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a};
}

inline RGBA operator+(const f32 b, const RGBA& a) {
	return (RGBA) {a.r + b, a.g + b, a.b + b, a.a + b};
}

inline RGBA operator+(const RGBA& a, const f32 b) {
	return (RGBA) {a.r + b, a.g + b, a.b + b, a.a + b};
}

// in-place binary rgba ops
inline RGBA& operator*=(RGBA& a, const RGBA& b) {
	RGBA c = {};
	a.r *= b.r;
	a.g *= b.g;
	a.b *= b.b;
	a.a *= b.a;
	return a;
}

inline RGBA& operator+=(RGBA& a, const RGBA& b) {
	a.r += b.r;
	a.g += b.g;
	a.b += b.b;
	a.a += b.a;
	return a;
}

inline RGBA random_opaque_color(PRNGState *prng_state) {
	f32 r = unit_uniform(prng_state);
	f32 g = unit_uniform(prng_state);
	f32 b = unit_uniform(prng_state);
	return (RGBA) {r, g, b, 1.0};
}

inline RGBA random_opaque_color(PRNGState *prng_state, f32 min, f32 max) {
	f32 r = uniform(prng_state, min, max);
	f32 g = uniform(prng_state, min, max);
	f32 b = uniform(prng_state, min, max);
	return (RGBA) {r, g, b, 1.0};
}

inline u32 rgba_to_u32(RGBA *color) {
	f32 cap = 256;
	RGBA srgb = linear_to_srgb(color);
	u32 r = (u8) (clamp_color_component(srgb.r, 0.0, 0.999) * cap);
	u32 g = (u8) (clamp_color_component(srgb.g, 0.0, 0.999) * cap);
	u32 b = (u8) (clamp_color_component(srgb.b, 0.0, 0.999) * cap);
	u32 a = (u8) (clamp_color_component(srgb.a, 0.0, 0.999) * cap);
	u32 hex = 0;
	hex |= (a << 24);
	hex |= (b << 16);
	hex |= (g << 8);
	hex |= (r << 0);
	return hex;
}
#endif //YELLOW_COLORS
