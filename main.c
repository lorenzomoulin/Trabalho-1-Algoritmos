#include <string.h>
#include <stdio.h>
#include "denso.h"
#include "CSR.h"
//program works with square matrix

int main(int argc, char** argv) {


    if ((argv[1] == NULL) || (strcmp(argv[1], "-CSR") && strcmp(argv[1], "-DENSO"))) {
        printf("ERRO: Nao escolheu o tipo de armazenamento\n");
        return 0;
    }
    if (argv[2] == NULL) {
        printf("ERRO: Nao escolheu arquivo da matrix\n");
        return 0;
    }
    if (argv[3] == NULL) {
        printf("Nao escolheu arquivo do vector!\n");
        return 0;
    }

    FILE * file = fopen(argv[2], "r");
    FILE * file2 = fopen(argv[3], "r");

    if (file == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo da matrix!\n");
        return 0;
    }
    if (file2 == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo do vector!\n");
        return 0;
    }
    /*ARMAZENAMENTO DENSO*/
    if (!strcmp(argv[1], "-DENSO")) {
        double ** matriz = read_matrix_MatrixMarket(file);
        double** b = read_vector(file2);
        double ** matriz_transposta = transpose_matrix(matriz);

        printf("MATRIZ ORIGINAL!\n");
        print_matrix(matriz);
        printf("MATRIZ TRANSPOSTA!\n");
        print_matrix(matriz_transposta);

        //alocando matriz U
        double** U = create_matrix();

        //alocando matriz L
        double** L = create_matrix();

        //alocando matrix de permutacoes
        double** P = create_matrix_P();
        printf("/*MATRIZ P ORIGINAL*/\n");
        print_matrix(P);

        gauss_elimination(matriz_transposta, L, P);

        create_utriangular(L);
        printf("/*MATRIZ L*/\n");
        print_matrix(L);

        copy_matrix(matriz_transposta, U);
        printf("/*MATRIZ U*/\n");
        print_matrix(U);


        printf("/*MATRIZ P*/\n");
        print_matrix(P);
        double** y = solution_Ly_Pb(L, P, b);
        printf("y VECTOR\n");
        print_vector(y);

        double** x = solution_Ux_y(U, y);
        printf("SOLUCAO POR LU: \n");
        //print_vector(x);

        destroy_matrix(b);
        destroy_matrix(y);
        destroy_matrix(x);
        destroy_matrix(L);
        destroy_matrix(U);
        destroy_matrix(matriz);
        destroy_matrix(matriz_transposta);
        destroy_matrix(P);

        return 0;

    } else if (!strcmp(argv[1], "-CSR")) {
        double ** matriz = read_matrix_MatrixMarket(file);
        double** b = read_vector(file2);
        double ** matriz_transposta = transpose_matrix(matriz);
        double ** matriz_CSR = create_matrix_CSR(matriz_transposta);
        
        print_matrix_CSR(matriz_CSR);
        return 0;

    }

    return (EXIT_SUCCESS);
}