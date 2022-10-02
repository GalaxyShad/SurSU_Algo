#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
	
#define VAR_TYPE 		double		// тип данных для массива
#define MODIFIER 		"0.2lf"		// модификатор для вывода массива на экран 

#define INVOKES_COUNT	5			// кол-во вызовов функции заполнения
		   		 
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef VAR_TYPE key_t;

typedef void (*fillArrFun_t)(key_t*, size_t, key_t, key_t, int);

typedef struct ftableCell {
	char name[62];
	fillArrFun_t invoke;
} ftableCell_t;

typedef int (*cmpFun_t)(const void *, const void *);

typedef void (*sortFunc_t) (void*, size_t, size_t, cmpFun_t);

typedef struct sortFuncCell {
    char        name[32];
    sortFunc_t  invoke;
    size_t      rangeMin;
    size_t      rangeMax;
    size_t      rangeStep;
} sortFuncCell_t;

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


DWORD     get_tick_count()   { return GetTickCount(); }


static uint64_t s_compCount = 0;
uint64_t  get_comp_count()   { return s_compCount;    }
void      reset_comp_count() { s_compCount = 0;       }


int     random_int(int min, int max) { return rand() % (max - min) + min; }
double  random_double(double min, double max) { return ((double)rand() / RAND_MAX) * (max - min) + min; }


void print_arr(const key_t* arr, size_t n) {
    for (int i = 0; i < n; i++) printf("%"MODIFIER" ", arr[i]);
    printf("\n");
}

void printf_ex(FILE* file, const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    va_start(ap, fmt);
    vfprintf(file, fmt, ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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


void fill_arr_sawtooth(key_t* arr, size_t n, key_t min, key_t max, int r) {
	if (!arr) return;
	if (r == 0) return;

	double delta = max - min;

	for (int i = 0; i < n; ++i) {
		arr[i] = ((double)(i % (r+1)) / r) * delta + min;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int comp_greater(const void* left, const void* right) {
    s_compCount++;
    return (*((key_t*)left) > *((key_t*)right)) ? 1 : -1;
}


int comp_less(const void* left, const void* right) {
    s_compCount++;
    return *((key_t*)left) < *((key_t*)right);
}


void swap(void* a, void* b, size_t size) {
    uint8_t temp, *pa=a, *pb=b; 
    for (size_t i = 0; i < size; i++) {
        temp = pa[i]; pa[i] = pb[i]; pb[i] = temp;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void sort_arr_bubble (void* arr, size_t n, size_t size, cmpFun_t compareFunction) {
    if (!compareFunction) compareFunction = comp_greater;

    uint8_t* barr = arr;
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++) {
            if (compareFunction(&barr[j*size], &barr[(j+1)*size]) > 0)
                swap(&barr[j*size], &barr[(j+1)*size], size);
        }
}

/////////////////////////////////////////////////////////////////////////////////////////

void sort_arr_bubble_m(void* arr, size_t n, size_t size, cmpFun_t compareFunction) {
    
    if (!compareFunction) compareFunction = comp_greater;
    
    uint8_t* barr = arr;
    int i, j, low = 0, indexOfChanged;
    while (low < n - 1) {
        for (indexOfChanged = j = n - 1 ; j > low; j--)
            if (compareFunction(&barr[(j-1)*size], &barr[j*size]) > 0) {
                swap(&barr[j*size], &barr[(j-1)*size], size);
                indexOfChanged = j;
            }
        low = indexOfChanged;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

static int _quick_sort_partition(void* arr, int low, int high, size_t size, cmpFun_t compareFunction) {
    uint8_t* barr = arr;
    // uint8_t* value = barr + (low + (int)arr % (high-low)) * size;
    uint8_t* value = barr + ((high + low) / 2) * size;

    int i = low - 1, j = high;
    for (;;)
    {
        while (compareFunction(value, &barr[(++i)*size]) > 0);
        while (compareFunction(value, &barr[(--j)*size]) < 0) if (j == low) break;
        
        if (i >= j) break;

        swap(&barr[i*size], &barr[j*size], size);
    }
    swap(&barr[high*size], &barr[i*size], size);
    return i;
}


static void _quick_sort_range(void* arr, int low, int high, size_t size, cmpFun_t compareFunction) {
    int partitionIndex;
    if (high - low <= 0)
        return;

    partitionIndex = _quick_sort_partition(arr, low, high, size, compareFunction);
    _quick_sort_range(arr, low, partitionIndex - 1, size, compareFunction);
    _quick_sort_range(arr, partitionIndex + 1, high, size, compareFunction);
}


void sort_arr_quick(void* arr, size_t n, size_t size, cmpFun_t compareFunction) {
    _quick_sort_range(arr, 0, n-1, size, compareFunction);
}

/////////////////////////////////////////////////////////////////////////////////////////

void sort_arr_insertion(void* arr, size_t n, size_t size, cmpFun_t compareFunction) {
    int i, j;
    uint8_t* barr = arr;
    uint8_t* temp = (uint8_t*)malloc(size);

    for (i = 1; i < n; i++) {
        memcpy(temp, barr + i*size, size);
        for (j = i; j > 0 && compareFunction(&barr[(j-1)*size], temp) > 0; j--) 
            memcpy(barr + j*size, barr + (j-1)*size, size);
        memcpy(barr + j*size, temp, size);
    }

    free(temp);
}

/////////////////////////////////////////////////////////////////////////////////////////

static void _quick_sort_modified_range(void* arr, int low, int high, size_t size, cmpFun_t compareFunction) {
    int partitionIndex;
    if (high - low <= 0)
        return;

    if (high - low < 10) {
        sort_arr_insertion(((uint8_t*)arr + low * size), high - low + 1, size, compareFunction);
        return;
    }

    partitionIndex = _quick_sort_partition(arr, low, high, size, compareFunction);
    _quick_sort_modified_range(arr, low, partitionIndex - 1, size, compareFunction);
    _quick_sort_modified_range(arr, partitionIndex + 1, high, size, compareFunction);
}


void sort_arr_quick_m(void* arr, size_t n, size_t size, cmpFun_t compareFunction) {
    _quick_sort_modified_range(arr, 0, n-1, size, compareFunction);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define N 8
int main() {
	srand(time(NULL));
	system("chcp 1251 > nul");

    const sortFuncCell_t sortFuncs[] = {
        {"Быстрая",         sort_arr_quick,    500000, 5000000, 500000},
        {"Быстрая Мод",     sort_arr_quick_m,  500000, 5000000, 500000},
        {"std qsort",       qsort,             500000, 5000000, 500000},

        {"Пузырек Мод",     sort_arr_bubble_m, 5000, 14000, 1000},
        {"Пузырек",         sort_arr_bubble,   5000, 14000, 1000},

        // {"Вставками",       sort_arr_insertion,},
    };
    const int sortFuncsCount = sizeof(sortFuncs) / sizeof(*sortFuncs);

    const ftableCell_t fillFuncs[] = {
		// { "Пилообразная", 						fill_arr_sawtooth 			},
        { "Случайная", 							fill_arr_random 			},
		{ "Упорядоченная", 						fill_arr_linear_upwards 	},
		{ "Упорядоченная в обратном порядке", 	fill_arr_linear_downwards 	},

    };
    const int fillFuncsCount = sizeof(fillFuncs) / sizeof(*fillFuncs);

    FILE* csvFile = fopen("out.csv", "w"); 
    if (!csvFile) { printf("can't open csv :("); return 1; }

    FILE* out = csvFile;

    for (int i = 0; i < sortFuncsCount; i++) {
        sortFuncCell_t fsort = sortFuncs[i];  

        printf_ex(out, "%s;=;=;=;=;=;=;=;=;=;=;=;=;\n", fsort.name);

        printf_ex(out, "\n");

        for (int j = 0; j < fillFuncsCount; j++) {
            ftableCell_t ffill = fillFuncs[j];

            printf_ex(out, "%s;\n", ffill.name);
            printf_ex(out, "n;T; ;n;S;\n");

            // Time Delta
            for (int n =  fsort.rangeMin; 
                     n <= fsort.rangeMax; 
                     n += fsort.rangeStep
            ) {
                key_t* arr = malloc(sizeof (key_t) * n);

                ffill.invoke(arr, n, 0, n, 9);

                DWORD delta = 0;
                uint64_t counts = 0;
                for (int k = 0; k < INVOKES_COUNT; k++) {   
                    DWORD t1 = get_tick_count();
                    reset_comp_count();
                    fsort.invoke(arr, n, sizeof(key_t), comp_greater);

                    delta  += get_tick_count() - t1;
                    counts += get_comp_count();
                }

                delta  /= INVOKES_COUNT;
                counts /= INVOKES_COUNT;

                printf_ex(out, "%d;%d; ;%d;%d;\n", n, delta, n, counts);
  
                free(arr);
            }

            printf_ex(out, "\n");
        }
        printf_ex(out, "\n");
    }

    printf("[LOG] Done");

	return 0;
}


