#pragma once

#include <stdio.h>
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

/*
 *	A basic framebuffer type, which serves as our intermediate image, which we
 *	can later convert & dump into an image file.
 */
typedef struct
{
	uintmax_t width, height;
	color3_t *pixels;
} framebuffer_t;

int framebuffer_create(framebuffer_t *framebuffer);
int framebuffer_resize(framebuffer_t *framebuffer, uintmax_t width, uintmax_t height);
int framebuffer_randomize(framebuffer_t* framebuffer);
int framebuffer_delete(framebuffer_t *framebuffer);
int framebuffer_dump(framebuffer_t* framebuffer, FILE* file);