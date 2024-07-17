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

inline Point3D ray_at(Ray *ray, f32 t) {
	return ray->origin + (t * ray->direction);
}

inline Ray diffuse_bounce(PRNGState *prng_state, Ray *ray, Vec3D *normal_pointer, Point3D *off_pointer) {
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	Vec3D random_direction = normal + random_unit_vector(prng_state);
	return (Ray) {off, random_direction};
}

inline Ray reflect(Ray *ray, Vec3D *normal_pointer, Point3D *off_pointer) {
	Vec3D direction = ray->direction;
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	Vec3D reflected = direction - (2 * dot(&direction, normal_pointer) * normal);
	return (Ray) {off, reflected};
}

inline Ray fuzzy_reflect(
	PRNGState *prng_state,
	Ray *ray,
	Vec3D *normal_pointer,
	Point3D *off_pointer,
	f32 scatter_index
) {
	Vec3D direction = ray->direction;
	Vec3D normal = *normal_pointer;
	Point3D off = *off_pointer;
	Vec3D reflected = direction - (2 * dot(&direction, &normal) * normal);
	Vec3D fuzzy_reflected = reflected + (scatter_index * random_unit_sphere_vector(prng_state));
	return (Ray) {off, fuzzy_reflected};
}

inline f32 schlick(f32 cos_theta, f32 refraction_ratio) {
	f32 r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
	r0 *= r0;
	return r0 + (1.0 - r0) * powf((1.0 - cos_theta), 5.0);
}

inline Ray refract(
	PRNGState *prng_state,
	Ray *ray,
	Vec3D *normal_p,
	Point3D *off_p,
	bool inside,
	f32 refractive_index
) {
	Vec3D normal = *normal_p;
	Point3D off = *off_p;
	f32 refraction_ratio = inside ? refractive_index : (1.0 / refractive_index);
	Vec3D direction = normalize(&ray->direction);
	Vec3D negative_direction = -direction;
	f32 cos_theta = fmin(dot(&negative_direction, normal_p), 1.0);
	f32 sin_theta = sqrt(1.0 - (cos_theta * cos_theta));
	if ((refraction_ratio * sin_theta) > 1.0) {
		// must reflect
		return reflect(ray, normal_p, off_p);
	}
	f32 reflectivity = schlick(cos_theta, refraction_ratio);
	f32 reflect_check = unit_uniform(prng_state);
	if (reflect_check < reflectivity) {
		return reflect(ray, normal_p, off_p);
	}
	Vec3D perpendicular = refraction_ratio * (direction + (cos_theta * normal));
	Vec3D parallel = -sqrt(fabs(1.0 - l2_norm_squared(&perpendicular))) * normal;
	Vec3D refracted = parallel + perpendicular;
	return (Ray) {off, refracted};
}

inline Ray scatter(PRNGState *prng_state, Ray *ray, Vec3D *normal, Point3D *off, f32 scatter_index) {
	if (scatter_index == 1.0) {
		return diffuse_bounce(prng_state, ray, normal, off);
	} else if (scatter_index == 0.0) {
		return reflect(ray, normal, off);
	} else {
		return fuzzy_reflect(prng_state, ray, normal, off, scatter_index);
	}
}

struct Intersection {
	Point3D origin;
	Vec3D normal;
	b8 inside;
	b8 intersected;
	u32 material_index;
};

struct IntersectionResult {
	Point3D origin;
	Vec3D normal; // always local, i.e. points away from ray direction
	f32 distance;
	b8 inside;
	b8 intersected;
};

inline IntersectionResult intersect_sphere(Ray *ray, Sphere *sphere) {
	IntersectionResult result = {};
	result.intersected = false;
	Point3D shifted_origin = ray->origin - sphere->origin;
	f32 direction_sq_l2 = dot(&ray->direction, &ray->direction);
	f32 origin_sq_l2 = dot(&shifted_origin, &shifted_origin);
	f32 origin_dot_direction = dot(&shifted_origin, &ray->direction);
	f32 squared_radius = sphere->radius * sphere-> radius;
	f32 discriminant = ((origin_dot_direction * origin_dot_direction) - (direction_sq_l2 * (origin_sq_l2 - squared_radius)));
	if (discriminant < 0.0) {
		return result;
	}
	f32 discriminant_sqrt = sqrt(discriminant);
	f32 t0 = (-origin_dot_direction - discriminant_sqrt) / direction_sq_l2;
	f32 t1 = (-origin_dot_direction + discriminant_sqrt) / direction_sq_l2;
	if ((t0 < 1e-2) && (t1 < 1e-2)) {
		return result;
	}
	result.intersected = true;
	f32 t = (t0 >= 1e-4) ? t0 : t1;
	Point3D intersection = ray_at(ray, t);
	Vec3D normal = (intersection - sphere->origin) / sphere->radius;
	if (dot(&normal, &ray->direction) > 0.0) {
		result.inside = true;
		normal = -normal;
	}
	result.origin = intersection;
	result.normal = normal;
	result.distance = t;
	return result;
}

inline IntersectionResult intersect_plane(Ray *ray, Plane *plane) {
	IntersectionResult result = {};
	result.intersected = false;
	Vec3D direction = ray->direction;
	Vec3D normal = plane->normal;
	f32 cos_theta = dot(&normal, &direction);
	if ((cos_theta < -1e-3) > (cos_theta > 1e-3)) {
		return result;
	}
	result.intersected = true;
	result.inside = false;
	Point3D origin = ray->origin;
	f32 t = (-plane->distance - dot(&normal, &origin))/cos_theta;
	Point3D intersection = ray_at(ray, t);
	result.origin = intersection;
	if (cos_theta > 0.0) {
		result.normal = cos_theta > 0 ? -normal : normal;
		result.inside = true;
	}
	result.distance = t;
	return result;
}

inline Intersection find_intersection(Ray *ray, World *world) {
	Intersection intersection = {};
	u32 num_spheres = world->num_spheres;
	u32 num_planes = world->num_planes;
	f32 nearest_distance = (f32) UINT32_MAX;
	// TODO(dd): try out unions with type enums again, measure perf
	for (u32 i = 0; i < num_spheres; i++) {
		Sphere *sphere = &world->spheres[i];
		IntersectionResult result;
		result = intersect_sphere(ray, sphere);
		if (result.intersected && (result.distance < nearest_distance)) {
			nearest_distance = result.distance;
			intersection.origin = result.origin;
			intersection.normal = result.normal;
			intersection.inside = result.inside;
			intersection.intersected = true;
			intersection.material_index = sphere->material_index;
		}
	}
	for (u32 i = 0; i < num_planes; i++) {
		Plane *plane = &world->planes[i];
		IntersectionResult result;
		result = intersect_plane(ray, plane);
		if (result.intersected && (result.distance < nearest_distance)) {
			nearest_distance = result.distance;
			intersection.origin = result.origin;
			intersection.normal = result.normal;
			intersection.inside = result.inside;
			intersection.intersected = true;
			intersection.material_index = plane->material_index;
		}
	}
	return intersection;
}

inline Ray prime_ray(PRNGState *prng_state, Camera *camera, f32 row_frac, f32 col_frac) {
	Vec3D basis1 = cross(&camera->up, &camera->normal);
	basis1 = normalize(&basis1);
	Vec3D basis2 = cross(&camera->normal, &basis1);
	f32 lens_radius = camera->aperture / 2.0;
	Vec3D random_lens_offset = lens_radius * random_unit_vector(prng_state);
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

inline RGBA trace(
	PRNGState *prng_state,
	RGBA *background,
	Ray *ray,
	World *world,
	RenderQueue *render_queue,
	u32 *num_traced_rays,
	u32 depth
) {
	RGBA color = {0.0, 0.0, 0.0, 1.0};
	RGBA attenuation = {1.0, 1.0, 1.0, 1.0};
	for (u32 d = 0; d < depth; d++) {
		*num_traced_rays += 1;
		Intersection intersection = find_intersection(ray, world);
		if (!intersection.intersected) {
			color += attenuation * *background;
			break;
		}
		Material material = world->materials[intersection.material_index];
		Point3D intersection_point = intersection.origin;
		Vec3D normal = intersection.normal;
		bool inside = intersection.inside;
		color += attenuation * material.emit;
		attenuation *= material.color;
		if (material.refractive_index > 0.0) {
			*ray = refract(prng_state, ray, &normal, &intersection_point, inside, material.refractive_index);
		} else {
			*ray = scatter(prng_state, ray, &normal, &intersection_point, material.scatter_index);
		}
	}
	return color;
}

inline b8 render_tile(RenderQueue *render_queue) {
	u64 job_index = sync_fetch_and_add(&render_queue->next_job_index, 1);
	if (job_index >= render_queue->num_tiles) {
		return false;
	}
	RenderJob *render_job = render_queue->jobs + job_index;
	PRNGState *prng_state = &render_job->prng_state;
	RGBA *background = render_job->background;
	World *world = render_job->world;
	Camera *camera = render_job->camera;
	u32 rows = render_job->rows;
	u32 cols = render_job->cols;
	u32 row_min = render_job->row_min;
	u32 row_max = render_job->row_max;
	u32 col_min = render_job->col_min;
	u32 col_max = render_job->col_max;
	u32 num_samples = render_job->num_samples;
	u32 max_depth = render_job->max_depth;
	u32 *out = render_job->out;
	u32 tile_rows = row_max - row_min;
	u32 tile_cols = col_max - col_min;
	u32 num_traced_rays = 0;
	for (u32 i = row_min; i < row_max; i++) {
		for (u32 j = col_min; j < col_max; j++) {
			RGBA color = {0.0, 0.0, 0.0, 1.0};
			for (u32 s = 0; s < num_samples; s++) {
				f32 row_rand = unit_uniform(prng_state);
				f32 col_rand = unit_uniform(prng_state);
				f32 u = ((f32) i + 0.5 + row_rand) / ((f32) rows);
				f32 v = ((f32) j + 0.5 + col_rand) / ((f32) cols);
				Ray ray = prime_ray(prng_state, camera, u, v);
				color += trace(
					prng_state,
					background,
					&ray,
					world,
					render_queue,
					&num_traced_rays,
					max_depth
				);
			}
			color = color / (f32) num_samples;
			u32 color_u32 = rgba_to_u32(&color);
			out[i * cols + j] = color_u32;
		}
	}
	sync_fetch_and_add(&render_queue->ray_count, num_traced_rays);
	sync_fetch_and_add(&render_queue->tile_rendered_count, 1);
	return true;
}

inline threaded render_thread(void *args) {
	RenderQueue *render_queue = (RenderQueue *) args;
	f32 progress = 0.0;
	while(render_tile(render_queue)) {};
	return 0;
}

inline f32 render(
	World *world,
	Camera *camera,
	RGBA *background,
	u32 rows,
	u32 cols,
	u32 tile_rows,
	u32 tile_cols,
	u32 num_samples,
	u32 max_depth,
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
			PRNGState prng_state = {read_entropy()};
			warm_up_xor_shift(&prng_state);
			render_job->prng_state = prng_state;
			render_job->background = background;
			render_job->world = world;
			render_job->camera = camera;
			render_job->rows = rows;
			render_job->cols = cols;
			render_job->row_min = row_min;
			render_job->row_max = row_max;
			render_job->col_min = col_min;
			render_job->col_max = col_max;
			render_job->num_samples = num_samples;
			render_job->max_depth = max_depth;
			render_job->out = out;
		}
	}
	// memory fence here, before we modify this from threads
	sync_fetch_and_add(&render_queue.next_job_index, 0);
	f64 sc = tick();
	ThreadHandle threads[num_threads];
	for (u32 i = 0; i < num_threads; i++) {
		ThreadHandle thread = create_thread(render_thread, (void *) &render_queue);
		threads[i] = thread;
	}
	f32 progress = 0.0;
	while (render_tile(&render_queue)) {
		progress = ((f32) render_queue.tile_rendered_count
			/ (f32) render_queue.num_tiles);
		printf("[running] rendered %.2f%%...\n", progress * 100.0);
	};
	for (u32 i = num_threads - 1; i > 0; i--) {
		join_thread(threads[i]);
	}
	f64 ec = tick();
	f64 dc = ec - sc;
	f32 ray_count = (f32) render_queue.ray_count;
	printf("[info] processed %llu rays\n", (u64) ray_count);
	printf("[info] scene rendered in %.9f seconds on %d threads\n", dc, num_threads + 1);
	printf("[info] rendered %.2f Mrays/s\n", (ray_count / 1.0e6) / dc);
	printf("[info] ray timing: %.10f ms/ray \n", (dc * 1000.0) / ray_count);
	printf("[info] writing image...\n");
	stbi_write_bmp("image.bmp", cols, rows, 4, image);
	printf("[ok] done!\n");
	return ray_count;
}
#endif // YELLOW_RAY
