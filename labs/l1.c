#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 				16		
#define VAR_TYPE 		int		
#define MODIFIER 		"d"		

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef VAR_TYPE key_t;

typedef void (*fillArrFun_t)(key_t*, size_t, key_t, key_t, int);

// utils
int random_int(int min, int max);
double random_double(double min, double max);
double lerp(double start, double end, double val);

// buisness logic
void fill_arr_random(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_linear_upwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_linear_downwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_sin(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_sawtooth(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_stepped(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_quasi_ordered(key_t* arr, size_t n, key_t min, key_t max, int r);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int main() {
	srand(time(NULL));

	key_t arr[N];

	const fillArrFun_t functions[] = {
		fill_arr_random,
		fill_arr_linear_upwards,
		fill_arr_linear_downwards,
		fill_arr_sin,
		fill_arr_sawtooth,
		fill_arr_stepped,
		fill_arr_quasi_ordered
	};

	const char functionNames[][26] = {
		"fill_arr_random",
		"fill_arr_linear_upwards",
		"fill_arr_linear_downwards",
		"fill_arr_sin",
		"fill_arr_sawtooth",
		"fill_arr_stepped",
		"fill_arr_quasi_ordered"
	};

	for (int i = 0; i < 7; ++i) {
		printf("Function: %s\n", functionNames[i]);

		functions[i](arr, N, 2, 8, 5);

		for (int j = 0; j < N; j++) printf("%"MODIFIER" ", arr[j]);

		printf("\n");
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

double lerp(double start, double end, double val) {
	return start + val * (end - start);
}


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
		//arr[i] = random_int((int)min, (int)max); 
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
	double period = 0.25;

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
		arr[i] = delta * (floor( (i / (double)(n-1)) * period) / period) + min;
	}
}


void fill_arr_quasi_ordered(key_t* arr, size_t n, key_t min, key_t max, int r) {

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

