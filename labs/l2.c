#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
	
#define VAR_TYPE 		double		// тип данных для массива
#define MODIFIER 		"0.2lf"		// модификатор для вывода массива на экран 

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

typedef int (*cmpFun_t)(const void *, const void *);

// utils
int 	random_int(int min, int max);
double 	random_double(double min, double max);
void    swap(key_t* a, key_t* b);

// buisness logic
void 	fill_arr_random(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_linear_upwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_linear_downwards(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_sin(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_sawtooth(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_stepped(key_t* arr, size_t n, key_t min, key_t max, int r);
void 	fill_arr_quasi_ordered(key_t* arr, size_t n, key_t min, key_t max, int r);

void    sort_arr_bubble  (key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction);
void    sort_arr_bubble_m(key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction);

void    sort_arr_quick  (key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction);
void    sort_arr_quick_m(key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction);

int    comp_greater(const void* left, const void* right) {
    return *((key_t*)left) > *((key_t*)right);
}

int    comp_less(const void* left, const void* right) {
    return *((key_t*)left) < *((key_t*)right);
}


void swap(key_t* a, key_t* b) {
    key_t t = *a;
    *a = *b;
    *b = t;
}

void sort_arr_bubble (key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction) {
    if (!compareFunction) compareFunction = comp_greater;
    
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (compareFunction(&arr[j], &arr[j+1]))
                swap(&arr[j], &arr[j + 1]);
}


void sort_arr_bubble_m(key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction) {
    
    if (!compareFunction) compareFunction = comp_greater;
    
    int i, j, low = 0, indexOfChanged;
    while (low < n - 1) {
        for (indexOfChanged = j = n - 1 ; j > low; j--)
            if (compareFunction(&arr[j-1], &arr[j])) {
                swap(&arr[j], &arr[j-1]);
                indexOfChanged = j;
            }
        low = indexOfChanged;
    }
}

static int _quick_sort_partition(key_t* arr, int low, int high) {
    key_t value = arr[high];

    int i = low - 1, j = high;
    for (;;)
    {
        while (value > arr[++i]);
        while (value < arr[--j]) if (j == low) break;
        
        if (i >= j) break;
        swap(&arr[i], &arr[j]);
    }
    swap(&arr[high], &arr[i]);
    return i;
}

static void _quick_sort_range(key_t* arr, int low, int high) {
    int PartitionIndex;
    if (high - low <= 0)
        return;

    PartitionIndex = Partition(arr, low, high);
    Quick(A, Low, PartitionIndex - 1);
    Quick(A, PartitionIndex + 1, High);
}

void sort_arr_quick(key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction) {
    _quick_sort_range(arr, 0, n-1);
}

void sort_arr_quick_m(key_t* arr, size_t element_size, size_t n, cmpFun_t compareFunction) {

}


void print_arr(const key_t* arr, size_t n) {
    for (int i = 0; i < n; i++) printf("%"MODIFIER" ", arr[i]);
    printf("\n");
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef void (*sortFunc_t) (key_t*, size_t, size_t, cmpFun_t);

typedef struct sortFuncCell {
    char        name[32];
    sortFunc_t  invoke;
} sortFuncCell_t;

int main() {
	srand(time(NULL));
	system("chcp 1251 > nul");

	key_t arr[N];
    fill_arr_random(arr, N, 0, N, 0);
    print_arr(arr, N);
    printf("\n");

    sortFuncCell_t sortFuncs[4] = {
        {"Пузырек",         sort_arr_bubble},
        {"Пузырек Мод",     sort_arr_bubble_m},

        {"Быстрая",         sort_arr_quick},
        {"Быстрая Мод",     sort_arr_quick_m},
    };

    for (int i = 0; i < 4; i++) {
        key_t* newArr = malloc(sizeof (key_t) * N);
        memcpy(newArr, arr, sizeof(key_t) * N);

        sortFuncs[i].invoke(newArr, sizeof(key_t), N, NULL);
        printf("%s\n", sortFuncs[i].name);
        print_arr(newArr, N);

        free(newArr);
    }

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

