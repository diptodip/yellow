#include <cmath>
#include <cstdio>

#include "types.h"
#include "stretchy_buffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Vec3D {
	f64 x;
	f64 y;
	f64 z;
};

using Point3D = Vec3D;

struct RGBA {
	f64 r;
	f64 g;
	f64 b;
	f64 a;
};

f64 dot(Vec3D *a, Vec3D *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vec3D cross(Vec3D *a, Vec3D *b) {
	Vec3D v = {
		a->y * b->z - a->z * b->z,
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

struct Material {
	RGBA color;
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

union Traceable {
	Sphere s;
};

struct Intersection {
	Point3D origin;
	Vec3D normal;
};

struct Camera {
	Point3D ray_origin;
	Point3D sensor_origin;
	Vec3D normal;
	Vec3D up;
	u32 height;
	u32 width;
};

Ray prime_ray(Camera *camera, u32 row, u32 col) {
	f32 row_frac = (f32) row / (f32) camera->height;
	f32 col_frac = (f32) row / (f32) camera->width;
	Vec3D basis1 = cross(&camera->normal, &camera->up);
	return Ray {};
}

// TODO(dip): implement struct of arrays to hold intersection information
// TODO(dip): implement ray tracing algorithm
void trace(Ray *r, Traceable *world, Intersection *intersections) {
}

int main(int argc, char **args) {
	Sphere s = {
		.origin = (Point3D){0, 0, 0},
		.radius = 0.5,
		.material = (Material){
			.color=(RGBA){1, 0, 0, 1},
			.refractive_index=1.5,
		},
	};
	std::printf("m scatter is %.2f\n", s.material.scatter_index);
	std::printf("m refractive is %.2f\n", s.material.refractive_index);
	Vec3D v = {10, 5, 7.5};
	f64 l = l2_norm(&v);
	Vec3D normalized_v = normalize(&v);
	std::printf("v is %.2f %.2f %.2f\n", v.x, v.y, v.z);
	std::printf("l2_norm is %.2f\n", l);
	std::printf("normalized v is %.2f %.2f %.2f\n", normalized_v.x, normalized_v.y, normalized_v.z);
	u32 *image = (u32*) malloc(sizeof(u32) * 100 * 100);
	u32 *out = image;
	for (u32 i = 0; i < 100; i++) {
		for (u32 j = 0; j < 100; j++) {
			*out++ = (i < 32) ? 0xFF0000FF : 0xFFFF0000;
		}
	}
	stbi_write_bmp("image.bmp", 100, 100, 4, image);
	return 0;
}
