#include <cmath>
#include <cstdio>
#include "stretchy_buffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "types.h"
#include "linalg.h"
#include "colors.h"
#include "materials.h"
#include "ray.h"
#include "cameras.h"
// TODO(dd): Implement include guards in the headers
// and include them in each other?

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
	u32 *image = imalloc(100, 100);
	u32 *out = image;
	for (u32 i = 0; i < 100; i++) {
		for (u32 j = 0; j < 100; j++) {
			*out++ = (i < 32) ? 0xFF0000FF : 0xFFFF0000;
		}
	}
	stbi_write_bmp("image.bmp", 100, 100, 4, image);
	return 0;
}
