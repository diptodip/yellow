#include <math.h>
#include <stdio.h>

#include "types.h"
#include "stretchy_buffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
	f64 x;
	f64 y;
	f64 z;
} Vec3D;


typedef Vec3D Point3D;

f64 dot(Vec3D *a, Vec3D *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vec3D cross(Vec3D *a, Vec3D *b) {
	Vec3D v = {};
	return v;
}

f64 length_squared(Vec3D *a) {
	return dot(a, a);
}

f64 length(Vec3D *a) {
	return sqrt(length_squared(a));
}

Vec3D normalize(Vec3D *a) {
	f64 norm = length(a);
	Vec3D a_normed = {a->x / norm, a->y / norm, a->z / norm};
	return a_normed;
}

typedef struct {
	u32 color;
	f64 scatter_index;
	f64 refractive_index;
} Material;

typedef struct {
	Point3D origin;
	f64 radius;
	Material material;
} Sphere;

typedef struct {
	Point3D origin;
	Vec3D direction;
} Ray;

typedef union {
	Sphere s;
} Traceable;

typedef struct {
	Point3D origin;
	Vec3D normal;
} Intersection;

typedef struct {
	Point3D ray_origin;
	Point3D sensor_origin;
	Vec3D normal;
	Vec3D up;
	u32 height;
	u32 width;
} Camera;

Ray prime_ray(Camera *camera, u32 row, u32 col) {
	f32 row_frac = (f32) row / (f32) height;
	f32 col_frac = (f32) row / (f32) width;
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
			.color=0xFF0000FF,
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
	u32 *image = malloc(sizeof(u32) * 100 * 100);
	u32 *out = image;
	for (i32 i = 0; i < 100; i++) {
		for (i32 j = 0; j < 100; j++) {
			*out++ = (i < 32) ? 0xFF0000FF : 0xFFFF0000;
		}
	}
	stbi_write_bmp("image.bmp", 100, 100, 4, image);
	return 0;
}
