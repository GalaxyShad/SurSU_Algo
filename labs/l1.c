#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

#define N 				16		
#define VAR_TYPE 		double		
#define MODIFIER 		"0.2lf"	

#define N_MAX			1000000
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef VAR_TYPE key_t;

typedef void (*fillArrFun_t)(key_t*, size_t, key_t, key_t, int);

typedef struct ftableCell {
	char name[26];
	fillArrFun_t invoke;
} ftableCell_t;

// utils
int 	random_int(int min, int max);
double 	random_double(double min, double max);

// buisness logic
void 	fill_arr_random(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_linear_upwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_linear_downwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_sin(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_sawtooth(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_stepped(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_quasi_ordered(key_t* arr, size_t n, key_t min, key_t max, int r);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int main() {
	srand(time(NULL));

	key_t arr[N];

	const ftableCell_t functionsTable[] = {
		{ "fill_arr_random", 			fill_arr_random 			},
		{ "fill_arr_linear_upwards", 	fill_arr_linear_upwards 	},
		{ "fill_arr_linear_downwards", 	fill_arr_linear_downwards 	},
		{ "fill_arr_sin", 				fill_arr_sin 				},
		{ "fill_arr_sawtooth", 			fill_arr_sawtooth 			},
		{ "fill_arr_stepped", 			fill_arr_stepped 			},
		{ "fill_arr_quasi_ordered", 	fill_arr_quasi_ordered 		}
	};

	for (int i = 0; i < 7; ++i) {
		printf("Function: %s\n", functionsTable[i].name);

		functionsTable[i].invoke(arr, N, 2, 8, 5);

		for (int j = 0; j < N; j++) printf("%"MODIFIER" ", arr[j]);

		printf("\n");
	}

	printf("\n");
	// return 0;

	///////////////////////////////////////////////////////////////////////////

	DWORD  t0, t1, tdelta;
	size_t range_low  = 500000, 
		   range_high = 5000000, 
		   range_step = 500000;

	FILE* csv_file = fopen("out.csv", "w");

	for (int i = 0; i < 7; i++) {
		printf("Function: %s\n"
			   "Time delta: ", functionsTable[i].name);

		fprintf(csv_file, "\nFucntion;%s\n", functionsTable[i].name);

		for (int n = range_low; n <= range_high; n += range_step) {
			key_t* test_arr = (key_t*)malloc(sizeof(key_t) * n);

			t0 = GetTickCount();
			functionsTable[i].invoke(test_arr, n, 2, 8, 5);
			t1 = GetTickCount();
			
			tdelta = t1 - t0;

			free(test_arr); 

			fprintf(csv_file, "%d;%d\n", n, tdelta);
			printf("%d ", tdelta);
		}

		printf("\n");
	}

	fclose(csv_file);
	

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int random_int(int min, int max) {
	return rand() % (max - min) + min;
	// return (rand() << 15 + rand()) % (max - min) + min;
}

double random_double(double min, double max) {
	return ((double)rand() / RAND_MAX) * (max - min) + min;
}


void fill_arr_random(key_t * arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	for (int i = 0; i < n; ++i) {
		arr[i] = (key_t)random_double(min, max); 
	}
}


void fill_arr_linear_upwards(key_t * arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	key_t delta = (max - min);

	for (int i = 0; i < n; ++i) {
		arr[i] = delta * (i / (double)(n-1)) + min;
	}
}


void fill_arr_linear_downwards(key_t * arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	key_t delta = (max - min);

	for (int i = 0; i < n; i++) {
		arr[i] = delta * ((double)(n - 1 - i) / (n-1)) + min;
	}
}


void fill_arr_sin(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;
	if (r == 0) return;

	double delta = max - min;
	double amplitude = delta / 2.0;

	for (int i = 0; i < n; ++i) {
		arr[i] = cos((i * M_PI * 2) / r) * amplitude + min + amplitude;
	}
}


// https://studfile.net/preview/9690421/page:5/
void fill_arr_sawtooth(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;
	if (r == 0) return;

	double delta = max - min;

	for (int i = 0; i < n; ++i) {
		arr[i] = ((double)(i % (r+1)) / r) * delta + min;
	}
}


void fill_arr_stepped(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;
	if (r == 0) return;

	double delta = max - min;
	double period = n / r;

	for (int i = 0; i < n; ++i) {
		arr[i] = delta * (floor( (i / (double)(n-1)) * period) / period) + 
				 random_double(0.0, period / 4)
				 + min;
	}
}


void fill_arr_quasi_ordered(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	key_t delta = (max - min);

	for (int i = 0; i < n; ++i) {
		arr[i] = delta * (i / (double)(n-1) + random_double(0.0, 0.4) - 0.2 ) + min;
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

