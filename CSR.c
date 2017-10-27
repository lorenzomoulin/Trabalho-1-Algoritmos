#include "CSR.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int length_matrix, quantity_non_zeros, kmax, col;
float tol, omega;


void destroy_matrixx(float** matrix) {

    for (int i = 0; i < length_matrix; i++)
        free(matrix[i]);
    free(matrix);
}

void destroy_matrix_CSR(float** m){	
	free(m[0]);
	free(m[1]);
	free(m[2]);
	free(m);
}

int return_length_matrix_CSR() {
    return length_matrix;
}

int return_quantity_non_zeros_CSR() {
    return quantity_non_zeros;
}

float return_omega_CSR(){
	return omega;
}

float return_tol_CSR(){
	return tol;
}

float return_kmax_CSR(){
	return kmax;
}

void set_global(FILE * file){
	fscanf(file, "%d %d %d\n", &length_matrix, &length_matrix, &quantity_non_zeros);
	printf("tolerancia:"); scanf("%f%*c", &tol);
    printf("numero max de iteracoes: ");scanf("%d%*c", &kmax);
    printf("omega: ");scanf("%f%*c", &omega);
}

float ** read_matrix_MatrixMarket_CSR(FILE * file) {
	
    
    float** matrix = (float**) calloc(length_matrix, sizeof (float*));

    //int tam = length_matrix, qnt = quantity_non_zeros;

    for (int i = 0; i < length_matrix; i++) {
        matrix[i] = (float*) calloc(length_matrix, sizeof (float));
    }
    int linha = 0, coluna = 0;
    float valor = 0;

    while (fscanf(file, "%d %d %f%*c", &linha, &coluna, &valor) != EOF) {
        matrix[coluna-1][linha-1] = valor;
    }
    fclose(file);

    return matrix;
}

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
                printf(" %f ", matrix[i][j]);
            }
        } else if (i == 2) {
            for (int j = 0; j < length_matrix + 1; j++) {
                printf(" %f ", matrix[i][j]);
            }
        }
        printf("|\n");
    }

    printf("\n\n\n");
}

int number_elements_row(float** A, int row){
	return A[2][row+1] - A[2][row];
}

float access_CSR(float** A, int i, int j){
	
	return A[0][(int)(A[2][i]+j-2)];
}

int get_column(float** A, int i, int j){
	return (int)A[1][(int)(A[2][i]+j-2)];
}

void generate_b_CSR(float** matrix,float** b){
	
	//print_matrix_CSR(matrix);
	
	for (int i = 0; i < length_matrix; i++){
		int n = number_elements_row(matrix, i);
		float sum = 0;
		for (int j = 1; j <= n; j++){
			sum += access_CSR(matrix,i,j);
		}
		b[i][0] = sum; //TAVA b[i][0]
	}
	printf("gerou b\n");
	
}

void copy_vectorr(float** v1, float** v2){
	for (int i = 0; i < length_matrix ; i++)
		v2[i][0] = v1[i][0];
}

float maxx(float** v){
	float max = -FLT_MAX;
	
	for (int i = 0 ; i < length_matrix; i++){
		
		
		if (v[i][0] > max)
			max = v[i][0];
	}
	
	return max;
}



float errorr(float** v2, float** v1){
	float e;
    float** sub = (float**) calloc(length_matrix,sizeof(float*));
	for (int i = 0; i < length_matrix; i++) 
        sub[i] = (float*) calloc(1,sizeof (float));
    if (sub == NULL){
    	printf("cabo\n");
    	exit(1);
    }
    for (int i = 0; i < length_matrix; i++){
		
		sub[i][0] = mod(v2[i][0] - v1[i][0]);
		
	}
    e = maxx(sub);
    
    //for (int i = 0; i < length_matrix; i++)
      //  free(sub[i]);
    free(sub);
	float** teste = (float**) calloc(length_matrix,sizeof(float*));
	
	for (int i = 0; i < length_matrix; i++) 
        teste[i] = (float*) calloc(1,sizeof (float));
    for (int i =0; i < length_matrix; i++)
    	teste[i][0] = mod (v2[i][0]);
    
	e /= maxx(teste);
	printf("erro:%f\n\n", e);
	free(teste);
	
	return e;
}

void SOR_solution_CSR(float** matrix_CSR, float** b, float** x){
	//alocando vetor solução x 
	
	
	int k = 0;
	float err = 1.0;
	
	
	
	float** ant = (float**) calloc(length_matrix,sizeof(float*));
	for (int i = 0; i < length_matrix; i++) 
        ant[i] = (float*) calloc(1,sizeof (float));
    int coluna;
    copy_vectorr(ant,x);
	while ( (k < kmax) && (err > tol)){
	
	
		for (int i = 0; i < length_matrix; i++){
			float sum = 0;
			
	
			int n = number_elements_row(matrix_CSR, i);
			
			for (int j = 1; j <= n; j++){
			
				
				//printf("coluna %d\n", get_column(matrix_CSR,i,j));
				//printf("seg fault?\n\n");
				if(i == get_column(matrix_CSR,i,j)){
					
					continue;
				}
				
					
				sum += access_CSR(matrix_CSR,i,j)*x[get_column(matrix_CSR,i,j)][0];
				printf("elemento de valor %f na linha %d e coluna %d\n\n", access_CSR(matrix_CSR,i,j), i, get_column(matrix_CSR, i,j));
			}
			if (access_CSR(matrix_CSR,i,i+1) == 0){
				printf("divisao por zero em i=%d\n\n", i);
			}
			x[i][0] = omega*(b[i][0] - sum)/access_CSR(matrix_CSR,i,i+1) + (1-omega)*x[i][0];
			
		}
		//printf("vetor solucao iteracao %d\n\n", k);
		//print_vector(x);
		k++;
		err = errorr(x, ant);
		copy_vectorr(x, ant);
		printf("iteracao %d\n", k);
	}	
	printf("norma da solucao: %f\n\n", maxx(x));
	//destroy_matrix(x);
    //destroy_matrixx(ant);
	
}
