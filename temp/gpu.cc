#include <stdio.h>
#include <stdlib.h>

int main() {
	const int N_elements{1000000};
	double* array = (double*) malloc(N_elements * sizeof(double));

	#pragma omp target teams distribute parallel for map(from:array[0:N_elements])
	for(int i = 0; i < N_elements; ++i) {
		array[i] = (double) i;
	}

	printf("array[42] = %f\n", array[42]);
	free(array);

	return 0;
}
