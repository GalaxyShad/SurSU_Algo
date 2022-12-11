#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
	
#define VAR_TYPE 		int		// тип данных для массива
#define MODIFIER 		"d"		// модификатор для вывода массива на экран 

#define RANGE_LOW   	500000		// нижняя граница количества эл-тов массива
#define RANGE_HIGH		5000000		// верхняя граница количества эл-тов массива
#define RANGE_STEP		500000		// шаг изменения элементов массива

#define INVOKES_COUNT	10			// кол-во вызовов функции заполнения
		   		 
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define N 				16			// кол-во эдлементов для контроля ф-ций
#define FUNCTIONS_COUNT	7			// кол-во функций заполнения

//////////////////////////////////////////////////////////////////////////////

typedef VAR_TYPE key_t;

typedef void (*fillArrFun_t)(key_t*, size_t, key_t, key_t, int);

typedef struct ftableCell {
	char name[62];
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
	system("chcp 1251 > nul");

	key_t arr[N];

	const ftableCell_t functionsTable[FUNCTIONS_COUNT] = {
		{ "Случайная", 							fill_arr_random 			},
		{ "Упорядоченная", 						fill_arr_linear_upwards 	},
		{ "Упорядоченная в обратном порядке", 	fill_arr_linear_downwards 	},

		{ "Пилообразная", 						fill_arr_sawtooth 			},
		{ "Синусоидальная", 					fill_arr_sin 				},
		{ "Ступенчатая", 						fill_arr_stepped 			},
		{ "Квази-упорядоченная", 				fill_arr_quasi_ordered 		}
	};

	for (int i = 0; i < FUNCTIONS_COUNT; ++i) {
		printf("Function: %s\n", functionsTable[i].name);

		functionsTable[i].invoke(arr, N, 2, 8, 5);

		for (int j = 0; j < FUNCTIONS_COUNT; j++) printf("%"MODIFIER" ", arr[j]);

		printf("\n");
	}

	printf("\n");

	///////////////////////////////////////////////////////////////////////////

	DWORD t0, t1, tdelta;

	// вывод в csv файл, для перставления данных в виде таблицы
	FILE* csv_file = fopen("out.csv", "w");
	if (csv_file == NULL) printf("\a[ERR] Cannot open a file!\n");

	for (int n = RANGE_LOW; n <= RANGE_HIGH; n += RANGE_STEP) 
		fprintf(csv_file, ";%d", n);
	fprintf(csv_file, "\n");

	for (int i = 0; i < FUNCTIONS_COUNT; i++) {
		printf("Function: %s\n"
			   "Time delta: ", functionsTable[i].name);

		fprintf(csv_file, "%s;", functionsTable[i].name);

		for (size_t n = RANGE_LOW; n <= RANGE_HIGH; n += RANGE_STEP) {
			key_t* test_arr = (key_t*)malloc(sizeof(key_t) * n);

			tdelta = 0;
			for (int j = 0; j < INVOKES_COUNT; j++) {
				t0 = GetTickCount();
				functionsTable[i].invoke(test_arr, n, 2, 8, 5);
				t1 = GetTickCount();

				tdelta += t1 - t0;
			}
			
			free(test_arr); 

			fprintf(csv_file, "%d;", tdelta / INVOKES_COUNT);
			printf("%d ", tdelta / INVOKES_COUNT);
		}

		fprintf(csv_file, "\n");
		printf("\n");
	}

	fclose(csv_file);
	printf("end");

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int random_int(int min, int max) {
	return rand() % (max - min) + min;
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
		arr[i] = delta * ((int)( (i / (double)(n-1)) * period) / period) + 
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

