#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

typedef int key_t;

int random_int(int min, int max);

void fill_arr_random(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_linear_upwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_linear_downwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_sin(key_t* arr, size_t n, key_t min, key_t max, int r);
void fill_arr_unk(key_t* arr, size_t n, key_t min, key_t max, int r);

void print_arr_int(const int* arr, size_t n);

///////////////////////////////////////////////////////////////////
int random_int(int min, int max) {
	return (rand() << 15 + rand()) % (max - min) + min;
}


void print_arr_int(const int* arr, size_t n) {
	for (int i = 0; i < n; ++i) {
		printf("%d ", arr[i]);
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


void fill_arr_unk(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;

	int delta = max - min;

	for (int i = 0; i < n; ++i) {
		arr[i] = (min + delta * ((double)i / n)) % max;
	}
}


#define n 30

int main() {
	srand(time(NULL));

	
	key_t arr[n] = {0};

	fill_arr_random(arr, n, 2, 12, 0);
	print_arr_int((const int*)arr, n);

	fill_arr_linear_upwards(arr, n, 3, 12, 0);
	print_arr_int((const int*)arr, n);

	fill_arr_linear_downwards(arr, n, 3, 12, 0);
	print_arr_int((const int*)arr, n);

	fill_arr_sin(arr, n, 3, 12, 0);
	print_arr_int((const int*)arr, n);

	fill_arr_unk(arr, n, 3, 12, 0);
	print_arr_int((const int*)arr, n);

	return 0;
}