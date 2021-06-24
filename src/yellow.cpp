#include <cmath>
#include <cstdio>
#include <ctime>
#include <random>
#include <time.h>
#include "types.h"
#include "linalg.h"
#include "colors.h"
#include "materials.h"
#include "cameras.h"
#include "ray.h"
// TODO(dd): implement include guards in the headers
// and include them in each other?

void test_spheres() {
	RGBA dark_blue = {0.1, 0.2, 0.7, 1.0};
	RGBA dark_red = {0.7, 0.2, 0.1, 1.0};
	RGBA grass_green = {0.8, 0.8, 0.0, 1.0};
	RGBA white = {1.0, 1.0, 1.0, 1.0};
	Material m1 = {
		.color = grass_green,
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m2 = {
		.color = dark_blue,
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m3 = {
		.color = dark_red,
		.scatter_index = 0.3,
		.refractive_index = 0.0,
	};
	Material m4 = {
		.color = white,
		.scatter_index = 0.0,
		.refractive_index = 1.5,
	};
	Material m5 = {
		.color = white,
		.scatter_index = 0.0,
		.refractive_index = 1.5,
	};
	Sphere s1 = {
		.origin = (Point3D) {0.0, -100.5, -1.0},
		.radius = 100.0,
	};
	Sphere s2 = {
		.origin = (Point3D) {0.0, 0.0, -1.0},
		.radius = 0.5,
	};
	Sphere s3 = {
		.origin = (Point3D) {1.0, 0.0, -1.0},
		.radius = 0.5,
	};
	Sphere s4 = {
		.origin = (Point3D) {-1.0, 0.0, -1.0},
		.radius = 0.5,
	};
	Sphere s5 = {
		.origin = (Point3D) {-1.0, 0.0, -1.0},
		.radius = -0.45,
	};
	Traceable ground_sphere = {SphereT, s1, m1};
	Traceable red_sphere = {SphereT, s2, m2};
	Traceable metal_sphere = {SphereT, s3, m3};
	Traceable glass_sphere = {SphereT, s4, m4};
	Traceable glass_sphere_inside = {SphereT, s5, m5};
	std::vector<Traceable> world = {};
	world.push_back(ground_sphere);
	world.push_back(red_sphere);
	world.push_back(metal_sphere);
	world.push_back(glass_sphere);
	world.push_back(glass_sphere_inside);
	f64 fov = 20.0;
	f64 aperture = 0.1;
	f64 aspect_ratio = 16.0 / 9.0;
	u32 pixel_height = 216;
	ImagePlane image_plane = create_image_plane(fov, aspect_ratio, pixel_height);
	Point3D origin = {-2.0, 2.0, 1.0};
	Point3D target = {0.0, 0.0, -1.0};
	Vec3D normal = origin - target;
	f64 focal_distance = l2_norm(&normal);
	normal = normalize(&normal);
	Vec3D up = {0.0, 1.0, 0.0};
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
	render(world, camera, image_plane.rows, image_plane.cols, 100);
}

void random_spheres() {
	f64 fov = 20.0;
	f64 aperture = 0.1;
	f64 aspect_ratio = 1.5;
	u32 pixel_height = 300;
	ImagePlane image_plane = create_image_plane(fov, aspect_ratio, pixel_height);
	Point3D origin = {13.0, 2.0, 3.0};
	Point3D target = {0.0, 0.0, 0.0};
	Vec3D normal = origin - target;
	f64 focal_distance = 10.0;
	normal = normalize(&normal);
	Vec3D up = {0.0, 1.0, 0.0};
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
	std::vector<Traceable> world = {};
	Material ground_material = {
		.color = (RGBA) {0.5, 0.5, 0.5, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere ground_sphere = {
		.origin = (Point3D) {0.0, -1000.0, 0.0},
		.radius = 1000.0,
	};
	Traceable ground = {SphereT, ground_sphere, ground_material};
	world.push_back(ground);
	static thread_local std::mt19937 generator;
	std::uniform_real_distribution<> unit_uniform_distribution(0.0, 1.0);
	for (i32 i = -11; i < 11; i++) {
		for (i32 j = -11; j < 11; j++) {
			f64 material_check = unit_uniform_distribution(generator);
			f64 x = (f64) i + (0.9 * unit_uniform_distribution(generator));
			f64 y = 0.2;
			f64 z = (f64) j + (0.9 * unit_uniform_distribution(generator));
			Point3D position = {x, y, z};
			Point3D target = {4.0, 0.2, 0.0};
			Point3D distance = position - target;
			if (l2_norm(&distance) > 0.9) {
				Material material;
				Sphere sphere;
				Traceable traceable;
				if (material_check < 0.8) {
					// make diffuse sphere
					RGBA random_color = random_opaque_color();
					material = {
					.color = random_color,
					.scatter_index = 1.0,
					.refractive_index = 0.0,
					};
					sphere = {
					.origin = position,
					.radius = y,
					};
				} else if (material_check < 0.95) {
					// make fuzzy reflective sphere
					RGBA random_color = random_opaque_color(0.5, 1.0);
					f64 scatter_index = unit_uniform_distribution(generator);
					material = {
					.color = random_color,
					.scatter_index = scatter_index,
					.refractive_index = 0.0,
					};
					sphere = {
					.origin = position,
					.radius = y,
					};
				} else {
					// make refractive sphere
					material = {
					.color = (RGBA) {1.0, 1.0, 1.0, 1.0},
					.scatter_index = 0.0,
					.refractive_index = 1.5,
					};
					sphere = {
					.origin = position,
					.radius = y,
					};
				}
				traceable = {SphereT, sphere, material};
				world.push_back(traceable);
			}
		}
	}
	Material big_glass_material = {
		.color = (RGBA) {1.0, 1.0, 1.0, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 1.5,
	};
	Sphere big_glass_sphere {
		.origin = (Point3D) {0.0, 1.0, 0.0},
		.radius = 1.0,
	};
	Traceable big_glass_traceable = {SphereT, big_glass_sphere, big_glass_material};
	Material big_diffuse_material = {
		.color = (RGBA) {0.4, 0.2, 0.1, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere big_diffuse_sphere {
		.origin = (Point3D) {-4.0, 1.0, 0.0},
		.radius = 1.0,
	};
	Traceable big_diffuse_traceable = {SphereT, big_diffuse_sphere, big_diffuse_material};
	Material big_reflective_material = {
		.color = (RGBA) {0.7, 0.6, 0.5, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 0.0,
	};
	Sphere big_reflective_sphere {
		.origin = (Point3D) {4.0, 1.0, 0.0},
		.radius = 1.0,
	};
	Traceable big_reflective_traceable = {SphereT, big_reflective_sphere, big_reflective_material};
	world.push_back(big_glass_traceable);
	world.push_back(big_diffuse_traceable);
	world.push_back(big_reflective_traceable);
	render(world, camera, image_plane.rows, image_plane.cols, 100);
}

int main(int argc, char **args) {
	time_t start = time(NULL);
	clock_t sc = clock();
	random_spheres();
	clock_t ec = clock();
	time_t stop = time(NULL);
	f64 duration = (f64) (stop - start);
	f64 dc = (f64) (ec - sc) / (f64) CLOCKS_PER_SEC;
	printf("[info] scene rendered in %.2f seconds\n", duration);
	printf("[info] scene rendered in %.2f seconds\n", dc);
	return 0;
}
