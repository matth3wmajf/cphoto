#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>

#include <cphoto/framebuffer.h>

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

/* Randomize the framebuffer's contents. */
int framebuffer_randomize(framebuffer_t* framebuffer)
{
    if (framebuffer == NULL || framebuffer->pixels == NULL) return -1;

    srand((unsigned int) time(NULL));

    for(uintmax_t l_y = 0; l_y < framebuffer->height; l_y++)
	{
        for(uintmax_t l_x = 0; l_x < framebuffer->width; l_x++)
		{
            color3_t* l_pixel = &framebuffer->pixels[l_y * framebuffer->width + l_x];
            l_pixel->value.r = (uint8_t) rand() % 256;
            l_pixel->value.g = (uint8_t) rand() % 256;
            l_pixel->value.b = (uint8_t) rand() % 256;
            l_pixel->value.a = 255;
        }
    }

	return 0;
}

/* Dump the framebuffer to a bitmap file. */
int framebuffer_dump(framebuffer_t* framebuffer, FILE* file)
{
    if(framebuffer == NULL || file == NULL) return -1;

    uint32_t l_width = (uint32_t)framebuffer->width;
    uint32_t l_height = (uint32_t)framebuffer->height;
    uint32_t l_row_size = (l_width * 3 + 3) & ~3;
    uint32_t l_pixel_array_size = l_row_size * l_height;
    uint32_t l_file_size = 54 + l_pixel_array_size;

    uint8_t l_bmp_header[54] = {'B', 'M', l_file_size, l_file_size >> 8, l_file_size >> 16, l_file_size >> 24, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, l_width, l_width >> 8, l_width >> 16, l_width >> 24, l_height, l_height >> 8, l_height >> 16, l_height >> 24, 1, 0, 24, 0, 0, 0, 0, 0, l_pixel_array_size, l_pixel_array_size >> 8, l_pixel_array_size >> 16, l_pixel_array_size >> 24, 0x13, 0x0B, 0, 0, 0x13, 0x0B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    fwrite(l_bmp_header, sizeof(l_bmp_header), 1, file);

    uint8_t l_padding[3] = {0, 0, 0};

    for(intmax_t l_y = (intmax_t)l_height - 1; l_y >= 0; l_y--)
	{
        for(intmax_t l_x = 0; l_x < (intmax_t)l_width; l_x++)
		{
            color3_t l_color = framebuffer->pixels[l_y * l_width + l_x];
            uint8_t l_bgr[3] = {l_color.value.b, l_color.value.g, l_color.value.r};

            fwrite(l_bgr, sizeof(l_bgr), 1, file);
        }

        fwrite(l_padding, 1, (4 - (l_width * 3) % 4) % 4, file);
    }

    return 0;
}