#include <math.h>
#include <stdio.h>

#include "types.h"

struct Vec3D {
	f64 x;
	f64 y;
	f64 z;
};

using Point3D = Vec3D;
using RGB = Vec3D;

f64 dot(Vec3D *a, Vec3D *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

f64 length_squared(Vec3D *a) {
	return dot(a, a);
}

f64 length(Vec3D *a) {
	return sqrt(length_squared(a));
}

Vec3D normalize(Vec3D *a) {
	f64 norm = length(a);
	return Vec3D {a->x / norm, a->y / norm, a->z / norm};
}

struct Material {
	RGB color;
	f64 scatter_index;
	f64 refractive_index;
};

struct Sphere {
	Point3D origin;
	f64 radius;
	Material material;
};

struct Ray {
	Point3D origin;
	Vec3D direction;
};

int main(int argc, char **args) {
	Sphere s = {
		Point3D {0, 0, 0},
		0.5,
		Material {
			.color=RGB {0.7, 0.5, 0.2},
			.refractive_index=1.5,
		},
	};
	printf("m scatter is %.2f\n", s.material.scatter_index);
	printf("m refractive is %.2f\n", s.material.refractive_index);
	Vec3D v = {10, 5, 7.5};
	f64 l = length(&v);
	Vec3D normalized_v = normalize(&v);
	printf("v is %.2f %.2f %.2f\n", v.x, v.y, v.z);
	printf("length is %.2f\n", l);
	printf("normalized v is %.2f %.2f %.2f\n", normalized_v.x, normalized_v.y, normalized_v.z);
	return 0;
}
