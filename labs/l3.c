#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

typedef int key_t;

int findDirect(key_t* arr, size_t len, key_t value) {
    for (int i = 0; i < len; i++) if (arr[i] == value) return i;
    return -1;
}

int findJump(key_t* arr, size_t len, key_t value) {
    int arrayLength = len;
    int jumpStep = (int)sqrt(len);
    int previousStep = 0;

    while (arr[min(jumpStep, arrayLength) - 1] < value) {
        previousStep = jumpStep;
        jumpStep += (int)(sqrt(arrayLength));
        if (previousStep >= arrayLength) return -1;
    }

    while (arr[previousStep] < value) {
        previousStep++;
        if (previousStep == min(jumpStep, arrayLength)) return -1;
    }

    if (arr[previousStep] == value)
        return previousStep;

    return -1;
}

int findBinary(key_t* arr, size_t len, key_t value) {
    int first = 0;
    int last  = len - 1;

    while (first <= last) {
        int mid = (first + last) / 2;

        if      (arr[mid] == value)  return mid;
        else if (arr[mid] < value)   first = mid + 1;
        else if (arr[mid] > value)   last  = mid - 1;

    }

    return -1;
}


int main() {
    key_t arr[] = {0, 3, 5, 7, 10, 15, 16, 17, 18};
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    printf("%d\n", findDirect(arr, arrSize, 7));
    printf("%d\n", findJump(arr, arrSize, 7));
    printf("%d\n", findBinary(arr, arrSize, 7));

    return 0;
}