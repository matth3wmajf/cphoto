#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <malloc.h>

#include <cphoto/object.h>
#include <cphoto/ray.h>
#include <cphoto/framebuffer.h>
#include <cphoto/numeric.h>

int ray_obtain(ray_t *ray, floatmax_vector3_t *color, object_t *object_buffer, uintmax_t object_buffer_size)
{
	/* Keep track of the closest hit. */
	floatmax_t l_closest_hit = -1.0;
	floatmax_vector3_t l_hit_color = {0.0, 0.0, 0.0};

	/* The accumulated color & remaining transparency. */
	floatmax_vector3_t l_accumulated_color = {0.0, 0.0, 0.0};
	floatmax_t l_remaining_transparency = 1.0;

	/* Iterate over all objects. */
	for(uintmax_t l_i = 0; l_i < object_buffer_size; l_i++)
	{
		/*
		 *	Follow intersection logic for spheres if our current object is a
		 *	sphere.
		 */
		if(object_buffer[l_i].type == OBJECT_TYPE_SPHERE)
		{
			/* The ray to sphere intersection. */
			floatmax_vector3_t l_oc = floatmax_vector3_subtract(ray->origin, object_buffer[l_i].data.sphere.center);
			floatmax_t l_a = floatmax_vector3_dot(ray->direction, ray->direction);
			floatmax_t l_b = 2.0 * floatmax_vector3_dot(l_oc, ray->direction);
			floatmax_t l_c = floatmax_vector3_dot(l_oc, l_oc) - object_buffer[l_i].data.sphere.radius * object_buffer[l_i].data.sphere.radius;
			floatmax_t l_discriminant = l_b * l_b - 4 * l_a * l_c;

			/* Check if the ray hits the sphere. */
			if(l_discriminant >= 0)
			{
				/* The closest root. */
				floatmax_t l_root = (-l_b - sqrtl(l_discriminant)) / (2.0 * l_a);
				
				/*
				 *	If bigger than zero, accumulate the color & remaining
				 *	transparency.
				 */
				if(l_root > 0.0)
				{
					/* Accumulate color with transparency. */
					l_accumulated_color.value.x += object_buffer[l_i].data.sphere.color.value.x * l_remaining_transparency * (1.0 - object_buffer[l_i].data.sphere.transparency);
					l_accumulated_color.value.y += object_buffer[l_i].data.sphere.color.value.y * l_remaining_transparency * (1.0 - object_buffer[l_i].data.sphere.transparency);
					l_accumulated_color.value.z += object_buffer[l_i].data.sphere.color.value.z * l_remaining_transparency * (1.0 - object_buffer[l_i].data.sphere.transparency);
					
					/* Calculate the remaining transparency. */
					l_remaining_transparency *= object_buffer[l_i].data.sphere.transparency;
				}
			}
		}
	}

	/* The direction. */
	floatmax_vector3_t l_unit_direction = floatmax_vector3_normalize(ray->direction);
	
	/* ... */
	floatmax_t l_t = 0.5 * (l_unit_direction.value.y + 1.0);

	/* The background color. */
	floatmax_vector3_t l_background_color = {(1.0 - l_t) * 1.0 + l_t * 0.5, (1.0 - l_t) * 1.0 + l_t * 0.7, (1.0 - l_t) * 1.0 + l_t * 1.0};

	/* Set the color, update it with the accumulated color. */
	color->value.x = l_accumulated_color.value.x + l_background_color.value.x * l_remaining_transparency;
	color->value.y = l_accumulated_color.value.y + l_background_color.value.y * l_remaining_transparency;
	color->value.z = l_accumulated_color.value.z + l_background_color.value.z * l_remaining_transparency;

	return 0;
}