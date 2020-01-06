#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define invert(x) 255 - x
#define NUM_FILTERS 6
#define KERNEL 9

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} pixel;

typedef struct {
	int height;
	int width;
	pixel *matrix;
} image;

void read_file(char *filename, image *img);
void write_file(char *filename, image *img);
void invert_pixels(image *img);
void grayscale(image *img);
void apply_filter(image *img, float *matrix);
void getSmoothMatrix (float matrix[9]);
void getBlurMatrix (float matrix[9]);
void getSharpenMatrix (float matrix[9]);
void getMeanMatrix (float matrix[9]);
void getEmbossMatrix (float matrix[9]);