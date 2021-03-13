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

union Traceable {
	Sphere s;
};
