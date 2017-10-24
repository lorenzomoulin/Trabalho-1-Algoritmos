#include "CSR.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>


double** create_matrix_CSR(double ** matriz_transposta) {

    int i, j, k = 0, l = 1, contAux = 1;

    int quantity_non_zeros = retorna_quantity_non_zeros();
    int length_matrix = retorna_length_matrix();

    double *AA = (double*) malloc(quantity_non_zeros * sizeof (double));
    double *JA = (double*) malloc(quantity_non_zeros * sizeof (double));
    double *IA = (double*) malloc((length_matrix + 1) * sizeof (double));

    IA[0] = 1;

    for (i = 0; i < length_matrix; i++) {
        for (j = 0; j < length_matrix; j++) {
            if (matriz_transposta[i][j] != 0) {
                AA[k] = matriz_transposta[i][j];
                JA[k] = j + 1;
                contAux++;
                k++;
            }
        }
        IA[l] = contAux;
        l++;
    }
    double** matrix_CSR = (double**) malloc(length_matrix * sizeof (double*));

    matrix_CSR[0] = AA;
    matrix_CSR[1] = JA;
    matrix_CSR[2] = IA;

    return matrix_CSR;

}

void print_matrix_CSR(double** matrix) {

    int quantity_non_zeros = retorna_quantity_non_zeros();
    int length_matrix = retorna_length_matrix();

    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            printf("AA: ");
        } else if (i == 1) {
            printf("JA: ");
        } else if (i == 2) {
            printf("IA: ");
        }
        printf("|");
        if (i == 0 || i == 1) {
            for (int j = 0; j < quantity_non_zeros; j++) {
                printf(" %.0f ", matrix[i][j]);
            }
        } else if (i == 2) {
            for (int j = 0; j < length_matrix + 1; j++) {
                printf(" %.0f ", matrix[i][j]);
            }
        }
        printf("|\n");
    }

    printf("\n\n\n");
}
