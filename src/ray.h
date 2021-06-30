#ifndef YELLOW_RAY
#define YELLOW_RAY
#include <cmath>
#include <cstdio>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "linalg.h"
#include "colors.h"
#include "materials.h"
#include "cameras.h"
#include "threads.h"

struct Ray {
	Point3D origin;
	Vec3D direction;
};

inline Point3D ray_at(Ray *ray, f64 t) {
	return ray->origin + (t * ray->direction);
}

inline Ray diffuse_bounce(Ray *ray, Vec3D *normal_pointer, Point3D *off_pointer) {
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	Vec3D random_direction = normal + random_unit_vector();
	return (Ray) {off, random_direction};
}

inline Ray reflect(Ray *ray, Vec3D *normal_pointer, Point3D *off_pointer) {
	Vec3D direction = ray->direction;
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	Vec3D reflected = direction - (2 * dot(&direction, normal_pointer) * normal);
	return (Ray) {off, reflected};
}

inline Ray fuzzy_reflect(Ray *ray, Vec3D *normal_pointer, Point3D *off_pointer, f64 scatter_index) {
	Vec3D direction = ray->direction;
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	Vec3D reflected = direction - (2 * dot(&direction, &normal) * normal);
	Vec3D fuzzy_reflected = reflected + (scatter_index * random_unit_vector());
	return (Ray) {off, fuzzy_reflected};
}

inline f64 schlick(f64 cos_theta, f64 refraction_ratio) {
	f64 r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
	r0 *= r0;
	return r0 + (1.0 - r0) * std::pow((1.0 - cos_theta), 5.0);
}

inline Ray refract(Ray *ray, Vec3D *normal_pointer, Point3D *off_pointer, bool inside, f64 refractive_index) {
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	f64 refraction_ratio = inside ? refractive_index : (1.0 / refractive_index);
	Vec3D direction = normalize(&ray->direction);
	Vec3D negative_direction = -direction;
	f64 cos_theta = std::fmin(dot(&negative_direction, normal_pointer), 1.0);
	f64 sin_theta = std::sqrt(1.0 - (cos_theta * cos_theta));
	if ((refraction_ratio * sin_theta) > 1.0) {
		// must reflect
		return reflect(ray, normal_pointer, off_pointer);
	}
	f64 reflectivity = schlick(cos_theta, refraction_ratio);
	f64 reflect_check = unit_uniform();
	if (reflect_check < reflectivity) {
		return reflect(ray, normal_pointer, off_pointer);
	}
	Vec3D perpendicular = refraction_ratio * (direction + (cos_theta * normal));
	Vec3D parallel = -std::sqrt(std::fabs(1.0 - l2_norm_squared(&perpendicular))) * normal;
	Vec3D refracted = parallel + perpendicular;
	return (Ray) {off, refracted};
}

inline Ray scatter(Ray *ray, Vec3D *normal, Point3D *off, f64 scatter_index) {
	if (scatter_index == 1.0) {
		return diffuse_bounce(ray, normal, off);
	} else if (scatter_index == 0.0) {
		return reflect(ray, normal, off);
	} else {
		return fuzzy_reflect(ray, normal, off, scatter_index);
	}
}

struct Intersection {
	Point3D origin;
	Vec3D normal;
	b8 inside;
	b8 intersected;
	Traceable *traceable;
};

struct IntersectionResult {
	Point3D origin;
	Vec3D normal; // always local, i.e. points away from ray direction
	f64 distance;
	b8 inside;
	b8 intersected;
};

inline IntersectionResult intersect_sphere(Ray *ray, Sphere *sphere) {
	IntersectionResult result = {};
	Point3D shifted_origin = ray->origin - sphere->origin;
	f64 direction_sq_l2 = dot(&ray->direction, &ray->direction);
	f64 origin_sq_l2 = dot(&shifted_origin, &shifted_origin);
	f64 origin_dot_direction = dot(&shifted_origin, &ray->direction);
	f64 squared_radius = sphere->radius * sphere-> radius;
	f64 discriminant = ((origin_dot_direction * origin_dot_direction) - (direction_sq_l2 * (origin_sq_l2 - squared_radius)));
	if (discriminant < 0.0) {
		result.intersected = false;
		return result;
	}
	f64 discriminant_sqrt = std::sqrt(discriminant);
	f64 t0 = (-origin_dot_direction - discriminant_sqrt) / direction_sq_l2;
	f64 t1 = (-origin_dot_direction + discriminant_sqrt) / direction_sq_l2;
	if ((t0 < 1e-6) && (t1 < 1e-6)) {
		result.intersected = false;
		return result;
	}
	result.intersected = true;
	f64 t = (t0 >= 1e-6) ? t0 : t1;
	Point3D intersection = ray_at(ray, t);
	Vec3D distance_vec = intersection - ray->origin;
	Vec3D normal = (intersection - sphere->origin) / sphere->radius;
	if (dot(&normal, &ray->direction) > 0.0) {
		result.inside = true;
		normal = -normal;
	}
	result.origin = intersection;
	result.normal = normal;
	result.distance = l2_norm(&distance_vec);
	return result;
}

inline Intersection find_intersection(Ray *ray, World *world) {
	Intersection intersection = {};
	u32 num_traceables = world->num_traceables;
	f64 nearest_distance = (f64) UINT64_MAX;
	for (u32 i = 0; i < num_traceables; i++) {
		Traceable *traceable = &world->traceables[i];
		IntersectionResult result;
		// TODO(dd): handle other traceable types
		switch (traceable->type) {
			case SphereT:
				result = intersect_sphere(ray, (Sphere *) &traceable->object);
				break;
		}
		if (result.intersected && (result.distance < nearest_distance)) {
			nearest_distance = result.distance;
			intersection.origin = result.origin;
			intersection.normal = result.normal;
			intersection.inside = result.inside;
			intersection.intersected = true;
			intersection.traceable = traceable;
		}
	}
	return intersection;
}

inline Ray prime_ray(Camera *camera, f64 row_frac, f64 col_frac) {
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

inline RGBA trace(Ray *ray, World *world, RenderQueue *render_queue, u32 depth) {
	sync_fetch_and_add(&render_queue->ray_count, 1);
	if (depth <= 0) {
		// light enters the void if we hit the depth limit
		return (RGBA) {0.0, 0.0, 0.0, 1.0};
	}
	Intersection intersection = find_intersection(ray, world);
	if (!intersection.intersected) {
		// light is colored according to height on the image plane
		// if we didn't intersect anything
		Vec3D direction = normalize(&ray->direction);
		f64 height = 0.5 * (direction.y + 1.0);
		RGBA bg_color = {1.0 - (0.5 * height), 1.0 - (0.3 * height), 1.0, 1.0};
		return bg_color;
	}
	Traceable *traceable = intersection.traceable;
	Material material = world->materials[traceable->material_index];
	Point3D intersection_point = intersection.origin;
	Vec3D normal = intersection.normal;
	bool inside = intersection.inside;
	RGBA color = material.color;
	if (material.refractive_index > 0.0) {
		Ray refracted = refract(ray, &normal, &intersection_point, inside, material.refractive_index);
		color *= trace(&refracted, world, render_queue, depth - 1);
	} else {
		Ray scattered = scatter(ray, &normal, &intersection_point, material.scatter_index);
		color *= trace(&scattered, world, render_queue, depth - 1);
	}
	return color;
}

inline b8 render_tile(RenderQueue *render_queue) {
	u64 job_index = sync_fetch_and_add(&render_queue->next_job_index, 1);
	if (job_index >= render_queue->num_tiles) {
		return false;
	}
	RenderJob *render_job = render_queue->jobs + job_index;
	World *world = render_job->world;
	Camera *camera = render_job->camera;
	u32 rows = render_job->rows;
	u32 cols = render_job->cols;
	u32 row_min = render_job->row_min;
	u32 row_max = render_job->row_max;
	u32 col_min = render_job->col_min;
	u32 col_max = render_job->col_max;
	u32 num_samples = render_job->num_samples;
	u32 *out = render_job->out;
	u32 tile_rows = row_max - row_min;
	u32 tile_cols = col_max - col_min;
	for (u32 i = row_min; i < row_max; i++) {
		for (u32 j = col_min; j < col_max; j++) {
			RGBA color = {0.0, 0.0, 0.0, 1.0};
			for (u32 s = 0; s < num_samples; s++) {
				f64 row_rand = unit_uniform();
				f64 col_rand = unit_uniform();
				f64 u = ((f64) i + 0.5 + row_rand) / ((f64) rows);
				f64 v = ((f64) j + 0.5 + col_rand) / ((f64) cols);
				Ray ray = prime_ray(camera, u, v);
				color += trace(&ray, world, render_queue, 50);
			}
			color = color / (f64) num_samples;
			u32 color_u32 = rgba_to_u32(&color);
			out[i * cols + j] = color_u32;
		}
	}
	sync_fetch_and_add(&render_queue->tile_rendered_count, 1);
	return true;
}

inline threaded render_thread(void *args) {
	RenderQueue *render_queue = (RenderQueue *) args;
	f64 progress = 0.0;
	while(render_tile(render_queue)) {};
	return 0;
}

inline f64 render(
	World *world,
	Camera *camera,
	u32 rows,
	u32 cols,
	u32 tile_rows,
	u32 tile_cols,
	u32 num_samples,
	u32 num_threads
) {
	u32 *image = imalloc(rows, cols);
	u32 *out = image;
	u32 pixel_count = rows * cols;
	u32 num_tiles = ((rows + tile_rows - 1) / tile_rows)
		* ((cols + tile_cols - 1) / tile_cols);
	RenderQueue render_queue = {};
	render_queue.jobs = (RenderJob *)malloc(sizeof(RenderJob) * num_tiles);
	printf("\n[start] rendering %dpx x %dpx (width x height) image with %dpx x %dpx tiles\n", cols, rows, tile_cols, tile_rows);
	for (u32 i = 0; i < rows; i += tile_rows) {
		u32 row_min = i;
		u32 row_max = row_min + tile_rows;
		if (row_max > rows) {
			row_max = rows;
		}
		for (u32 j = 0; j < cols; j += tile_cols) {
			u32 col_min = j;
			u32 col_max = col_min + tile_cols;
			if (col_max > cols) {
				col_max = cols;
			}
			RenderJob *render_job = render_queue.jobs + render_queue.num_tiles++;
			render_job->world = world;
			render_job->camera = camera;
			render_job->rows = rows;
			render_job->cols = cols;
			render_job->row_min = row_min;
			render_job->row_max = row_max;
			render_job->col_min = col_min;
			render_job->col_max = col_max;
			render_job->num_samples = num_samples;
			render_job->out = out;
		}
	}
	sync_fetch_and_add(&render_queue.next_job_index, 0);
	ThreadHandle threads[num_threads];
	for (u32 i = 0; i < num_threads; i++) {
		ThreadHandle thread = create_thread(render_thread, (void *) &render_queue);
		threads[i] = thread;
	}
	f64 progress = 0.0;
	while (render_tile(&render_queue)) {
		progress = ((f64) render_queue.tile_rendered_count
			/ (f64) render_queue.num_tiles);
		printf("[running] rendered %.2f%%...\n", progress * 100.0);
	};
	for (u32 i = num_threads - 1; i > 0; i--) {
		join_thread(threads[i]);
	}
	f64 ray_count = (f64) render_queue.ray_count;
	printf("[info] writing image...\n");
	stbi_write_bmp("image.bmp", cols, rows, 4, image);
	printf("[ok] done!\n");
	return ray_count;
}
#endif // YELLOW_RAY
