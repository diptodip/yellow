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

ImagePlane create_image_plane(f64 fov, f64 aspect_ratio, u32 pixel_height) {
	ImagePlane image_plane = {};
	f64 fov_radians = fov * (M_PI / 180.0);
	image_plane.fov = fov;
	image_plane.aspect_ratio = aspect_ratio;
	image_plane.height = 2.0 * std::tan(fov_radians / 2.0);
	image_plane.width = aspect_ratio * image_plane.height;
	image_plane.pixel_size = image_plane.height / (f64) pixel_height;
	image_plane.rows = (u32) pixel_height;
	image_plane.cols = (u32) std::round(image_plane.width / image_plane.pixel_size);
	return image_plane;
}

Ray prime_ray(Camera *camera, f64 row_frac, f64 col_frac) {
	Vec3D basis1 = cross(&camera->up, &camera->normal);
	basis1 = normalize(&basis1);
	Vec3D basis2 = cross(&camera->normal, &basis1);
	f64 lens_radius = camera->aperture / 2.0;
	Vec3D random_lens_offset = lens_radius * random_unit_vector();
	random_lens_offset = (basis1 * random_lens_offset.x) + (basis2 * random_lens_offset.y);
	Point3D top_left = (camera->origin
			    - (camera->focal_distance * camera->image_plane.width * basis1 / 2.0)
			    + (camera->focal_distance * camera->image_plane.height * basis2 / 2.0)
			    - (camera->focal_distance * camera->normal));
	Vec3D direction = (top_left
			   + (camera->focal_distance * basis1 * col_frac * camera->image_plane.width)
			   - (camera->focal_distance * basis2 * row_frac * camera->image_plane.height)
			   - camera->origin
			   - random_lens_offset);
	return (Ray) {camera->origin + random_lens_offset, direction};
}

u32* imalloc(size_t rows, size_t cols) {
	return (u32*) malloc(sizeof(u32) * rows * cols);
}
