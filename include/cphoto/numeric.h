#pragma once

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