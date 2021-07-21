#include <cmath>
#include <cstdio>
#include "types.h"
#include "linalg.h"
#include "colors.h"
#include "materials.h"
#include "cameras.h"
#include "ray.h"
#include "threads.h"
#include "rand.h"

inline f32 test_spheres(u32 num_threads) {
	f32 fov = 20.0;
	f32 aperture = 0.1;
	f32 aspect_ratio = 16.0 / 9.0;
	u32 pixel_height = 216;
	ImagePlane image_plane = create_image_plane(fov, aspect_ratio, pixel_height);
	Point3D origin = {-2.0, 2.0, 1.0};
	Point3D target = {0.0, 0.0, -1.0};
	Vec3D normal = origin - target;
	f32 focal_distance = l2_norm(&normal);
	normal = normalize(&normal);
	Vec3D up = {0.0, 1.0, 0.0};
	RGBA background = {0.5, 0.7, 1.0, 1.0};
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
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
		.material_index = 0
	};
	Sphere s2 = {
		.origin = (Point3D) {0.0, 0.0, -1.0},
		.radius = 0.5,
		.material_index = 1
	};
	Sphere s3 = {
		.origin = (Point3D) {1.0, 0.0, -1.0},
		.radius = 0.5,
		.material_index = 2
	};
	Sphere s4 = {
		.origin = (Point3D) {-1.0, 0.0, -1.0},
		.radius = 0.5,
		.material_index = 3
	};
	Sphere s5 = {
		.origin = (Point3D) {-1.0, 0.0, -1.0},
		.radius = -0.45,
		.material_index = 4
	};
	Material materials[5] = {m1, m2, m3, m4, m5};
	Sphere spheres[5] = {
		s1,
		s2,
		s3,
		s4,
		s5
	};
	World world = {};
	world.num_materials = 5;
	world.num_spheres = 5;
	world.materials = materials;
	world.spheres = spheres;
	printf("[info] total spheres: %d\n", world.num_spheres);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		&background,
		image_plane.rows,
		image_plane.cols,
		32,
		32,
		100,
		50,
		num_threads
	);
	return ray_count;
}

inline f32 random_spheres(u32 num_threads) {
	PRNGState prng_state = {read_entropy()};
	f32 fov = 20.0;
	f32 aperture = 0.1;
	f32 aspect_ratio = (16.0 / 9.0);
	u32 pixel_height = 216;
	ImagePlane image_plane = create_image_plane(fov, aspect_ratio, pixel_height);
	Point3D origin = {13.0, 2.0, 3.0};
	Point3D target = {0.0, 0.0, 0.0};
	Vec3D normal = origin - target;
	f32 focal_distance = 10.0;
	normal = normalize(&normal);
	Vec3D up = {0.0, 1.0, 0.0};
	RGBA background = {0.5, 0.7, 1.0, 1.0};
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
	Material materials[488];
	Sphere spheres[488];
	World world = {};
	world.num_materials = 0;
	world.num_spheres = 0;
	world.materials = materials;
	world.spheres = spheres;
	static Material ground_material = {
		.color = (RGBA) {0.5, 0.5, 0.5, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0
	};
	Sphere ground_sphere = {
		.origin = (Point3D) {0.0, -1000.0, 0.0},
		.radius = 1000.0,
		.material_index = world.num_materials
	};
	world.materials[world.num_materials] = ground_material;
	world.spheres[world.num_spheres] = ground_sphere;
	world.num_materials++;
	world.num_spheres++;
	for (i32 i = -11; i < 11; i++) {
		for (i32 j = -11; j < 11; j++) {
			f32 material_check = unit_uniform(&prng_state);
			f32 x = (f32) i + (0.9 * unit_uniform(&prng_state));
			f32 y = 0.2;
			f32 z = (f32) j + (0.9 * unit_uniform(&prng_state));
			Point3D position = {x, y, z};
			Point3D target = {4.0, 0.2, 0.0};
			Point3D distance = position - target;
			if (l2_norm(&distance) > 0.9) {
				Material material;
				Sphere sphere;
				if (material_check < 0.8) {
					// make diffuse sphere
					RGBA random_color = random_opaque_color(&prng_state);
					material = {
						.color = random_color,
						.scatter_index = 1.0,
						.refractive_index = 0.0
					};
				} else if (material_check < 0.95) {
					// make fuzzy reflective sphere
					RGBA random_color = random_opaque_color(&prng_state, 0.5, 1.0);
					f32 scatter_index = unit_uniform(&prng_state);
					material = {
						.color = random_color,
						.scatter_index = scatter_index,
						.refractive_index = 0.0
					};
				} else {
					// make refractive sphere
					material = {
						.color = (RGBA) {1.0, 1.0, 1.0, 1.0},
						.scatter_index = 0.0,
						.refractive_index = 1.5
					};
				}
				sphere = {
					.origin = position,
					.radius = y,
					.material_index = world.num_materials
				};
				world.materials[world.num_materials] = material;
				world.spheres[world.num_spheres] = sphere;
				world.num_materials++;
				world.num_spheres++;
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
		.material_index = world.num_materials
	};
	world.materials[world.num_materials] = big_glass_material;
	world.spheres[world.num_spheres] = big_glass_sphere;
	world.num_materials++;
	world.num_spheres++;
	Material big_diffuse_material = {
		.color = (RGBA) {0.4, 0.2, 0.1, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere big_diffuse_sphere {
		.origin = (Point3D) {-4.0, 1.0, 0.0},
		.radius = 1.0,
		.material_index = world.num_materials
	};
	world.materials[world.num_materials] = big_diffuse_material;
	world.spheres[world.num_spheres] = big_diffuse_sphere;
	world.num_materials++;
	world.num_spheres++;
	Material big_reflective_material = {
		.color = (RGBA) {0.7, 0.6, 0.5, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 0.0,
	};
	Sphere big_reflective_sphere {
		.origin = (Point3D) {4.0, 1.0, 0.0},
		.radius = 1.0,
		.material_index = world.num_materials
	};
	world.materials[world.num_materials] = big_reflective_material;
	world.spheres[world.num_spheres] = big_reflective_sphere;
	world.num_materials++;
	world.num_spheres++;
	printf("[info] total spheres: %d\n", world.num_spheres);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		&background,
		image_plane.rows,
		image_plane.cols,
		32,
		32,
		100,
		50,
		num_threads
	);
	return ray_count;
}

inline f32 arasp_9spheres(u32 num_threads) {
	f32 fov = 60.0;
	f32 aperture = 0.1;
	f32 aspect_ratio = (16.0 / 9.0);
	u32 pixel_height = 720;
	ImagePlane image_plane = create_image_plane(fov, aspect_ratio, pixel_height);
	Point3D origin = {0.0, 2.0, 3.0};
	Point3D target = {0.0, 0.0, 0.0};
	Vec3D normal = origin - target;
	f32 focal_distance = 3.0;
	normal = normalize(&normal);
	Vec3D up = {0.0, 1.0, 0.0};
	RGBA background = {0.5, 0.7, 1.0, 1.0};
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
	Material m1 = {
		.color = (RGBA) {0.8, 0.8, 0.8, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m2 = {
		.color = (RGBA) {0.8, 0.4, 0.4, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m3 = {
		.color = (RGBA) {0.4, 0.8, 0.4, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m4 = {
		.color = (RGBA) {0.4, 0.4, 0.8, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 0.0,
	};
	Material m5 = {
		.color = (RGBA) {0.4, 0.8, 0.4, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 0.0,
	};
	Material m6 = {
		.color = (RGBA) {0.4, 0.8, 0.4, 1.0},
		.scatter_index = 0.2,
		.refractive_index = 0.0,
	};
	Material m7 = {
		.color = (RGBA) {0.4, 0.8, 0.4, 1.0},
		.scatter_index = 0.6,
		.refractive_index = 0.0,
	};
	Material m8 = {
		.color = (RGBA) {1.0, 1.0, 1.0, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 1.5,
	};
	Material m9 = {
		.color = (RGBA) {0.8, 0.6, 0.2, 1.0},
		.emit = (RGBA) {30.0, 25.0, 15.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere s1 = {
		.origin = (Point3D) {0.0, -100.5, -1.0},
		.radius = 100.0,
		.material_index = 0
	};
	Sphere s2 = {
		.origin = (Point3D) {2.0, 0.0, -1.0},
		.radius = 0.5,
		.material_index = 1
	};
	Sphere s3 = {
		.origin = (Point3D) {0.0, 0.0, -1.0},
		.radius = 0.5,
		.material_index = 2
	};
	Sphere s4 = {
		.origin = (Point3D) {-2.0, 0.0, -1.0},
		.radius = 0.5,
		.material_index = 3
	};
	Sphere s5 = {
		.origin = (Point3D) {2.0, 0.0, 1.0},
		.radius = 0.5,
		.material_index = 4
	};
	Sphere s6 = {
		.origin = (Point3D) {0.0, 0.0, 1.0},
		.radius = 0.5,
		.material_index = 5
	};
	Sphere s7 = {
		.origin = (Point3D) {-2.0, 0.0, 1.0},
		.radius = 0.5,
		.material_index = 6
	};
	Sphere s8 = {
		.origin = (Point3D) {0.5, 1.0, 0.5},
		.radius = 0.5,
		.material_index = 7
	};
	Sphere s9 = {
		.origin = (Point3D) {-1.5, 1.5, 0.0},
		.radius = 0.3,
		.material_index = 8
	};
	Material materials[9] = {
		m1,
		m2,
		m3,
		m4,
		m5,
		m6,
		m7,
		m8,
		m9,
	};
	Sphere spheres[9] = {
		s1,
		s2,
		s3,
		s4,
		s5,
		s6,
		s7,
		s8,
		s9,
	};
	World world = {};
	world.num_materials = 9;
	world.num_spheres = 9;
	world.materials = materials;
	world.spheres = spheres;
	printf("[info] total spheres: %d\n", world.num_spheres);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		&background,
		image_plane.rows,
		image_plane.cols,
		64,
		64,
		1024,
		50,
		num_threads
	);
	return ray_count;
}

inline f32 caseym_5spheres(u32 num_threads) {
	f32 fov = 31.0;
	f32 aperture = 0.1;
	f32 aspect_ratio = (16.0 / 9.0);
	u32 pixel_height = 720;
	ImagePlane image_plane = create_image_plane(fov, aspect_ratio, pixel_height);
	Point3D origin = {0.0, 1.0, -10.0};
	Point3D target = normalize(&origin) - origin;
	Vec3D normal = origin - target;
	f32 focal_distance = 10.0;
	normal = normalize(&normal);
	Vec3D up = {0.0, 1.0, 0.0};
	RGBA background = {0.5, 0.7, 1.0, 1.0};
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
	Material m1 = {
		.color = (RGBA) {0.5, 0.5, 0.5, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m2 = {
		.color = (RGBA) {0.7, 0.5, 0.3, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m3 = {
		.color = (RGBA) {0.9, 0.0, 0.0, 1.0},
		.emit = (RGBA) {4.0, 0.0, 0.0, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Material m4 = {
		.color = (RGBA) {0.2, 0.8, 0.2, 1.0},
		.scatter_index = 0.7,
		.refractive_index = 0.0,
	};
	Material m5 = {
		.color = (RGBA) {0.4, 0.8, 0.9, 1.0},
		.scatter_index = 0.85,
		.refractive_index = 0.0,
	};
	Material m6 = {
		.color = (RGBA) {0.95, 0.95, 0.95, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 0.0,
	};
	Sphere s1 = {
		.origin = (Point3D) {0.0, -1.0e3, -1.0},
		.radius = 1.0e3,
		.material_index = 0
	};
	Sphere s2 = {
		.origin = (Point3D) {0.0, 0.0, 0.0},
		.radius = 1.0,
		.material_index = 1
	};
	Sphere s3 = {
		.origin = (Point3D) {-3.0, 0.0, -2.0},
		.radius = 1.0,
		.material_index = 2
	};
	Sphere s4 = {
		.origin = (Point3D) {2.0, 2.0, -1.0},
		.radius = 1.0,
		.material_index = 3
	};
	Sphere s5 = {
		.origin = (Point3D) {-1.0, 3.0, -1.0},
		.radius = 1.0,
		.material_index = 4
	};
	Sphere s6 = {
		.origin = (Point3D) {2.0, 0.0, 3.0},
		.radius = 2.0,
		.material_index = 5
	};
	Material materials[6] = {
		m1,
		m2,
		m3,
		m4,
		m5,
		m6,
	};
	Sphere spheres[6] = {
		s1,
		s2,
		s3,
		s4,
		s5,
		s6,
	};
	World world = {};
	world.num_materials = 6;
	world.num_spheres = 6;
	world.materials = materials;
	world.spheres = spheres;
	printf("[info] total spheres: %d\n", world.num_spheres);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		&background,
		image_plane.rows,
		image_plane.cols,
		64,
		64,
		1024,
		8,
		num_threads
	);
	return ray_count;
}

int main(int argc, char **args) {
	u32 num_threads = core_count() - 1;
	f32 ray_count = caseym_5spheres(num_threads);
	// f32 ray_count = arasp_9spheres(num_threads);
	// f32 ray_count = random_spheres(num_threads);
	// f32 ray_count = test_spheres(num_threads);
	return 0;
}
