#pragma once

#include <cphoto/numeric.h>

/* ... */
typedef struct
{
	enum
	{
		OBJECT_TYPE_NONE,
		OBJECT_TYPE_SPHERE
	} type;

	union
	{
		struct
		{
			floatmax_vector3_t center;
			floatmax_t radius;
			floatmax_vector3_t color;
		} sphere;
	} data;
} object_t;