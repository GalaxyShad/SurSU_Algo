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

#define INVOKES_COUNT	10			// кол-во вызовов функции заполнения
		   		 
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
} sortFuncCell_t;




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



///////////////////////////////////////////////////////////////////////////////////////

static uint64_t s_compCount = 0;

DWORD get_tick_count()   { return GetTickCount(); }

uint64_t get_comp_count()   { return s_compCount;    }
void     reset_comp_count() { s_compCount = 0;       }


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

static int _quick_sort_partition(void* arr, int low, int high, size_t size, cmpFun_t compareFunction) {
    uint8_t* barr = arr;
    uint8_t* value = barr + (low + (int)arr % (high-low)) * size;

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


void print_arr(const key_t* arr, size_t n) {
    for (int i = 0; i < n; i++) printf("%"MODIFIER" ", arr[i]);
    printf("\n");
}


void test_sort_perfomance(const key_t* srcarr, size_t n, sortFuncCell_t cellsort, 
                          uint32_t* outTimedelta, uint64_t* outCompdelta
) {
    key_t* arr = malloc(sizeof (key_t) * n);
    memcpy(arr, srcarr, sizeof(key_t) * n);
    
    DWORD t1 = get_tick_count();
    reset_comp_count();

    cellsort.invoke(arr, n, sizeof(key_t), comp_greater);

    *outCompdelta = get_comp_count();
    *outTimedelta = get_tick_count() - t1;

    free(arr);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


// ?
// Для каждого алгоритма сортировки гененрировать различные последовательности, 
// или тест должен проводиться на одной созданной?

#define N 8
int main() {
	srand(time(NULL));
	system("chcp 1251 > nul");

    const sortFuncCell_t sortFuncs[] = {
        {"std qsort",       qsort,             },
        {"Быстрая",         sort_arr_quick,    },
        {"Быстрая Мод",     sort_arr_quick_m,  },
        // {"Вставками",       sort_arr_insertion,},
        {"Пузырек",         sort_arr_bubble,   },
        {"Пузырек Мод",     sort_arr_bubble_m, },
    };
    const int sortFuncsCount = sizeof(sortFuncs) / sizeof(*sortFuncs);

    const ftableCell_t fillFuncs[] = {
        { "Случайная", 							fill_arr_random 			},
		{ "Упорядоченная", 						fill_arr_linear_upwards 	},
		{ "Упорядоченная в обратном порядке", 	fill_arr_linear_downwards 	},

		{ "Пилообразная", 						fill_arr_sawtooth 			},
    };
    const int fillFuncsCount = sizeof(fillFuncs) / sizeof(*fillFuncs);

    //FILE* out = stdout;

    const size_t rangeMin  = 500;
    const size_t rangeMax  = rangeMin*1000;
    const size_t rangeStep = rangeMin;

    for (int i = 0; i < fillFuncsCount; i++) {
        ftableCell_t ffill = fillFuncs[i];
        printf("%s\n", ffill.name);

        size_t outTableColums   = (rangeMax - rangeMin) / rangeStep + 1;
        size_t outTableRows     = sortFuncsCount;

        uint32_t* outTableDelta = malloc(outTableColums * outTableRows * sizeof(uint32_t));
        uint64_t* outTableCmp   = malloc(outTableColums * outTableRows * sizeof(uint64_t));

        for (int n =  rangeMin; n <= rangeMax; n += rangeStep) {
            key_t* arr = malloc(sizeof(key_t) * n);
            ffill.invoke(arr, n, 0, n, 9);

            printf("%d\n", n);

            for (int j = 0; j < sortFuncsCount; j++) {
                sortFuncCell_t fsort = sortFuncs[j];  

                uint32_t timedelta; uint64_t compdelta;
                test_sort_perfomance(arr, n, fsort, &timedelta, &compdelta);

                printf("%s %d %lld\n", fsort.name, timedelta, compdelta);

                int row = j;
                int col = (n - rangeMin) / rangeStep;
                int index = row *outTableColums + col;

                outTableDelta[index] = timedelta;
                outTableCmp[index]   = compdelta;
            }

            free(arr);
        }

        char buff[64];
        sprintf(buff, "%s.csv", ffill.name);
        FILE* csvFile = fopen(buff, "w"); 
        if (!csvFile) { printf("can't open csv :("); return 1; }

        fprintf(csvFile, "%s;\n", ffill.name);
        for (int k = 0; k < 2; k++) {
            for (int n =  rangeMin; n <= rangeMax; n += rangeStep) {
                fprintf(csvFile, ";%d", n);
                printf(";%d", n);
            }
            printf("\n");
            fprintf(csvFile, "\n");

            for (int r = 0; r < outTableRows; r++) {
                printf("%s;", sortFuncs[r]);
                fprintf(csvFile, "%s;", sortFuncs[r]);
                for (int c = 0; c < outTableColums; c++) {
                    int index = r *outTableColums + c;
                    printf("%d;", k == 0 ? outTableDelta[index] : 
                                           outTableCmp[index]);

                    fprintf(csvFile, "%d;", k == 0 ? outTableDelta[index] : 
                                                     outTableCmp[index]);
                }
                printf("\n");
                fprintf(csvFile, "\n");
            }

            printf("\n\n");
            fprintf(csvFile, "\n\n");
        }
        fclose(csvFile);

        free(outTableDelta);
        free(outTableCmp);
    }


    printf("[LOG] Done");

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

