struct Ray {
	Point3D origin;
	Vec3D direction;
};

Ray bounce(Ray *ray, Point3D off) {
	return (Ray) {off, ray->direction};
}

Ray reflect(Ray *ray, Vec3D normal) {
	Vec3D direction = ray->direction;
	Vec3D reflected = direction - (2 * dot(&direction, &normal) * normal);
	return (Ray) {ray->origin, reflected};
}

struct Intersections {
	Point3D *origins;
	Vec3D *normals;
};

// TODO(dd): implement struct of arrays to hold intersection information
// TODO(dd): implement ray tracing algorithm
RGBA trace(Ray *ray, Traceable *world, Intersections *intersections) {
	return (RGBA) {};
}
