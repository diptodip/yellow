#ifndef YELLOW_MATERIALS
#define YELLOW_MATERIALS
#include "types.h"
#include "linalg.h"
#include "colors.h"
//TODO(dd): rearrange these structs for SIMD

struct Material {
	RGBA color;
	f32 scatter_index;
	f32 refractive_index;
};

struct Sphere {
	Point3D origin;
	f32 radius;
};

union TraceableObject {
	Sphere s;
};

enum TraceableType {
	SphereT
};

struct Traceable {
	TraceableType type;
	TraceableObject object;
	u32 material_index;
};

struct World {
	u32 num_materials;
	u32 num_traceables;
	Material *materials;
	Traceable *traceables;
};
#endif //YELLOW_MATERIALS
