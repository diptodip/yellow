#ifndef YELLOW_CAMERAS
#define YELLOW_CAMERAS
#include <cmath>
#include "types.h"
#include "linalg.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

struct ImagePlane {
	f32 fov;
	f32 aspect_ratio;
	f32 pixel_size;
	f32 height;
	f32 width;
	u32 rows;
	u32 cols;
};

struct Camera {
	Point3D origin;
	Vec3D normal;
	Vec3D up;
	ImagePlane image_plane;
	f32 aperture;
	f32 focal_distance;
};

ImagePlane create_image_plane(f32 fov, f32 aspect_ratio, u32 pixel_height) {
	ImagePlane image_plane = {};
	f32 fov_radians = fov * (M_PI / 180.0);
	image_plane.fov = fov;
	image_plane.aspect_ratio = aspect_ratio;
	image_plane.height = 2.0 * std::tan(fov_radians / 2.0);
	image_plane.width = aspect_ratio * image_plane.height;
	image_plane.pixel_size = image_plane.height / (f32) pixel_height;
	image_plane.rows = (u32) pixel_height;
	image_plane.cols = (u32) std::round(image_plane.width / image_plane.pixel_size);
	return image_plane;
}

u32* imalloc(size_t rows, size_t cols) {
	return (u32*) malloc(sizeof(u32) * rows * cols);
}
#endif //YELLOW_CAMERAS
