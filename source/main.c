#include <stdio.h>

#include <cphoto/framebuffer.h>

int main(int argc, char* argv[])
{
	/* Declare the framebuffer handle. */
	framebuffer_t l_framebuffer;

	/* Initialize & re-size the framebuffer to a 1920x1080 resolution. */
	framebuffer_create(&l_framebuffer);
	framebuffer_resize(&l_framebuffer, 1920, 1080);

	/* Randomize the framebuffer's contents. */
	framebuffer_randomize(&l_framebuffer);

	/* Dump the framebuffer to a bitmap file. */
	FILE* l_file = fopen("output.bmp", "wb");
	if(l_file != NULL)
	{
		framebuffer_dump(&l_framebuffer, l_file);
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