#pragma once

#include <cphoto/object.h>
#include <cphoto/numeric.h>
#include <cphoto/framebuffer.h>

/* ... */
typedef struct
{
	floatmax_vector3_t origin;
	floatmax_vector3_t direction;
} ray_t;

int ray_obtain(ray_t *ray, floatmax_vector3_t *color, object_t *object_buffer, uintmax_t object_buffer_size);