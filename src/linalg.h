#ifndef YELLOW_LINALG
#define YELLOW_LINALG
#include <cmath>
#include <cstdio>
#include <random>
#include "types.h"

struct Vec3D {
	f64 x;
	f64 y;
	f64 z;
};

using Point3D = Vec3D;

// binary vec ops
Vec3D operator+(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3D operator*(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x * b.x, a.y * b.y, a.z * b.z};
}

Vec3D operator-(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3D operator/(const Vec3D& a, const Vec3D& b) {
	return (Vec3D) {a.x / b.x, a.y / b.y, a.z / b.z};
}

// binary vec-scalar ops
Vec3D operator+(const Vec3D& a, const f64 b) {
	return (Vec3D) {a.x + b, a.y + b, a.z + b};
}

Vec3D operator*(const Vec3D& a, const f64 b) {
	return (Vec3D) {a.x * b, a.y * b, a.z * b};
}

Vec3D operator-(const Vec3D& a, const f64 b) {
	return (Vec3D) {a.x - b, a.y - b, a.z - b};
}

Vec3D operator/(const Vec3D& a, const f64 b) {
	return (Vec3D) {a.x / b, a.y / b, a.z / b};
}

// binary scalar-vec ops
Vec3D operator+(const f64 b, const Vec3D& a) {
	return (Vec3D) {a.x + b, a.y + b, a.z + b};
}

Vec3D operator*(const f64 b, const Vec3D& a) {
	return (Vec3D) {a.x * b, a.y * b, a.z * b};
}

Vec3D operator-(const f64 b, const Vec3D& a) {
	return (Vec3D) {a.x - b, a.y - b, a.z - b};
}

Vec3D operator/(const f64 b, const Vec3D& a) {
	return (Vec3D) {a.x / b, a.y / b, a.z / b};
}

// unary vec ops
Vec3D operator-(const Vec3D& a) {
	return (Vec3D) {-a.x, -a.y, -a.z};
}

f64 dot(Vec3D *a, Vec3D *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vec3D cross(Vec3D *a, Vec3D *b) {
	Vec3D v = {
		a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x
	};
	return v;
}

f64 l2_norm_squared(Vec3D *a) {
	return dot(a, a);
}

f64 l2_norm(Vec3D *a) {
	return std::sqrt(l2_norm_squared(a));
}

Vec3D normalize(Vec3D *a) {
	f64 norm = l2_norm(a);
	Vec3D a_normed = {a->x / norm, a->y / norm, a->z / norm};
	return a_normed;
}

Vec3D random_direction_in_ranges(f64 x1, f64 x2, f64 y1, f64 y2, f64 z1, f64 z2) {
	static thread_local std::mt19937 generator;
	std::uniform_real_distribution<> x_distribution(x1, x2);
	std::uniform_real_distribution<> y_distribution(y1, y2);
	std::uniform_real_distribution<> z_distribution(z1, z2);
	
	return (Vec3D) {x_distribution(generator), y_distribution(generator), z_distribution(generator)};
}

Vec3D random_unit_vector() {
	f64 l2_squared = 2.0;
	Vec3D direction;
	while (l2_squared > 1.0) {
		direction = random_direction_in_ranges(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		l2_squared = l2_norm_squared(&direction);
	}
	direction = normalize(&direction);
	return direction;
}

Vec3D random_unit_sphere_vector() {
	f64 l2_squared = 2.0;
	Vec3D direction;
	while (l2_squared > 1.0) {
		direction = random_direction_in_ranges(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		l2_squared = l2_norm_squared(&direction);
	}
	return direction;
}

Vec3D random_unit_disk_vector() {
	f64 l2_squared = 2.0;
	Vec3D direction;
	while (l2_squared > 1.0) {
		direction = random_direction_in_ranges(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		l2_squared = l2_norm_squared(&direction);
	}
	return direction;
}

void print_vec(Vec3D *a) {
	printf("(x: %f, y: %f, z: %f)", a->x, a->y, a->z);
}
#endif //YELLOW_LINALG
