#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int read(int n, int ** matrix) {

    // Citire cele n linii si alocare spatiu necesar
    int i;
    for (i = 0; i < n; ++i) { 
        int x; // variabila pentru citire
        int lenght = 3; // Lungimea initiala a vectorului
        matrix[i] = (int * ) malloc(lenght * sizeof(int));
        int buffer = 0;

        // cat timp citeste x si x e diferit de 0 salveaza in matrice
        while (scanf("%d", & x) && x != 0) { 
            if (++buffer > lenght) {
                // daca nu sunt introduse mai mult de 100 de numere (asa nu vor fi mai mult de 200 de caractere, pentru ca fiecare numar e separat prin spatiu)
                if (lenght + 3 <= 100) { 
                    lenght += 3;
                    matrix[i] = realloc(matrix[i], lenght * sizeof(int)); // realocare in caz ca depaseste lungimea initiala
                } else {
                    return 0;
                }
            }
            matrix[i][buffer - 1] = x;
        }
        // Adaugare nr 0 la final pentru a unde se termina o linie
        if (++buffer > lenght) {
            lenght += 3;
            matrix[i] = realloc(matrix[i], lenght * sizeof(int));
        }
        matrix[i][buffer - 1] = 0;
    }
    return 1;
}

// Functie pentru eliberare memorie
void freeMem(int n, int ** matrix) {
    int i;
    for (i = 0; i < n; ++i) {
        free(matrix[i]);
        matrix[i] = NULL;
    }
    free(matrix);
    matrix = NULL;
}

// Functie creare vector de ordine pentru afisare
void buildOrderArray(int * order, int n) {
    int count = 0, i;
    for (i = 0; i < n; ++i) {
        order[i] = count++;
    }
}

// Fcuntie creare vector de medii
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

// Sortare vector de medii aceleasi mutari fiind facute si pe vectorul de ordine
void bubbleSort(float * average, int * order, int n) {
    int i;
    for(i = 0; i < n - 1; i++) {
        int j;
        for (j = 0; j < n - i - 1; j++) {
            // fabs pentru cazuri in care 2 medii egale sunt obtinute din numere diferite 
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

// Functie de afisare medii si matricea notelor utilizand vectorul de ordine
void print(int ** matrix, int * order, float * average, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf("%-9.03lf", average[i]); // afisare la stanga pe 10 spatii (9 + 1 de la nota) si media pe exact 3 zecimale
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
    } else {
        printf("Randul de note este prea mare! Introduceti maxim 100 de numere \n"); // caz in care sunt introduse mai mult de 200 de caractere pe linia de note
    }

    freeMem(n, & ( * matrix));
    free(average);
    free(order);

    return 0;
}
