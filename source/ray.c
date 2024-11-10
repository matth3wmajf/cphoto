#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <malloc.h>

#include <cphoto/object.h>
#include <cphoto/ray.h>
#include <cphoto/framebuffer.h>
#include <cphoto/numeric.h>

int ray_color(ray_t *ray, color3_t *color)
{
	floatmax_vector3_t l_unit_direction = (floatmax_vector3_t){ray->direction.array[0] / sqrtl(ray->direction.array[0] * ray->direction.array[0] + ray->direction.array[1] * ray->direction.array[1] + ray->direction.array[2] * ray->direction.array[2]), ray->direction.array[1] / sqrtl(ray->direction.array[0] * ray->direction.array[0] + ray->direction.array[1] * ray->direction.array[1] + ray->direction.array[2] * ray->direction.array[2]), ray->direction.array[2] / sqrtl(ray->direction.array[0] * ray->direction.array[0] + ray->direction.array[1] * ray->direction.array[1] + ray->direction.array[2] * ray->direction.array[2])};
	floatmax_t l_a = 0.5 * (l_unit_direction.value.y + 1.0);
	*color = (color3_t){(uint8_t)((1.0 - l_a) * 255.0 + l_a * 127.5), (uint8_t)((1.0 - l_a) * 255.0 + l_a * 178.5), (uint8_t)((1.0 - l_a) * 255.0 + l_a * 255.0), 255};

	return 0;
}

int ray_render(framebuffer_t* framebuffer, object_t* object_buffer, uintmax_t object_buffer_size)
{
	if(framebuffer == NULL || object_buffer == NULL || object_buffer_size == 0) return -1;

	/* ... */
	floatmax_t l_viewport_height = 2.0;
	floatmax_t l_viewport_width = l_viewport_height * ((floatmax_t)framebuffer->width / framebuffer->height);
	
	/* ... */
	floatmax_t l_focal_length = 1.0;

	/* ... */
	floatmax_vector3_t l_camera_center = (floatmax_vector3_t){0.0, 0.0, 0.0};

	/* ... */
	floatmax_vector3_t l_viewport_u = (floatmax_vector3_t){l_viewport_width, 0.0, 0.0};
	floatmax_vector3_t l_viewport_v = (floatmax_vector3_t){0.0, -l_viewport_height, 0.0};

	/* ... */
	floatmax_vector3_t l_pixel_delta_u = {l_viewport_u.value.x / (floatmax_t)framebuffer->width, l_viewport_u.value.y / (floatmax_t)framebuffer->width, l_viewport_u.value.z / (floatmax_t)framebuffer->width};
	floatmax_vector3_t l_pixel_delta_v = {l_viewport_v.value.x / (floatmax_t)framebuffer->height, l_viewport_v.value.y / (floatmax_t)framebuffer->height, l_viewport_v.value.z / (floatmax_t)framebuffer->height};

	/* ... */
	floatmax_vector3_t l_viewport_upper_left = (floatmax_vector3_t){l_camera_center.value.x - 0.0 - l_viewport_u.value.x / 2.0 - l_viewport_v.value.x / 2.0, l_camera_center.value.y - 0.0 - l_viewport_u.value.y / 2.0 - l_viewport_v.value.y / 2.0, l_camera_center.value.z - l_focal_length - l_viewport_u.value.z / 2.0 - l_viewport_v.value.z / 2.0};
	floatmax_vector3_t l_center_pixel_location = (floatmax_vector3_t){l_viewport_upper_left.value.x + (l_pixel_delta_u.value.x + l_pixel_delta_v.value.x) * 0.5, l_viewport_upper_left.value.y + (l_pixel_delta_u.value.y + l_pixel_delta_v.value.y) * 0.5, l_viewport_upper_left.value.z + (l_pixel_delta_u.value.z + l_pixel_delta_v.value.z) * 0.5};

	/* Loop for each scanline in the framebuffer. */
	for(uintmax_t l_y = 0; l_y < framebuffer->height; l_y++)
	{
		/* Loop for each pixel in the scanline. */
		for(uintmax_t l_x = 0; l_x < framebuffer->width; l_x++)
		{
			/* ... */
			color3_t *l_pixel = &framebuffer->pixels[l_y * framebuffer->width + l_x];

			/* ... */
			floatmax_vector3_t l_pixel_center = {l_center_pixel_location.value.x + l_pixel_delta_u.value.x * (floatmax_t)l_x + l_pixel_delta_v.value.x * (floatmax_t)l_y, l_center_pixel_location.value.y + l_pixel_delta_u.value.y * (floatmax_t)l_x + l_pixel_delta_v.value.y * (floatmax_t)l_y, l_center_pixel_location.value.z + l_pixel_delta_u.value.z * (floatmax_t)l_x + l_pixel_delta_v.value.z * (floatmax_t)l_y};
			floatmax_vector3_t l_ray_direction = {l_pixel_center.value.x - l_camera_center.value.x, l_pixel_center.value.y - l_camera_center.value.y, l_pixel_center.value.z - l_camera_center.value.z};

			/* ... */
			ray_t l_ray = (ray_t){l_camera_center, l_ray_direction};

			/* ... */
			ray_color(&l_ray, l_pixel);
		}
	}

	return 0;
}