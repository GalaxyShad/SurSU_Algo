#include <stdio.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int* arr, int low, int high) {

    int value = arr[high];

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

int main () {
    int arr[10] = {8, 6, 5, 4, 3, 2, 9, 1, 7, 0};

    for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    printf("\n");

    printf("%d\n", partition(arr, 1, 9));

    for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);

    return 0;

}