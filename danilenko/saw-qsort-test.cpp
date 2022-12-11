#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <windows.h>

using namespace std;

int compare(const void *pa, const void *pb)
{
    int va = *(int *)pa, vb = *(int *)pb;
    if (va < vb)
        return -1;
    else if (va > vb)
        return 1;
    else
        return 0;
}

int RandMinMax(int Min, int Max)
{
    return (rand() << 15 | rand()) % (Max - Min) + Min;
}

void FillUp(int *A, int N, int Min, int Max, int k)
{
    for (int i = 0; i < N; i++)
        A[i] = (Max - Min) * i / (N - 1) + Min;
}

void FillSaw(int *A, int N, int Min, int Max, int k)
{
    for (int i = 0; i < N; i++) // k - длина интервала
        A[i] = (Max - Min) * (i % k) / (k - 1) + Min;
}

void FillRand(int *A, int N, int Min, int Max, int k)
{
    for (int i = 0; i < N; i++)
        A[i] = RandMinMax(Min, Max);
}

void FillStears(int *A, int N, int Min, int Max, int k)
{
    int n = N / k;           // количество интервалов
    int h = (Max - Min) / n; //высота ступеньки
    for (int i = 0; i < n - 1; i++)
        FillRand(A + i * k, n, Min + i * h, Min + i * h + h / 2, 0);
    FillRand(A + k * (n - 1), N - k * (n - 1), Min + (n - 1) * h, Min + (n - 1) * h + h / 2, 0);
}

// void FillStears(int * A, int N, int Min, int Max, int k) {
//   int n = N/k;
//   int d = (Max - Min)/k;
//   int b = (Max - Min)/N;
//
//   //for (int i = 0; i < k; i++)
//  // for (int j = 0; j < n; j++)
//  //  A[i*n + j] = d*i + rand()% (d/5) + Min;
//   for (int i = 0; i < N; i++)
//    A[i] = i*b + Min + rand() % (3 * b);
//
// }

void print(int *A, int from, int to)
{
    for (int i = from; i <= to; i++)
        printf("%d ", A[i]);
    printf("\n");
}

int Compares = 0;

bool Equal(int A, int B)
{
    Compares++;
    return A == B;
}

void TestQSort(int *A, int N)
{
    qsort(A, N, sizeof(int), compare);
}
// int SeqSearch(int * A, int Key, int N) {
// for (int i = 0; i < N; i++)
//  if (Equal(Key, A[i])) return i;
// return -1;
// }
////SeqSearch(A, A[RandMinMax(0, i)], i);

typedef void (*FillFunctionType)(int *, int, int, int, int);
typedef void (*SortFunctionType)(int *, int);

#define NMax 50000000
#define Trials 1
#define FillFunctionsCount 4
#define SortFunctionsCount 1
int main()
{
    int *A = new int[NMax], t0, t1, time = 0;
    FillFunctionType FillFunctions[FillFunctionsCount] = {FillRand,
                                                          FillUp, FillSaw, FillStears};
    char FillFunctionsNames[FillFunctionsCount][10] = {"Rand\n", "Up\n",
                                                       "Saw\n", "Stears\n"};
    SortFunctionType SortFunctions[SortFunctionsCount] = {TestQSort};
    

    FillSaw(A, NMax, 0, NMax, 10);
    for (int i = 0; i < 20; i++)
        printf("%d\n", A[i]);

    printf("\n\n");
    TestQSort(A, NMax);
    for (int i = 0; i < 20; i++)
        printf("%d\n", A[i]);

    return 0;

    
    for (int N = NMax / 10; N < NMax; N += NMax / 10)
    {
        for (int f = 0; f < FillFunctionsCount; f++)
        {
            printf("%s", FillFunctionsNames[f]);
            for (int s = 0; s < SortFunctionsCount; s += 1)
            {
                // FillFunctions[f](A, N, 0, N, N / 5000);
                FillFunctions[f](A, N, 0, N, 10);
                // print(A, 0, N - 1);
                t0 = GetTickCount();
                SortFunctions[s](A, N);
                t1 = GetTickCount();
                // print(A, 0, N - 1);
                time = t1 - t0;
                printf("%d %d\n", N, time / Trials);
            }
        }
    }
    delete[] A;
    system("pause");
}