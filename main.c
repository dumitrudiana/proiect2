#include "functions.h"

int main (int argc, char **argv) {
	image img;
	srand(time(0));

	if (argc != 3) {
		printf("Numele imaginii si filtrul / numarul de filtre trebuie date ca parametru.");
		exit(0);
	}

	char out[100] = "out_";
	strcat(out, argv[1]);
	int filters, filter;

    if (strcmp(argv[2], "invert") == 0) {
        filters = 1;
        filter = 0;
    } else if (strcmp(argv[2], "smooth") == 0) {
        filters = 1;
        filter = 1;
    } else if (strcmp(argv[2], "blur") == 0) {
        filters = 1;
        filter = 2;
    } else if (strcmp(argv[2], "sharpen") == 0) {
        filters = 1;
        filter = 3;
    } else if (strcmp(argv[2], "mean") == 0) {
        filters = 1;
        filter = 4;
    } else if (strcmp(argv[2], "emboss") == 0) {
        filters = 1;
        filter = 5;
    } else if (strcmp(argv[2], "grayscale") == 0) {
        filters = 1;
        filter = 6;
    } else if (strcmp(argv[2], "sepia") == 0) {
        filters = 1;
        filter = 7;
    } else {
        filters = atoi(argv[2]);
        filter = rand() % NUM_FILTERS;
    }

	read_file(argv[1], &img);

	float *matrix;
	float smooth[9], blur[9], sharpen[9], mean[9], emboss[9];
	getSmoothMatrix(smooth);
	getBlurMatrix(blur);
	getSharpenMatrix(sharpen);
	getMeanMatrix(mean);
	getEmbossMatrix(emboss);

	for (int i = 0; i < filters; i++) {
		switch(filter) {
			case 0:
				invert_pixels(&img);
				break;
			case 1:
				matrix = smooth;
				apply_filter(&img, matrix);
				break;
			case 2:
				matrix = blur;
				apply_filter(&img, matrix);
				break;
			case 3:
				matrix = sharpen;
				apply_filter(&img, matrix);
				break;
			case 4:
				matrix = mean;
				apply_filter(&img, matrix);
				break;
			case 5:
				matrix = emboss;
				apply_filter(&img, matrix);
				break;
			case 6:
			    grayscale(&img);
			    break;
			case 7:
			    sepia(&img);
			    break;
		}

		filter = rand() % NUM_FILTERS;
	}

	write_file(out, &img);

	return 0;
}