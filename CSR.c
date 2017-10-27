#include "CSR.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

float** create_matrix_CSR(float ** matriz_transposta) {

    int i, j, k = 0, l = 1, contAux = 1;

    int quantity_non_zeros = return_quantity_non_zeros();
    int length_matrix = return_length_matrix();

    float *AA = (float*) malloc(quantity_non_zeros * sizeof (float));
    float *JA = (float*) malloc(quantity_non_zeros * sizeof (float));
    float *IA = (float*) malloc((length_matrix + 1) * sizeof (float));

    IA[0] = 1;

    for (i = 0; i < length_matrix; i++) {
        for (j = 0; j < length_matrix; j++) {
            if (matriz_transposta[i][j] != 0) {
                AA[k] = matriz_transposta[i][j];
                JA[k] = j;
                contAux++;
                k++;
            }
        }
        IA[l] = contAux;
        l++;
    }
    float** matrix_CSR = (float**) malloc(length_matrix * sizeof (float*));

    matrix_CSR[0] = AA;
    matrix_CSR[1] = JA;
    matrix_CSR[2] = IA;

    return matrix_CSR;

}

void print_matrix_CSR(float** matrix) {

    int quantity_non_zeros = return_quantity_non_zeros();
    int length_matrix = return_length_matrix();

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
                printf(" %e ", matrix[i][j]);
            }
        } else if (i == 2) {
            for (int j = 0; j < length_matrix + 1; j++) {
                printf(" %e ", matrix[i][j]);
            }
        }
        printf("|\n");
    }

    printf("\n\n\n");
}

int number_elements_row(float** A, int row) {
    return A[2][row + 1] - A[2][row];
}

float access_CSR(float** A, int i, int j) {

    return A[0][(int) (A[2][i] + j - 2)];
}

int get_column(float** A, int i, int j) {
    return (int) A[1][(int) (A[2][i] + j - 2)];
}

float** SOR_solution_CSR(float** matrix_CSR, float** b) {
    //alocando vetor solução x 
    int order = return_length_matrix();
    float** x = (float**) calloc(order, sizeof (float*));
    for (int i = 0; i < order; i++)
        x[i] = (float*) calloc(1, sizeof (float));

    printf("vetor b: \n");
    print_vector(b);
    int k = 0;
    float err = 1.0;
    float omega = return_omega();
    float tol = return_tol();
    int kmax = return_kmax();


    float** ant = (float**) calloc(order, sizeof (float*));
    for (int i = 0; i < order; i++)
        ant[i] = (float*) calloc(1, sizeof (float));

    copy_vector(ant, x);

    while ((k < kmax) && (err > tol)) {

        for (int i = 0; i < order; i++) {
            double sum = 0;
            int n = number_elements_row(matrix_CSR, i);
            for (int j = 1; j <= n; j++) {
                printf("coluna %d\n", get_column(matrix_CSR, i, j));
                if (i == get_column(matrix_CSR, i, j))
                    continue;
                sum += access_CSR(matrix_CSR, i, j) * x[get_column(matrix_CSR, i, j)][0];

            }


            x[i][0] = omega * (b[i][0] - sum) / access_CSR(matrix_CSR, i, i + 1) + (1 - omega) * ant[i][0];
        }
        printf("vetor solucao iteracao %d\n\n", k);
        print_vector(x);
        k++;
        err = error(x, ant);
        copy_vector(x, ant);
    }
    printf("Norma SOR : \n\n ---> %f \n\n", max(x));
    destroy_matrix(ant);
    return x;
}
