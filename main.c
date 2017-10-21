#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//program works with square matrix
int length_matrix;
int number_rows_swapped = 0;
int pivot_row;
int quantity_non_zeros;

void print_matrix(double** matrix) {
    for (int i = 0; i < length_matrix; i++) {
        printf("|");
        for (int j = 0; j < length_matrix; j++) {

            printf("%f ", matrix[i][j]);
        }
        printf("|\n");
    }

    printf("\n\n\n");
}

void copy_matrix(double** matrix, double** copy) {
    for (int i = 0; i < length_matrix; i++) {
        for (int j = 0; j < length_matrix; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
}

//read matrix from file 

double ** read_matrix(FILE * file) {
    fscanf(file, "%d%*c", &length_matrix);
    double** matrix = (double**) malloc(length_matrix * sizeof (double*));

    for (int i = 0; i < length_matrix; i++) {
        matrix[i] = (double*) malloc(length_matrix * sizeof (double));
    }

    for (int i = 0; i < length_matrix; i++) {
        for (int j = 0; j < length_matrix; j++) {
            fscanf(file, "%f%*c", &matrix[i][j]);
        }
    }

    return matrix;
}

double ** read_matrix_MatrixMarket(FILE * file) {
    fscanf(file, "%*[^\n]%*c");
    fscanf(file, "%d%*c%d%*c%d%*c", &length_matrix, &length_matrix, &quantity_non_zeros);
    double** matrix = (double**) calloc(length_matrix, sizeof (double*));

    int tam = length_matrix, qnt = quantity_non_zeros;

    for (int i = 0; i < length_matrix; i++) {
        matrix[i] = (double*) calloc(length_matrix, sizeof (double));
    }
    int linha = 0, coluna = 0, valor = 0;

    while (fscanf(file, "%d%*c%d%*c%*c%d%*c", &linha, &coluna, &valor) != EOF) {
        for (int i = 0; i < length_matrix; i++) {
            for (int j = 0; j < length_matrix; j++) {
                if (i == (linha - 1) && j == (coluna - 1)) {
                    matrix[i][j] = valor;
                }
            }
        }
    }


    return matrix;
}

double mod(double n) {
    return n >= 0 ? n : -1 * n;
}

//encontra o pivo de uma coluna

double pivot(double** matrix, int j) {
    double pivot = -FLT_MAX;
    for (int i = j; i < length_matrix; i++) {
        if ((mod(matrix[i][j]) > pivot) && (matrix[i][j] != 0)) {
            pivot = matrix[i][j];
            pivot_row = i;
        }
    }

    return pivot;
}

//swap two lines of matrix

void swap(double** matrix, int row1, int row2) {
    double* aux = (double*) malloc(length_matrix * sizeof (double));

    for (int i = 0; i < length_matrix; i++)
        aux[i] = matrix[row1][i];

    for (int j = 0; j < length_matrix; j++) {
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = aux[j];
    }
    number_rows_swapped++;
    free(aux);
}

void add_row(double** matrix, int row1, int row2, double k) {
    for (int j = 0; j < length_matrix; j++) {
        matrix[row2][j] = matrix[row2][j] + (-1) * k * matrix[row1][j];
    }
}

int eliminated(double** matrix) {
    for (int i = 1; i < length_matrix; i++)
        for (int j = 0; j < i; j++)
            if(matrix[i][j] && matrix[i][j] != -0.0000)
                return 0;
    return 1;
}

//generate base of upper triangular matrix

void create_utriangular(double** matrix) {
    for (int i = 0; i < length_matrix; i++) {
        for (int j = i; j < length_matrix; j++) {
            if (i == j)
                matrix[i][j] = 1.00;
            else
                matrix[i][j] = 0.00;
        }
    }
}

//apply the rule Ln <--- Ln -Mnp*Lp

void clear_column(double** matrix, double** L, int column, double pivot) {
    for (int i = column + 1; i < length_matrix; i++) {
        double multi = matrix[i][column] / pivot;

        L[i][column] = multi;
        add_row(matrix, column, i, multi);

    }
}

double** gauss_elimination(double** matrix, double** L) {
    for (int j = 0; j < length_matrix; j++) {
        double pivo = pivot(matrix, j);
        if (matrix[j][j] != pivo)
            swap(matrix, j, pivot_row);
        clear_column(matrix, L, j, pivo);
    }
}

double** transpose_matrix(double** matrix) {
    double** transpose_matrix = (double**) malloc(length_matrix * sizeof (double*));

    for (int i = 0; i < length_matrix; i++) {
        transpose_matrix [i] = (double*) malloc(length_matrix * sizeof (double));
    }

    for (int i = 0; i < length_matrix; i++) {
        for (int j = 0; j < length_matrix; j++) {
            transpose_matrix[i][j] = matrix[j][i];
        }
    }
    return transpose_matrix;
}

int main(int argc, char** argv) {
    /*ARMAZENAMENTO DENSO*/

    if (argc != 2) {
        printf("ERRO: Passe o nome do arquivo como argumento!\n");
        return 0;
    }
    FILE * file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo %s!\n", argv[1]);
        return 0;
    }

    double ** matriz = read_matrix_MatrixMarket(file);
    double ** matriz_transposta = transpose_matrix(matriz);

    printf("MATRIZ ORIGINAL!\n");
    print_matrix(matriz);
    printf("MATRIZ TRANSPOSTA!\n");
    print_matrix(matriz_transposta);

    //alocando matriz U
    double** U = (double**) malloc(length_matrix * sizeof (double*));

    for (int i = 0; i < length_matrix; i++) {
        U[i] = (double*) malloc(length_matrix * sizeof (double));
    }

    //alocando matriz L
    double** L = (double**) malloc(length_matrix * sizeof (double*));

    for (int i = 0; i < length_matrix; i++)
        L[i] = (double*) calloc(length_matrix, sizeof (double));


    create_utriangular(L);

    //print_matrix(matriz);
    //create_utriangular(L);

    gauss_elimination(matriz, L);
    printf("/*MATRIZ L*/\n");
    print_matrix(L);
    copy_matrix(matriz, U);
    printf("/*MATRIZ U*/\n");
    print_matrix(U);

    int retorno = eliminated(U);
    printf("----------> %d\n\n", retorno);


    /*int linhas = 5, colunas = 5, i = 0, j = 0, k = 0, l = 1, quantNaoNulos = 0, contAux = 1;
    int  *IA, *JA;
        double* AA;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] != 0) {
                quantNaoNulos++;
            }
            
        }
        
    }  
    /*ARMAZENAMENTO CSR*/
    /*
AA = (double*) malloc(quantNaoNulos * sizeof (double));
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
            
    }
    IA[l] = contAux;
    l++;
        
}
     */

    return (EXIT_SUCCESS);
}