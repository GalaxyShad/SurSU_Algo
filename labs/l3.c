#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <stdint.h>

// #define max(a,b) (((a) > (b)) ? (a) : (b))
// #define min(a,b) (((a) < (b)) ? (a) : (b))

typedef int key_t;

static uint64_t cmp = 0;

int findDirect(key_t* arr, size_t len, key_t value) {
    for (int i = 0; i < len; i++) { 
        if (arr[i] == value) return i;
        cmp++;
    }
    return -1;
}

int findJump(key_t* arr, size_t len, key_t value) {
    int arrayLength = len;

    int jumpLen = (int)sqrt(len);

    int jumpStep = jumpLen;
    int previousStep = 0;

    cmp++;
    while (arr[min(jumpStep, arrayLength) - 1] < value) {
        previousStep = jumpStep;
        jumpStep += jumpLen;
        
        cmp++;
        if (previousStep >= arrayLength) return -1;
    }

    cmp++;
    while (arr[previousStep] < value) {
        cmp++;
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

        cmp++;

        if      (arr[mid] == value)  return mid;
        else if (arr[mid] < value)   first = mid + 1;
        else if (arr[mid] > value)   last  = mid - 1;

    }

    return -1;
}


int main() {
    key_t arr[] = {0, 3, 5, 7, 10, 15, 16, 17, 18};
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    int (*fsearch[3]) (key_t*, size_t, key_t) = {
        findBinary,
        findJump, 
        findDirect, 
    };
    char ssearch[3][15] = {
        "findBinary",
        "findJump", 
        "findDirect", 
    };
    int fsearchSize = sizeof(fsearch) / sizeof(fsearch[0]); 

    const int iterations = 500;
    const long long nmax = 1000000;

    for (int f = 0; f < fsearchSize; f++) {
        printf("%s;\n", ssearch[f]);
        for (long long n = 10; n <= nmax; n *= 2) {

            key_t* arr = malloc(sizeof(key_t) * n);
            if (!arr) printf("[ERR] Cannot allocate memmory :(");

            for (int i = 0; i < n; i++) arr[i] = (i / 2) * 2;
            
            printf("%d;", n);

            uint64_t cmpSum = 0; 
            for (int i = 0; i < iterations; i++) {
                cmp = 0;
                int index = ((rand() << 15) | rand()) % n;

                fsearch[f](arr, n, arr[index]);
                cmpSum += cmp; 
            }

            printf("%d;\n", cmpSum / iterations);

            free(arr);
        }
        printf("\n");
    }

    return 0;
}