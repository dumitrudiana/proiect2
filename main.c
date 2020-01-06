#include "functions.h"

int main (int argc, char **argv) {
	image img;
	srand(time(0));

	if (argc != 3) {
		printf("Numele imaginii si numarul de filtre trebuie date ca parametru. Exemplu: ./serial file1.pnm 4\n");
		exit(0);
	}

	char out[100] = "out_";
	strcat(out, argv[1]);
	int filters = atoi(argv[2]);

	read_file(argv[1], &img);

	float *matrix;
	float smooth[9], blur[9], sharpen[9], mean[9], emboss[9];
	getSmoothMatrix(smooth);
	getBlurMatrix(blur);
	getSharpenMatrix(sharpen);
	getMeanMatrix(mean);
	getEmbossMatrix(emboss);

	for (int i = 0; i < filters; i++) {
		int filter = rand() % NUM_FILTERS;

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
		}
	}

	write_file(out, &img);

	return 0;
}