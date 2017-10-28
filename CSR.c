#include "CSR.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

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
	 scanf("%f%*c", &tol);
    scanf("%d%*c", &kmax);
    scanf("%f%*c", &omega);
}


void create_matrix_CSR(FILE * file,float** matrix_CSR, float* AA, float* JA, float* IA) {

     int i, j, k = 0, l = 1, contAux = 1;
    
    
    //le informacoes da matriz
    
     
	
    
    
	
    if (matrix_CSR == NULL || AA ==NULL || JA == NULL || IA == NULL){
		printf("nao foi possivel alocar memoria\n\n");
		exit(1);
    }
   
    IA[0] = 0;
	int column, row;
    float value;
    int ant;
    ant = 0;
    while (fscanf(file, "%d %d %f\n", &row, &column, &value) != EOF){ 	
        
        row -= 1;
        column -= 1;
        AA[k] = value;
        JA[k] = row;       
        IA[column+1] = k+1;
        k++;
    	
    }

    matrix_CSR[0] = AA;
    matrix_CSR[1] = JA;
    matrix_CSR[2] = IA;

}

/*float ** read_matrix_MatrixMarket_CSR(FILE * file) {
	
    
    float** matrix = (float**) calloc(length_matrix, sizeof (float*));

    

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
}*/

/*float** create_matrix_CSR(float ** matriz_transposta) {

    int i, j, k = 0, l = 1, contAux = 1;

    int quantity_non_zeros = return_quantity_non_zeros();
    int length_matrix = return_length_matrix();

    float *AA = (float*) malloc(quantity_non_zeros * sizeof (float));
    float *JA = (float*) malloc(quantity_non_zeros * sizeof (float));
    float *IA = (float*) malloc((length_matrix + 1) * sizeof (float));

    IA[0] = 0;

    for (i = 0; i < length_matrix; i++) {
        for (j = 0; j < length_matrix; j++) {
            if (matriz_transposta[i][j] != 0) {
                AA[k] = matriz_transposta[i][j];
                JA[k] = j;
                contAux++;
                k++;
            }
        }
        IA[l] = contAux-1;
        l++;
    }
    float** matrix_CSR = (float**) malloc(length_matrix * sizeof (float*));

    matrix_CSR[0] = AA;
    matrix_CSR[1] = JA;
    matrix_CSR[2] = IA;

    return matrix_CSR;

}*/

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



float access_CSR(float** A, int i, int j){
	
	return A[0][(int)(A[2][i]+j-1)];
}

int get_column(float** A, int i, int j){
	return (int)A[1][(int)(A[2][i]+j-1)];
}

void generate_b_CSR(float** matrix_CSR,float** b){
	
	
	
	for (int i = 0; i < length_matrix; i++){
		
		float sum = 0;
		for (int j = matrix_CSR[2][i]; j < matrix_CSR[2][i+1]; j++){
			sum += matrix_CSR[0][j];
		}
		
		b[i][0] = sum; 
	}
	
}

void copy_vectorr(float** v1, float** v2){
	for (int i = 0; i < length_matrix ; i++)
		v2[i][0] = v1[i][0];
}

float maxx(float** v){
	float max = -FLT_MAX;
	
	for (int i = 0 ; i < length_matrix; i++){
		
		
		if (mod(v[i][0]) > max)
			max = mod(v[i][0]);
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
		
		sub[i][0] = v2[i][0] - v1[i][0];
		
	}
    e = mod(maxx(sub));
    
   
    destroy_matrixx(sub);
	
    
	e /= mod(maxx(v2));
	
	
	
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
			
			float diag;
			
			for (int j = matrix_CSR[2][i]; j < matrix_CSR[2][i+1]; j++){
			
				
				
				if(i == matrix_CSR[1][j]){
					diag = matrix_CSR[0][j];
					continue;
				}
				
					
				sum += matrix_CSR[0][j]*x[(int)matrix_CSR[1][j]][0];
				
			}
			
			x[i][0] = omega*(b[i][0] - sum)/diag + (1-omega)*x[i][0];
			
			
		}
		
		k++;
		err = errorr(x, ant);
		copy_vectorr(x, ant);
		
	}	
	destroy_matrixx(ant);
	printf("norma da solucao: %f\n\n", maxx(x));
	
}
