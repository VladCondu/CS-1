#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int read(int n, int ** matrix) {

    //Citire cele n linii si alocare spatiu necesar
    int i;
    for (i = 0; i < n; ++i) {
        int x;
        int lenght = 3;
        matrix[i] = (int * ) malloc(lenght * sizeof(int));
        int buffer = 0;
        while (scanf("%d", & x) && x != 0) {
            if (++buffer > lenght) {
                if (lenght + 3 <= 100) {
                    lenght += 3;
                    matrix[i] = realloc(matrix[i], lenght * sizeof(int));
                } 
                else
                    return 0;
            }
            matrix[i][buffer - 1] = x;
        }
        if (++buffer > lenght) {
            lenght += 3;
            matrix[i] = realloc(matrix[i], lenght * sizeof(int));
        }
        matrix[i][buffer - 1] = 0;
    }
    return 1;
}

void freeMem(int n, int ** matrix) {
    int i;
    for (i = 0; i < n; ++i) {
        free(matrix[i]);
        matrix[i] = NULL;
    }
    free(matrix);
    matrix = NULL;
}

void buildOrderArray(int * order, int n) {
    int count = 0, i;
    for (i = 0; i < n; ++i) {
        order[i] = count++;
    }
}

void buildAverageArray(float * average, int n, int ** matrix) {
    int i;
    for (i = 0; i < n; i++) {
        int num = 0, total = 0;
        while (matrix[i][num]) {
            total += matrix[i][num++];
        }
        average[i] = ((float) total / (float) num);
    }
}

void bubbleSort(float * average, int * order, int n) {
    int i;
    for(i = 0; i < n - 1; i++) {
        int j;
        for (j = 0; j < n - i - 1; j++) {
            if (average[j] < average[j + 1] && fabs(average[j] - average[j + 1]) > 0.0001) {
                float aux = average[j];
                average[j] = average[j + 1];
                average[j + 1] = aux; 

                order[j + 1] += order[j];
                order[j] = order[j + 1] - order[j];
                order[j + 1] -= order[j];
            } 
        }
    }
}

void print(int ** matrix, int * order, float * average, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf("%-9.03lf", average[i]);
        int j = 0;
        while (matrix[order[i]][j] != 0)
            printf(" %d", matrix[order[i]][j++]);
        printf("\n");
    }
}
int main(void) {
    int n;
    scanf("%d", & n);
    int ** matrix = (int ** ) malloc(n * sizeof(int * )); // alocare pentru vectorul de linii
    float * average = (float * ) malloc(n * sizeof(float));
    int * order = (int * ) malloc(n * sizeof(int));

    if (read(n, matrix)) {
        buildOrderArray(order, n);
        buildAverageArray(average, n, matrix);
        bubbleSort(average, order, n);
        print(matrix, order, average, n);
    } 
    else
        printf("%s\n", "Randul de numere este prea mare! Introduceti maxim 100 de numere");

    freeMem(n, & ( * matrix));
    free(average);
    free(order);

    return 0;
}
