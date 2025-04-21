#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

void merge(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int* arr, int left, int right, int deep, double setter) {
    if((double) deep < setter){
      if (left < right) {
          int mid = left + (right - left) / 2;
          #pragma omp task
          mergeSort(arr, left, mid, deep++, setter);
          #pragma omp task
          mergeSort(arr, mid + 1, right, deep++, setter);

          #pragma omp taskwait
          merge(arr, left, mid, right);
      }
    }

    else{
      if (left < right) {
          int mid = left + (right - left) / 2;
          mergeSort(arr, left, mid, deep++, setter);
          mergeSort(arr, mid + 1, right, deep++, setter);
          merge(arr, left, mid, right);
      }
    }
}

void printArray(int* arr, int size) {
    for (int i = 0; i < size; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main(int argc, char *argv[]) {

    srand (time(NULL));

    int n = atoi(argv[1]);
    int nth = atoi(argv[2]);


    int* arr = new int[n];

    for (int i = 0; i < n; ++i)
        arr[i] = rand() %999;

    printArray(arr, n);
    omp_set_num_threads(nth);
    double setter = log2(nth);
    #pragma omp parallel
    {
      #pragma omp single
      {
        mergeSort(arr, 0, n - 1, 0, setter);
      }
    }



    cout << "array ordenado:\n";
    printArray(arr, n);

    delete[] arr;

    return 0;
}
