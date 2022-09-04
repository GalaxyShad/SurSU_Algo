#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 8

typedef int key_t;

typedef void (*fillArrFun_t)(key_t*, size_t, key_t, key_t, int);

int random_int(int min, int max);

void print_arr(const int* arr, const char* modifier, size_t n);
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

	key_t arr[N] = {1, 2, 3};

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

		functions[i](arr, N, 2, 7, 4);

		print_arr((const int*)arr, "%d ", N);
		printf("\n");
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int random_int(int min, int max) {
	return rand() % (max - min) + min;
	// return (rand() << 15 + rand()) % (max - min) + min;
}


void print_arr(const int* arr, const char* modifier, size_t n) {
	for (int i = 0; i < n; ++i) {
		printf(modifier, arr[i]);
		printf(" ");
	}

	printf("\n");
}


void fill_arr_random(key_t * arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	for (int i = 0; i < n; ++i) {
		arr[i] = random_int((int)min, (int)max); 
	}
}


void fill_arr_linear_upwards(key_t * arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	key_t delta = (max - min);

	for (int i = 0; i < n; ++i) {
		arr[i] = delta * ((double)i / (n-1)) + min;
	}
}


void fill_arr_linear_downwards(key_t * arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	key_t delta = (max - min);

	for (int i = n-1; i >= 0; i--) {
		arr[i] = delta * (((double)n - i) / n) + min;
	}
}


void fill_arr_sin(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	int delta = max - min;

	for (int i = 0; i < n; ++i) {
		arr[i] = sin(i) * delta + min;
	}
}


// https://studfile.net/preview/9690421/page:5/
void fill_arr_sawtooth(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	key_t amplitude = max - min;
	double period = 2;

	for (int i = 0; i < n; ++i) {
		double step = (double)i  / (n - 1);

		double sp = step / period;

		arr[i] = sp - fabs(sp);
	}
}


void fill_arr_stepped(key_t* arr, size_t n, key_t min, key_t max, int r) {

}


void fill_arr_quasi_ordered(key_t* arr, size_t n, key_t min, key_t max, int r) {

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

