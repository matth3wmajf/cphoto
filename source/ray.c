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
	floatmax_t closest_t = -1.0; // To keep track of the closest hit
	floatmax_vector3_t hit_color = {0.0, 0.0, 0.0};

	for(uintmax_t l_i = 0; l_i < object_buffer_size; l_i++)
	{
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
				/* Calculate the nearest root. */
				floatmax_t root = (-l_b - sqrtl(l_discriminant)) / (2.0 * l_a);

				/* Only consider positive roots (in front of the ray origin). */
				if (root > 0.0 && (closest_t < 0.0 || root < closest_t))
				{
					closest_t = root;

					/* Compute the normal at the hit point. */
					floatmax_vector3_t hit_point = floatmax_vector3_add(ray->origin, floatmax_vector3_multiply_scalar(ray->direction, root));
					floatmax_vector3_t l_n = floatmax_vector3_normalize(floatmax_vector3_subtract(hit_point, object_buffer[l_i].data.sphere.center));

					/* Assign the color based on the normal vector. */
					hit_color = (floatmax_vector3_t){
						0.5 * (l_n.value.x + 1.0),
						0.5 * (l_n.value.y + 1.0),
						0.5 * (l_n.value.z + 1.0)
					};
				}
			}
		}
	}

	/* If there was a hit, return the computed color. */
	if(closest_t > 0.0)
	{
		*color = hit_color;
		return 0;
	}

	/* The background gradient. */
	floatmax_vector3_t l_unit_direction = floatmax_vector3_normalize(ray->direction);
	floatmax_t l_t = 0.5 * (l_unit_direction.value.y + 1.0);

	/* Set the final color. */
	*color = (floatmax_vector3_t){
		(1.0 - l_t) * 1.0 + l_t * 0.5,
		(1.0 - l_t) * 1.0 + l_t * 0.7,
		(1.0 - l_t) * 1.0 + l_t * 1.0
	};

	return 0;
}