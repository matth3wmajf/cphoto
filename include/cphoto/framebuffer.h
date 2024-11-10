#pragma once

#include <stdio.h>
#include <stdint.h>

#include <cphoto/numeric.h>

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