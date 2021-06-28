#ifndef YELLOW_MATERIALS
#define YELLOW_MATERIALS
#include "types.h"
#include "linalg.h"
#include "colors.h"

struct Material {
	RGBA color;
	f64 scatter_index;
	f64 refractive_index;
};

struct Sphere {
	Point3D origin;
	f64 radius;
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
