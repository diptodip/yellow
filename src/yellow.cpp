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

f32 test_spheres(u32 num_threads) {
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
	Traceable ground_sphere = {SphereT, s1, 0};
	Traceable red_sphere = {SphereT, s2, 1};
	Traceable metal_sphere = {SphereT, s3, 2};
	Traceable glass_sphere = {SphereT, s4, 3};
	Traceable glass_sphere_inside = {SphereT, s5, 4};
	Material materials[5] = {m1, m2, m3, m4, m5};
	Traceable traceables[5] = {
		ground_sphere,
		red_sphere,
		metal_sphere,
		glass_sphere,
		glass_sphere_inside
	};
	World world = {};
	world.num_materials = 5;
	world.num_traceables = 5;
	world.materials = materials;
	world.traceables = traceables;
	printf("[info] total traceables: %d\n", world.num_traceables);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		image_plane.rows,
		image_plane.cols,
		image_plane.rows / 36,
		image_plane.cols / 96,
		100,
		num_threads
	);
	return ray_count;
}

f32 random_spheres(u32 num_threads) {
	PRNGState prng_state = {134123};
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
	Camera camera = {origin, normal, up, image_plane, aperture, focal_distance};
	Material materials[488];
	Traceable traceables[488];
	World world = {};
	world.num_materials = 0;
	world.num_traceables = 0;
	world.materials = materials;
	world.traceables = traceables;
	static Material ground_material = {
		.color = (RGBA) {0.5, 0.5, 0.5, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere ground_sphere = {
		.origin = (Point3D) {0.0, -1000.0, 0.0},
		.radius = 1000.0,
	};
	Traceable ground = {SphereT, ground_sphere, world.num_materials};
	world.materials[world.num_materials] = ground_material;
	world.traceables[world.num_traceables] = ground;
	world.num_materials++;
	world.num_traceables++;
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
				Traceable traceable;
				if (material_check < 0.8) {
					// make diffuse sphere
					RGBA random_color = random_opaque_color(&prng_state);
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
					RGBA random_color = random_opaque_color(&prng_state, 0.5, 1.0);
					f32 scatter_index = unit_uniform(&prng_state);
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
				traceable = {SphereT, sphere, world.num_materials};
				world.materials[world.num_materials] = material;
				world.traceables[world.num_traceables] = traceable;
				world.num_materials++;
				world.num_traceables++;
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
	Traceable big_glass_traceable = {SphereT, big_glass_sphere, world.num_materials};
	world.materials[world.num_materials] = big_glass_material;
	world.traceables[world.num_traceables] = big_glass_traceable;
	world.num_materials++;
	world.num_traceables++;
	Material big_diffuse_material = {
		.color = (RGBA) {0.4, 0.2, 0.1, 1.0},
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere big_diffuse_sphere {
		.origin = (Point3D) {-4.0, 1.0, 0.0},
		.radius = 1.0,
	};
	Traceable big_diffuse_traceable = {SphereT, big_diffuse_sphere, world.num_materials};
	world.materials[world.num_materials] = big_diffuse_material;
	world.traceables[world.num_traceables] = big_diffuse_traceable;
	world.num_materials++;
	world.num_traceables++;
	Material big_reflective_material = {
		.color = (RGBA) {0.7, 0.6, 0.5, 1.0},
		.scatter_index = 0.0,
		.refractive_index = 0.0,
	};
	Sphere big_reflective_sphere {
		.origin = (Point3D) {4.0, 1.0, 0.0},
		.radius = 1.0,
	};
	Traceable big_reflective_traceable = {SphereT, big_reflective_sphere, world.num_materials};
	world.materials[world.num_materials] = big_reflective_material;
	world.traceables[world.num_traceables] = big_reflective_traceable;
	world.num_materials++;
	world.num_traceables++;
	printf("[info] total traceables: %d\n", world.num_traceables);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		image_plane.rows,
		image_plane.cols,
		16,
		16,
		100,
		num_threads
	);
	return ray_count;
}

f32 aras_9spheres(u32 num_threads) {
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
		.scatter_index = 1.0,
		.refractive_index = 0.0,
	};
	Sphere s1 = {
		.origin = (Point3D) {0.0, -100.5, -1.0},
		.radius = 100.0,
	};
	Sphere s2 = {
		.origin = (Point3D) {2.0, 0.0, -1.0},
		.radius = 0.5,
	};
	Sphere s3 = {
		.origin = (Point3D) {0.0, 0.0, -1.0},
		.radius = 0.5,
	};
	Sphere s4 = {
		.origin = (Point3D) {-2.0, 0.0, -1.0},
		.radius = 0.5,
	};
	Sphere s5 = {
		.origin = (Point3D) {2.0, 0.0, 1.0},
		.radius = 0.5,
	};
	Sphere s6 = {
		.origin = (Point3D) {0.0, 0.0, 1.0},
		.radius = 0.5,
	};
	Sphere s7 = {
		.origin = (Point3D) {-2.0, 0.0, 1.0},
		.radius = 0.5,
	};
	Sphere s8 = {
		.origin = (Point3D) {0.5, 1.0, 0.5},
		.radius = 0.5,
	};
	Sphere s9 = {
		.origin = (Point3D) {-1.5, 1.5, 0.0},
		.radius = 0.3,
	};
	Traceable t1 = {SphereT, s1, 0};
	Traceable t2 = {SphereT, s2, 1};
	Traceable t3 = {SphereT, s3, 2};
	Traceable t4 = {SphereT, s4, 3};
	Traceable t5 = {SphereT, s5, 4};
	Traceable t6 = {SphereT, s6, 5};
	Traceable t7 = {SphereT, s7, 6};
	Traceable t8 = {SphereT, s8, 7};
	Traceable t9 = {SphereT, s9, 8};
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
	Traceable traceables[9] = {
		t1,
		t2,
		t3,
		t4,
		t5,
		t6,
		t7,
		t8,
		t9,
	};
	World world = {};
	world.num_materials = 9;
	world.num_traceables = 9;
	world.materials = materials;
	world.traceables = traceables;
	printf("[info] total traceables: %d\n", world.num_traceables);
	printf("[info] total materials: %d\n", world.num_materials);
	f32 ray_count = render(
		&world,
		&camera,
		image_plane.rows,
		image_plane.cols,
		16,
		16,
		100,
		num_threads
	);
	return ray_count;
}

int main(int argc, char **args) {
	u32 num_threads = core_count() - 1;
	f32 sc = tick();
	f32 ray_count = aras_9spheres(num_threads);
	// f32 ray_count = random_spheres(num_threads);
	// f32 ray_count = test_spheres(num_threads);
	printf("Processed %llu rays\n", (u64) ray_count);
	f32 ec = tick();
	f32 dc = ec - sc;
	printf("[info] scene rendered in %.2f seconds on %d threads\n", dc, num_threads + 1);
	printf("[info] rendered %.2f Mrays/s\n", (ray_count / 1.0e6) / dc);
	printf("[info] ray timing: %.10f ms/ray \n", (dc * 1000.0) / ray_count);
	return 0;
}
