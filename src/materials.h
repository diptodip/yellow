#ifndef YELLOW_MATERIALS
#define YELLOW_MATERIALS
#include "types.h"
#include "linalg.h"
#include "colors.h"
//TODO(dd): rearrange these structs for SIMD

struct Material {
	RGBA color;
	RGBA emit;
	f32 scatter_index;
	f32 refractive_index;
};

struct Sphere {
	Point3D origin;
	f32 radius;
	u32 material_index;
};

struct World {
	u32 num_materials;
	u32 num_spheres;
	Material *materials;
	Sphere *spheres;
};
#endif //YELLOW_MATERIALS
