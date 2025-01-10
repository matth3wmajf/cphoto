#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>

#include <cphoto/object.h>
#include <cphoto/ray.h>
#include <cphoto/framebuffer.h>
#include <cphoto/numeric.h>

/* Initialize a framebuffer's structure. */
int framebuffer_create(framebuffer_t *framebuffer)
{
	if(framebuffer == NULL) return -1;

	framebuffer->width = 0;
	framebuffer->height = 0;
	framebuffer->pixels = NULL;

	return 0;
}

/* Resize the framebuffer to a new resolution. */
int framebuffer_resize(framebuffer_t *framebuffer, uintmax_t width, uintmax_t height)
{
	if(framebuffer == NULL) return -1;

	framebuffer->width = width;
	framebuffer->height = height;

	framebuffer->pixels = realloc(framebuffer->pixels, width * height * sizeof(color3_t));

	if(framebuffer->pixels == NULL) return -2;

	return 0;
}

/* De-initialize the framebuffer, and potentially it's contents. */
int framebuffer_delete(framebuffer_t *framebuffer)
{
	if(framebuffer == NULL) return -1;

	if(framebuffer->pixels != NULL)
	{
		free(framebuffer->pixels);
		framebuffer->pixels = NULL;
	}

	framebuffer->width = 0;
	framebuffer->height = 0;

	return 0;
}

/* Render to the framebuffer's contents. */
int framebuffer_render(framebuffer_t* framebuffer, object_t* object_buffer, uintmax_t object_buffer_size)
{
	/* Return with error if a required argument is passed as nullified. */
	if(framebuffer == NULL || object_buffer == NULL || object_buffer_size == 0) return -1;

	/* Calculate the aspect ratio. */
	floatmax_t l_aspect_ratio = (floatmax_t)framebuffer->width / framebuffer->height;

	/* The lower-left corner. */
	floatmax_vector3_t l_lower_left_corner = (floatmax_vector3_t){-l_aspect_ratio, -1.0, -1.0};

	/* The horizontal & vertical vectors. */
	floatmax_vector3_t l_horizontal = (floatmax_vector3_t){2.0 * l_aspect_ratio, 0.0, 0.0}, l_vertical = (floatmax_vector3_t){0.0, 2.0, 0.0};

	/* The origin. */
	floatmax_vector3_t l_origin = (floatmax_vector3_t){0.0, 0.0, 0.0};

	/* Loop for each scanline in the framebuffer. */
	for(uintmax_t l_x = 0; l_x < framebuffer->width; l_x++)
	{
		/* Loop for each pixel in the scanline. */
		for(uintmax_t l_y = 0; l_y < framebuffer->height; l_y++)
		{
			color3_t *l_pixel = &framebuffer->pixels[l_y * framebuffer->width + l_x];
			floatmax_vector3_t l_pixel_color = {0.0, 0.0, 0.0};

			const uintmax_t l_samples_per_pixel = 4;

			for(uintmax_t l_s = 0; l_s < l_samples_per_pixel; ++l_s)
			{
				floatmax_t l_u = (floatmax_t)(l_x + (double)rand() / RAND_MAX) / (framebuffer->width -1);
				floatmax_t l_v = (floatmax_t)(l_y + (double)rand() / RAND_MAX) / (framebuffer->height -1);

				floatmax_vector3_t l_ray_direction;
				l_ray_direction = floatmax_vector3_add(l_lower_left_corner, floatmax_vector3_add(floatmax_vector3_multiply_scalar(l_horizontal, l_u), floatmax_vector3_multiply_scalar(l_vertical, l_v)));
				l_ray_direction = floatmax_vector3_normalize(l_ray_direction);

				ray_t l_ray;
				l_ray.origin = l_origin;
				l_ray.direction = l_ray_direction;

				floatmax_vector3_t l_color;
				ray_obtain(&l_ray, &l_color, object_buffer, object_buffer_size);

				l_pixel_color.value.x += l_color.value.x;
				l_pixel_color.value.y += l_color.value.y;
				l_pixel_color.value.z += l_color.value.z;
			}

			/* ... */
			l_pixel_color.value.x /= l_samples_per_pixel;
			l_pixel_color.value.y /= l_samples_per_pixel;
			l_pixel_color.value.z /= l_samples_per_pixel;


			*l_pixel = (color3_t){
				255.99 * l_pixel_color.value.x,
				255.99 * l_pixel_color.value.y,
				255.99 * l_pixel_color.value.z
			};
		}
	}

	return 0;
}

/* Dump the framebuffer to a bitmap file. */
int framebuffer_dump(framebuffer_t* framebuffer, FILE* file)
{
	if(framebuffer == NULL || file == NULL) return -1;

	/*
	 *	Convert the dimensions to 32-bit values, as the BMP file format relies
	 *	on 32-bit values.
	 */
	uint32_t l_width = (uint32_t)framebuffer->width;
	uint32_t l_height = (uint32_t)framebuffer->height;

	/* Determine the row size. */
	uint32_t l_row_size = (l_width * 3 + 3) & ~3;

	/* Determine the total size of pixel data. */
	uint32_t l_pixel_array_size = l_row_size * l_height;

	/*
	 *	Determine the total file size, but including the consideration of the
	 *	header's size.
	 */
	uint32_t l_file_size = 54 + l_pixel_array_size;

	/*
	 *	Fill out the header with the signature, file size, dimensions,
	 *	framebuffer offset, bits per pixel, image size, and color information
	 */
	uint8_t l_bmp_header[54] = {'B', 'M', l_file_size, l_file_size >> 8, l_file_size >> 16, l_file_size >> 24, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, l_width, l_width >> 8, l_width >> 16, l_width >> 24, l_height, l_height >> 8, l_height >> 16, l_height >> 24, 1, 0, 24, 0, 0, 0, 0, 0, l_pixel_array_size, l_pixel_array_size >> 8, l_pixel_array_size >> 16, l_pixel_array_size >> 24, 0x13, 0x0B, 0, 0, 0x13, 0x0B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	/* Write the header to the file. */
	fwrite(l_bmp_header, sizeof(l_bmp_header), 1, file);

	/* Write the framebuffer's contents to the file... */

	uint8_t l_padding[3] = {0, 0, 0};

	/* Loop for each row in the framebuffer. */
	for(intmax_t l_y = 0; l_y <(intmax_t)l_height; l_y++)
	{
		/* Loop for each pixel in the framebuffer. */
		for(intmax_t l_x = 0; l_x < (intmax_t)l_width; l_x++)
		{
			/* Extract the color from the pixel & re-format it. */
			color3_t l_color = framebuffer->pixels[l_y * l_width + l_x];
			uint8_t l_bgr[3] = {l_color.value.b, l_color.value.g, l_color.value.r};

			/* Write the re-formatted color to the file. */
			fwrite(l_bgr, sizeof(l_bgr), 1, file);
		}

		/* Write the padding to the file. */
		fwrite(l_padding, 1, (4 - (l_width * 3) % 4) % 4, file);
	}

	return 0;
}