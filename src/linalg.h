struct Vec3D {
	f64 x;
	f64 y;
	f64 z;
};

using Point3D = Vec3D;

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
