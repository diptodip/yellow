struct Ray {
	Point3D origin;
	Vec3D direction;
};

struct Intersection {
	Point3D origin;
	Vec3D normal;
};

// TODO(dd): implement struct of arrays to hold intersection information
// TODO(dd): implement ray tracing algorithm
void trace(Ray *r, Traceable *world, Intersection *intersections) {
}
