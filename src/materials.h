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
	Material material;
};
#endif //YELLOW_MATERIALS
