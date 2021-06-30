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

f32 clamp_color_component(f32 c, f32 min, f32 max) {
	if (c < min) {
		return min;
	}
	if (c > max) {
		return max;
	}
	return c;
}

RGBA max_normalize(RGBA *a) {
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

// binary rgba ops
RGBA operator*(const RGBA& a, const RGBA& b) {
	return (RGBA) {a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a};
}

RGBA operator*(const f32 b, const RGBA& a) {
	return (RGBA) {a.r * b, a.g * b, a.b * b, a.a * b};
}

RGBA operator*(const RGBA& a, const f32 b) {
	return (RGBA) {a.r * b, a.g * b, a.b * b, a.a * b};
}

RGBA operator/(const RGBA& a, const RGBA& b) {
	return (RGBA) {a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a};
}

RGBA operator/(const RGBA& a, const f32 b) {
	return (RGBA) {a.r / b, a.g / b, a.b / b, a.a / b};
}

RGBA operator+(const RGBA& a, const RGBA& b) {
	return (RGBA) {a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a};
}

RGBA operator+(const f32 b, const RGBA& a) {
	return (RGBA) {a.r + b, a.g + b, a.b + b, a.a + b};
}

RGBA operator+(const RGBA& a, const f32 b) {
	return (RGBA) {a.r + b, a.g + b, a.b + b, a.a + b};
}

// in-place binary rgba ops
RGBA& operator*=(RGBA& a, const RGBA& b) {
	RGBA c = {};
	a.r *= b.r;
	a.g *= b.g;
	a.b *= b.b;
	a.a *= b.a;
	return a;
}

RGBA& operator+=(RGBA& a, const RGBA& b) {
	a.r += b.r;
	a.g += b.g;
	a.b += b.b;
	a.a += b.a;
	return a;
}

RGBA random_opaque_color() {
	f32 r = unit_uniform();
	f32 g = unit_uniform();
	f32 b = unit_uniform();
	return (RGBA) {r, g, b, 1.0};
}

RGBA random_opaque_color(f32 min, f32 max) {
	f32 r = uniform(min, max);
	f32 g = uniform(min, max);
	f32 b = uniform(min, max);
	return (RGBA) {r, g, b, 1.0};
}

u32 rgba_to_u32(RGBA *color) {
	f32 cap = 256;
	u32 r = (u8) (clamp_color_component(std::sqrt(color->r), 0.0, 0.999) * cap);
	u32 g = (u8) (clamp_color_component(std::sqrt(color->g), 0.0, 0.999) * cap);
	u32 b = (u8) (clamp_color_component(std::sqrt(color->b), 0.0, 0.999) * cap);
	u32 a = (u8) (clamp_color_component(std::sqrt(color->a), 0.0, 0.999) * cap);
	u32 hex = 0;
	hex |= (a << 24);
	hex |= (b << 16);
	hex |= (g << 8);
	hex |= (r << 0);
	return hex;
}
#endif //YELLOW_COLORS
