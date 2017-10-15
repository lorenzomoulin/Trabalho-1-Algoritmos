#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    /*ARMAZENAMENTO DENSO*/

    int linhas = 5, colunas = 5, i = 0, j = 0, k = 0, l = 1, quantNaoNulos = 0, contAux = 1;
    int *AA, *IA, *JA;

    /*matriz = (int**) malloc(linhas * sizeof (int*));
    for (; i < linhas; i++) {
        matriz[i] = (int*) malloc(colunas * sizeof (int));
    }*/
    int matriz[5][5] = {1, 4, 0, 0, 0,
        5, 3, 2, 0, 0,
        0, 3, 6, 8, 0,
        0, 0, 5, 9, 4,
        0, 0, 0, 3, 7};

    printf("\nDIMENSOES DA MATRIZ: [%d,%d]\n", linhas, colunas);
    printf("\nMATRIZ DENSA\n");

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] != 0) {
                quantNaoNulos++;
            }
            //printf(" %d ", matriz[i][j]);
        }
        //printf("\n");
    }

    //printf("\n");

    /*ARMAZENAMENTO CSR*/

    AA = (int*) malloc(quantNaoNulos * sizeof (int));
    JA = (int*) malloc(quantNaoNulos * sizeof (int));
    IA = (int*) malloc((linhas + 1) * sizeof (int));

    IA[0] = 1;

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] != 0) {
                AA[k] = matriz[i][j];
                JA[k] = j + 1;
                contAux++;
                k++;
            }
            printf(" %d ", matriz[i][j]);
        }
        IA[l] = contAux;
        l++;
        printf("\n");
    }
    printf("\n");

    printf("\nARMAZENAMENTO CSR\n");
    for (k = 0; k < quantNaoNulos; k++) {
        printf(" %d ", AA[k]);
    }
    printf("\n");
    for (k = 0; k < quantNaoNulos; k++) {
        printf(" %d ", JA[k]);
    }
    printf("\n");
    for (k = 0; k < linhas + 1; k++) {
        printf(" %d ", IA[k]);
    }
    printf("\n");


    return (EXIT_SUCCESS);
}
