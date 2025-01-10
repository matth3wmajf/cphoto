#pragma once

#include <math.h>
#include <stdint.h>

/*
 *	Our color type, it should be exactly 32-bits in size.
 *	It can be accessed as a neat structure or as an array. Either way, both
 *	are equally fast and have their own advantages in terms of ease of use.
 */
typedef union
{
	struct
	{
		uint8_t r, g, b, a;
	} value;
	
	uint8_t array[4];
} color3_t;

/* The most precise float we can get. */
typedef long double floatmax_t;

/* A two-dimensional vector based on the `floatmax_t` type. */
typedef union
{
	struct
	{
		floatmax_t x, y; 
	} value;

	floatmax_t array[2];
} floatmax_vector2_t;

/* A three-dimensional vector based on the `floatmax_t` type. */
typedef union
{
	struct
	{
		floatmax_t x, y, z;
	} value;

	floatmax_t array[3];
} floatmax_vector3_t;

/* A four-dimensional vector based on the `floatmax_t` type. */
typedef union
{
	struct
	{
		floatmax_t x, y, z, w;
	} value;

	floatmax_t array[4];
} floatmax_vector4_t;

/* Operation(s) for the `floatmax_vector2_t` type. */

/* Perform addition between two `floatmax_vector2_t` objects. */
static inline floatmax_vector2_t floatmax_vector2_add(floatmax_vector2_t a, floatmax_vector2_t b)
{
	return (floatmax_vector2_t){a.value.x + b.value.x, a.value.y + b.value.y};
}

/* Perform subtraction between two `floatmax_vector2_t` objects. */
static inline floatmax_vector2_t floatmax_vector2_subtract(floatmax_vector2_t a, floatmax_vector2_t b)
{
	return (floatmax_vector2_t){a.value.x - b.value.x, a.value.y - b.value.y};
}

/* Perform multiplication between two `floatmax_vector2_t` objects. */
static inline floatmax_vector2_t floatmax_vector2_multiply(floatmax_vector2_t a, floatmax_vector2_t b)
{
	return (floatmax_vector2_t){a.value.x * b.value.x, a.value.y * b.value.y};
}

/* Perform division between two `floatmax_vector2_t` objects. */
static inline floatmax_vector2_t floatmax_vector2_divide(floatmax_vector2_t a, floatmax_vector2_t b)
{
	return (floatmax_vector2_t){a.value.x / b.value.x, a.value.y / b.value.y};
}

/* Normalize a `floatmax_vector2_t` object. */
static inline floatmax_vector2_t floatmax_vector2_normalize(floatmax_vector2_t a)
{
	floatmax_t l_k = 1.0 / sqrtl(a.value.x * a.value.x + a.value.y * a.value.y);
	a.value.x *= l_k;
	a.value.y *= l_k;
	return a;
}

/* Operation(s) for the `floatmax_vector3_t` type. */

/* Perform addition between two `floatmax_vector3_t` objects. */
static inline floatmax_vector3_t floatmax_vector3_add(floatmax_vector3_t a, floatmax_vector3_t b)
{
	return (floatmax_vector3_t){a.value.x + b.value.x, a.value.y + b.value.y, a.value.z + b.value.z};
}

/* Perform subtraction between two `floatmax_vector3_t` objects. */
static inline floatmax_vector3_t floatmax_vector3_subtract(floatmax_vector3_t a, floatmax_vector3_t b)
{
	return (floatmax_vector3_t){a.value.x - b.value.x, a.value.y - b.value.y, a.value.z - b.value.z};
}

/* Perform multiplication between two `floatmax_vector3_t` objects. */
static inline floatmax_vector3_t floatmax_vector3_multiply(floatmax_vector3_t a, floatmax_vector3_t b)
{
	return (floatmax_vector3_t){a.value.x * b.value.x, a.value.y * b.value.y, a.value.z * b.value.z};
}

/* Perform division between two `floatmax_vector3_t` objects. */
static inline floatmax_vector3_t floatmax_vector3_divide(floatmax_vector3_t a, floatmax_vector3_t b)
{
	return (floatmax_vector3_t){a.value.x / b.value.x, a.value.y / b.value.y, a.value.z / b.value.z};
}

/* Normalize a `floatmax_vector3_t` object. */
static inline floatmax_vector3_t floatmax_vector3_normalize(floatmax_vector3_t a)
{
	floatmax_t l_k = 1.0 / sqrtl(a.value.x * a.value.x + a.value.y * a.value.y + a.value.z * a.value.z);
	a.value.x *= l_k;
	a.value.y *= l_k;
	a.value.z *= l_k;
	return a;
}

/* Get dot product of two `floatmax_vector3_t` objects. */
static inline floatmax_t floatmax_vector3_dot(floatmax_vector3_t a, floatmax_vector3_t b)
{
	return a.value.x * b.value.x + a.value.y * b.value.y + a.value.z * b.value.z;
}

/* Perform a scalar multiplication on a `floatmax_vector3_t` object. */
static inline floatmax_vector3_t floatmax_vector3_multiply_scalar(floatmax_vector3_t a, floatmax_t scalar)
{
	return (floatmax_vector3_t){a.value.x * scalar, a.value.y * scalar, a.value.z * scalar};
}

/* Clamp a `floatmax_vector3_t` object. */
static inline floatmax_vector3_t floatmax_vector3_clamp(floatmax_vector3_t v, floatmax_t min, floatmax_t max)
{
	floatmax_vector3_t result;
	result.value.x = fmax(min, fmin(v.value.x, max));
	result.value.y = fmax(min, fmin(v.value.y, max));
	result.value.z = fmax(min, fmin(v.value.z, max));
	return result;
}