#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <cphoto/object.h>
#include <cphoto/ray.h>
#include <cphoto/framebuffer.h>
#include <cphoto/numeric.h>

int main(int argc, char* argv[])
{
	/* Seed the randomness. */
	srand(time(NULL));

	/* Declare the framebuffer handle. */
	framebuffer_t l_framebuffer;

	/* Initialize & re-size the framebuffer to a 1920x1080 resolution. */
	framebuffer_create(&l_framebuffer);
	framebuffer_resize(&l_framebuffer, 1920, 1080);

	/* Declare & initialize an empty object buffer. */
	object_t *l_object_buffer = NULL;
	uintmax_t l_object_buffer_size = 0;

#if 0
	/* Re-size the buffer to store two spheres. */
	l_object_buffer_size += (uintmax_t)rand() % 25 + 5;
	l_object_buffer = realloc(l_object_buffer, l_object_buffer_size * sizeof(object_t));

	/* Populate the buffer... */
	for(uintmax_t l_i = 0; l_i < l_object_buffer_size; l_i++)
	{
		l_object_buffer[l_i].type = OBJECT_TYPE_SPHERE;
		l_object_buffer[l_i].data.sphere.center = (floatmax_vector3_t){((floatmax_t)rand() / RAND_MAX) * 4.0 - 2.0, ((floatmax_t)rand() / RAND_MAX) * 4.0 - 2.0, (((floatmax_t)rand() / RAND_MAX) * 4.0 - 2.0) - 5.0};
		l_object_buffer[l_i].data.sphere.radius = (floatmax_t)rand() / RAND_MAX;
		l_object_buffer[l_i].data.sphere.color = (floatmax_vector3_t){(floatmax_t)rand() / RAND_MAX, (floatmax_t)rand() / RAND_MAX, (floatmax_t)rand() / RAND_MAX};
		l_object_buffer[l_i].data.sphere.diffuse = 0.3;
		l_object_buffer[l_i].data.sphere.transparency = 0.0;
	}
#endif

	/* Re-size the buffer to store two spheres. */
	l_object_buffer_size += 2;
	l_object_buffer = realloc(l_object_buffer, l_object_buffer_size * sizeof(object_t));

	/* Create the first sphere. */
	l_object_buffer[0].type = OBJECT_TYPE_SPHERE;
	l_object_buffer[0].data.sphere.center = (floatmax_vector3_t){0.0, 0.0, -1.0};
	l_object_buffer[0].data.sphere.radius = 0.5;
	l_object_buffer[0].data.sphere.color = (floatmax_vector3_t){0.1, 0.1, 0.1};
	l_object_buffer[0].data.sphere.diffuse = 0.3;
	l_object_buffer[0].data.sphere.transparency = 0.0;

	/* Create the second sphere. */
	l_object_buffer[1].type = OBJECT_TYPE_SPHERE;
	l_object_buffer[1].data.sphere.center = (floatmax_vector3_t){0.0, -100.5, -1.0};
	l_object_buffer[1].data.sphere.radius = 100;
	l_object_buffer[1].data.sphere.color = (floatmax_vector3_t){0.1, 0.1, 0.1};
	l_object_buffer[1].data.sphere.diffuse = 0.3;
	l_object_buffer[1].data.sphere.transparency = 0.0;

	/* Raytrace the objects to the framebuffer. */
	framebuffer_render(&l_framebuffer, l_object_buffer, l_object_buffer_size);

	/* De-initialize the object buffer. */
	l_object_buffer_size = 0;
	free(l_object_buffer);
	l_object_buffer = NULL;

	/* Dump the framebuffer to a bitmap file. */
	FILE* l_file = fopen("output.bmp", "wb");
	if(l_file != NULL)
	{
		/* Dump the framebuffer to a bitmap file. */
		framebuffer_dump(&l_framebuffer, l_file);

		/* Now that we've wrote to the bitmap file, close it. */
		fclose(l_file);
	}
	else
	{
		/* Seems like we failed to open file for writing, throw an error! */
		fprintf(stderr, "error: Failed to open the bitmap file for writing!\n");

		/* Delete the framebuffer before exiting. */
		framebuffer_delete(&l_framebuffer);

		return -1;
	}

	/* Delete the framebuffer before exiting. */
	framebuffer_delete(&l_framebuffer);

	return 0;
}