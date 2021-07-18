#ifndef YELLOW_LINALG
#define YELLOW_LINALG
#include <cmath>
#include <cstdio>
#include "types.h"
#include "rand.h"

inline f32 y_fabs(f32 x) {
	return (x < 0) ? -x : x;
}

struct Vec3D {
	f32 x;
	f32 y;
	f32 z;
};

typedef Vec3D Point3D;

// binary vec ops
inline Vec3D operator+(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vec3D operator*(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline Vec3D operator-(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vec3D operator/(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x / b.x, a.y / b.y, a.z / b.z};
}

// binary vec-scalar ops
inline Vec3D operator+(const Vec3D& a, const f32 b) {
	return (Vec3D) {a.x + b, a.y + b, a.z + b};
}

inline Vec3D operator*(const Vec3D& a, const f32 b) {
	return (Vec3D) {a.x * b, a.y * b, a.z * b};
}

inline Vec3D operator-(const Vec3D& a, const f32 b) {
	return (Vec3D) {a.x - b, a.y - b, a.z - b};
}

inline Vec3D operator/(const Vec3D& a, const f32 b) {
	return (Vec3D) {a.x / b, a.y / b, a.z / b};
}

// binary scalar-vec ops
inline Vec3D operator+(const f32 b, const Vec3D& a) {
	return (Vec3D) {a.x + b, a.y + b, a.z + b};
}

inline Vec3D operator*(const f32 b, const Vec3D& a) {
	return (Vec3D) {a.x * b, a.y * b, a.z * b};
}

inline Vec3D operator-(const f32 b, const Vec3D& a) {
	return (Vec3D) {a.x - b, a.y - b, a.z - b};
}

inline Vec3D operator/(const f32 b, const Vec3D& a) {
	return (Vec3D) {a.x / b, a.y / b, a.z / b};
}

// unary vec ops
inline Vec3D operator-(const Vec3D& a) {
	return (Vec3D) {-a.x, -a.y, -a.z};
}

inline f32 dot(Vec3D *a, Vec3D *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

inline Vec3D cross(Vec3D *a, Vec3D *b) {
	Vec3D v = {
		a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x
	};
	return v;
}

inline f32 l2_norm_squared(Vec3D *a) {
	return dot(a, a);
}

inline f32 l2_norm(Vec3D *a) {
	return sqrt(l2_norm_squared(a));
}

inline Vec3D normalize(Vec3D *a) {
	f32 norm = l2_norm(a);
	Vec3D a_normed = {a->x / norm, a->y / norm, a->z / norm};
	return a_normed;
}

inline Vec3D random_direction_in_ranges(PRNGState *prng_state, f32 x1, f32 x2, f32 y1, f32 y2, f32 z1, f32 z2) {
	return (Vec3D) {uniform(prng_state, x1, x2), uniform(prng_state, y1, y2), uniform(prng_state, z1, z2)};
}

inline Vec3D random_unit_vector(PRNGState *prng_state) {
	Vec3D direction = random_direction_in_ranges(prng_state, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	direction = normalize(&direction);
	return direction;
}

inline Vec3D random_unit_sphere_vector(PRNGState *prng_state) {
	f32 l2_squared = 2.0;
	Vec3D direction;
	while (l2_squared > 1.0) {
		direction = random_direction_in_ranges(prng_state, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		l2_squared = l2_norm_squared(&direction);
	}
	return direction;
}

inline Vec3D random_unit_disk_vector(PRNGState *prng_state) {
	f32 l2_squared = 2.0;
	Vec3D direction;
	while (l2_squared > 1.0) {
		direction = random_direction_in_ranges(prng_state, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		l2_squared = l2_norm_squared(&direction);
	}
	return direction;
}

inline void print_vec(Vec3D *a) {
	printf("(x: %f, y: %f, z: %f)", a->x, a->y, a->z);
}
#endif //YELLOW_LINALG
