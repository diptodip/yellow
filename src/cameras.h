struct Camera {
	Point3D ray_origin;
	Point3D sensor_origin;
	Vec3D normal;
	Vec3D up;
	u32 height;
	u32 width;
};

Ray prime_ray(Camera *camera, u32 row, u32 col) {
	f32 row_frac = (f32) row / (f32) camera->height;
	f32 col_frac = (f32) row / (f32) camera->width;
	Vec3D basis1 = cross(&camera->normal, &camera->up);
	return Ray {};
}
