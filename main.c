#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//program works with square matrix
int length_matrix;
int number_rows_swapped = 0;
int pivot_row;


void print_matrix(float** matrix){
	for (int i = 0; i < length_matrix; i++){
		printf("|");
		for (int j = 0; j < length_matrix ; j++){
			
			printf("%f ", matrix[i][j]);		
		}
		printf("|\n");
	}

	printf("\n\n\n");
}


void copy_matrix(float** matrix, float** copy){
	for (int i = 0; i < length_matrix; i++){
		for (int j = 0; j < length_matrix ; j++){
			copy[i][j]=matrix[i][j];		
		}	
	}
}

//read matrix from file 
float ** read_matrix(FILE * file){
	
	fscanf(file, "%d%*c", &length_matrix);
	float** matrix = (float**)malloc(length_matrix * sizeof(float*));
	
	for (int i = 0; i < length_matrix ; i++){
		matrix[i] = (float*) malloc(length_matrix * sizeof (float));
	}

	for (int i = 0; i < length_matrix; i++){
		for (int j = 0; j < length_matrix ; j++){
			fscanf(file, "%f%*c", &matrix[i][j]);			
		}	
	}

	return matrix;
}

float mod(float n){
	return n >= 0 ? n : -1*n;	
}

//encontra o pivo de uma coluna
float pivot(float** matrix, int j){
	float pivot = -99999999.999999;	
	for (int i = j; i < length_matrix ; i++){
		if ((mod(matrix[i][j]) > pivot) && (matrix[i][j] != 0)){
			pivot = matrix[i][j];
			pivot_row = i;	
		}	
	}
	
	return pivot;
}

//swap two lines of matrix
void swap(float** matrix, int row1, int row2){
	float* aux = (float*)malloc(length_matrix*sizeof(float));

	for (int i = 0; i < length_matrix; i++)
		aux[i] = matrix[row1][i];

	for (int j = 0; j < length_matrix; j++){
		matrix[row1][j] = matrix[row2][j];
		matrix[row2][j] = aux[j];	
	}
	number_rows_swapped++;
	free(aux);
}


void add_row(float** matrix, int row1, int row2, float k){
	for (int j = 0; j < length_matrix; j++){
		matrix[row2][j] = matrix[row2][j] + (-1)*k*matrix[row1][j];
	}
}


int eliminated(float** matrix){
	for (int j = 0; j < length_matrix; j++)
		for (int i = j + 1; i < length_matrix; i++)
			if (matrix[i][j])
				return 0;		
	return 1;
}

//generate base of upper triangular matrix
void create_utriangular(float** matrix){
	for (int i = 0; i < length_matrix; i++){
		for (int j = i; j < length_matrix; j++){
			if(i==j)
				matrix[i][j] = 1.00;
			else 
				matrix[i][j] = 0.00;
		}
	}
	
					
}

//apply the rule Ln <--- Ln -Mnp*Lp
void clear_column(float** matrix, float** L,int column, float pivot){
	for (int i = column+1; i < length_matrix ; i++){
		float multi = matrix[i][column]/pivot;
		
		L[i][column] = multi;
		add_row(matrix, column, i,  multi);
		
	}
}


float** gauss_elimination(float** matrix, float** L){
	for (int j = 0; j < length_matrix; j++){
		float pivo = pivot(matrix, j);
		if (matrix[j][j] != pivo)
			swap(matrix, j, pivot_row);
		clear_column(matrix, L, j, pivo);
	}
}



int main(int argc, char** argv) {
    /*ARMAZENAMENTO DENSO*/

	if (argc != 2){
		printf("ERRO: Passe o nome do arquivo como argumento!\n");
		return 0;
	}
	
	FILE * file = fopen(argv[1], "r");
	if (file == NULL){
		printf("ERRO: Nao foi possivel abrir o arquivo!\n");
		return 0;
	}
	float ** matriz = read_matrix(file);
	
	//alocando matriz U
	float** U = (float**)malloc(length_matrix * sizeof(float*));
	
	for (int i = 0; i < length_matrix ; i++){
		U[i] = (float*) malloc(length_matrix * sizeof (float));
	}

	//alocando matriz L
	float** L = (float**)malloc(length_matrix * sizeof(float*));
	
	for (int i = 0; i < length_matrix ; i++)
		L[i] = (float*) calloc(length_matrix ,  sizeof (float));
	

	
	create_utriangular(L);
	

	
	//print_matrix(matriz);
	//create_utriangular(L);
	
	gauss_elimination(matriz, L);
	printf("/*MATRIZ L*/\n");
	print_matrix(L);
	copy_matrix(matriz, U);
	printf("/*MATRIZ U*/\n");
	print_matrix(U);
    /*int linhas = 5, colunas = 5, i = 0, j = 0, k = 0, l = 1, quantNaoNulos = 0, contAux = 1;
    int  *IA, *JA;
	float* AA;

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (matriz[i][j] != 0) {
                quantNaoNulos++;
            }
            
        }
        
    }  

    /*ARMAZENAMENTO CSR*/
	/*
    AA = (float*) malloc(quantNaoNulos * sizeof (float));
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
