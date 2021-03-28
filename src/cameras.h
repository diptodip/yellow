struct ImagePlane {
	f64 fov;
	f64 aspect_ratio;
	f64 pixel_size;
	f64 height;
	f64 width;
	u32 rows;
	u32 cols;
};

struct Camera {
	Point3D origin;
	Vec3D normal;
	Vec3D up;
	ImagePlane image_plane;
	f64 aperture;
	f64 focal_distance;
};

Ray prime_ray(Camera *camera, u32 row, u32 col) {
	f32 row_frac = (f32) row / (f32) camera->height;
	f32 col_frac = (f32) row / (f32) camera->width;
	Vec3D basis1 = cross(&camera->normal, &camera->up);
	return Ray {};
}

u32* imalloc(size_t height, size_t width) {
	return (u32*) malloc(sizeof(u32) * height * width);
}
