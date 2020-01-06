#include "functions.h"

void read_file(char *filename, image *img) {
	FILE *fin = fopen(filename, "r");

	if (fin == NULL) {
		printf("Imaginea nu exista.\n");
		exit(0);
	}

	fscanf(fin, "P6\n%d %d\n255\n", &(img->height), &(img->width));
	img->matrix = (pixel*)malloc(img->height * img->width * sizeof(pixel));
	fread(img->matrix, sizeof(pixel), img->height * img->width, fin);

	fclose(fin);
}

void write_file(char *filename, image *img) {
	FILE *fout = fopen(filename, "w");

	fprintf(fout, "P6\n%d %d\n255\n", img->height, img->width);
	fwrite(img->matrix, sizeof(pixel), img->height * img->width, fout);

	free(img->matrix);
	fclose(fout);
}

void invert_pixels(image *img) {
	for (int i = 0; i < img->height * img->width; i++) {
		img->matrix[i].r = invert(img->matrix[i].r);
		img->matrix[i].g = invert(img->matrix[i].g);
		img->matrix[i].b = invert(img->matrix[i].b);
	}
}

void grayscale(image *img) {
    for (int i = 0; i < img->height * img->width; i++) {
        img->matrix[i].r = (img->matrix[i].r + img->matrix[i].g + img->matrix[i].b)/3;
        img->matrix[i].g = img->matrix[i].r;
        img->matrix[i].b = img->matrix[i].r;
    }
}

void sepia(image *img) {
    for (int i = 0; i < img->height * img->width; i++) {
        int red = (img->matrix[i].r * 0.393 + img->matrix[i].g * 0.769 + img->matrix[i].b * 0.189);
        int green = (img->matrix[i].r * 0.349 + img->matrix[i].g * 0.686 + img->matrix[i].b * 0.168);
        int blue = (img->matrix[i].r * 0.272 + img->matrix[i].g * 0.534 + img->matrix[i].b * 0.131);

        if (red > 255)
            img->matrix[i].r = 255;
        else
            img->matrix[i].r = red;

        if (green > 255)
            img->matrix[i].g = 255;
        else
            img->matrix[i].g = green;

        if (blue > 255)
            img->matrix[i].b = 255;
        else
            img->matrix[i].b = blue;
    }
}

void sketch (image *img, float matrix[KERNEL], int color) {
    pixel *mat = (pixel*)malloc(img->height * img->width * sizeof(pixel));

    if (color == 0)
        grayscale(img);

    for (int i = 0; i < img->height * img->width; i++) {
         mat[i].r = img->matrix[i].r;
         mat[i].g = img->matrix[i].g;
         mat[i].b = img->matrix[i].b;
    }
    invert_pixels(img);
    apply_filter(img, matrix);

    for(int i = 0; i < img->height * img->width; i++) {
        if (mat[i].r == 255) {
            img->matrix[i].r = 255;
        } else {
            int red = 255 * img->matrix[i].r / (255 - mat[i].r);
            img->matrix[i].r = (red > 255) ? 255 : red;
        }

        if (mat[i].g == 255) {
            img->matrix[i].g = 255;
        } else {
            int green = 255 * img->matrix[i].g / (255 - mat[i].g);
            img->matrix[i].g = (green > 255) ? 255 : green;
        }

        if (mat[i].b == 255) {
            img->matrix[i].b = 255;
        } else {
            int blue = 255 * img->matrix[i].b / (255 - mat[i].b);
            img->matrix[i].b = (blue > 255) ? 255 : blue;
        }
    }
}

void apply_filter(image *img, float *matrix) {
	pixel *mat = (pixel*)malloc(img->height * img->width * sizeof(pixel));

	for (int i = 0; i < img->height * img->width; i++) {
		if (i % img->width == 0 || i % img->width == img->width - 1 ||
			i < img->width || i > (img->height-1) * img->width) {
			mat[i] = img->matrix[i];
		} else {
			float sumr = 0, sumg = 0, sumb = 0;
			for (int j = 0; j < KERNEL; j++) {
			    int index = i + img->width * (j/3 - 1) + (j%3 - 1);
				sumr += matrix[j] * img->matrix[index].r;
				sumg += matrix[j] * img->matrix[index].g;
				sumb += matrix[j] * img->matrix[index].b;
			}

			mat[i].r = (unsigned char)sumr;
			mat[i].g = (unsigned char)sumg;
			mat[i].b = (unsigned char)sumb;
		}
	}

	free(img->matrix);
	img->matrix = mat;
}

void getSmoothMatrix (float matrix[KERNEL]) {
	float smooth = 1.0f / 9;
	for (int i = 0; i < KERNEL; i++)
		matrix[i] = smooth;
}

void getBlurMatrix (float matrix[KERNEL]) {
	float blur = 1.0f / 16;

	matrix[0] = blur;
	matrix[1] = 2 * blur;
	matrix[2] = blur;
	matrix[3] = 2 * blur;
	matrix[4] = 4 * blur;
	matrix[5] = 2 * blur;
	matrix[6] = blur;
	matrix[7] = 2 * blur;
	matrix[8] = blur;
}

void getSharpenMatrix (float matrix[KERNEL]) {
	float sharpen = 1.0f / 3;

	matrix[0] = 0;
	matrix[1] = (-2) * sharpen;
	matrix[2] = 0;
	matrix[3] = (-2) * sharpen;
	matrix[4] = 11 * sharpen;
	matrix[5] = (-2) * sharpen;
	matrix[6] = 0;
	matrix[7] = (-2) * sharpen;
	matrix[8] = 0;
}

void getMeanMatrix (float matrix[KERNEL]) {
	for (int i = 0; i < KERNEL; i++)
		matrix[i] = -1;

	matrix[4] = 9;
}

void getEmbossMatrix (float matrix[KERNEL]) {
	for (int i = 0; i < KERNEL; i++)
		matrix[i] = 0;

	matrix[1] = 1;
	matrix[7] = -1;
}