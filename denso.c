#include "denso.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

int length_matrix;
int number_rows_swapped = 0;
int pivot_row;
int quantity_non_zeros;
float omega;
int kmax;
float tol;

float** generate_b(float** A){
	float** b = (float**) malloc(length_matrix* sizeof (float*));
    for (int i = 0; i < length_matrix; i++)
        b[i] = (float*) malloc( sizeof (float));
        
	for (int i = 0; i < length_matrix; i++){
		float sum = 0;
		for (int j = 0; j < length_matrix; j++){
			sum += A[i][j];
		}
		b[i][0] = sum;
	}
	return b;
}


float** create_matrix() {
    float** matrix = (float**) malloc(length_matrix * sizeof (float*));

    for (int i = 0; i < length_matrix; i++) {
        matrix[i] = (float*) malloc(length_matrix * sizeof (float));
    }

    return matrix;
}

//allocate space for vector matrix[length][1]



void print_vector(float** vector) {
    for (int i = 0; i < length_matrix; i++)
        printf("| %e |\n", vector[i][0]);
}

float** multi_matrix_vector(float** A, float** x) {

    float** c = (float**) calloc(length_matrix, sizeof (float*));
    for (int i = 0; i < length_matrix; i++)
        c[i] = (float*) calloc(1, sizeof (float));

    for (int i = 0; i < length_matrix; i++)
        for (int k = 0; k < length_matrix; k++)
            c[i][0] += A[i][k] * x[k][0];

    return c;
}

//solve Ly=Pb

float** solution_Ly_Pb(float** L, float** P, float** b) {
    float** c = multi_matrix_vector(P, b);


    float** y = (float**) malloc(length_matrix * sizeof (float*));
    for (int i = 0; i < length_matrix; i++)
        y[i] = (float*) malloc(sizeof (float));

    for (int i = 0; i < length_matrix; i++) {
        y[i][0] = c[i][0];
        for (int j = 0; j < i; j++)
            y[i][0] -= L[i][j] * y[j][0];

    }

    destroy_matrix(c);
    return y;
}

float** solution_Ux_y(float** U, float** y) {
    float** x = (float**) malloc(length_matrix * sizeof (float*));
    for (int i = 0; i < length_matrix; i++)
        x[i] = (float*) malloc(sizeof (float));

    for (int i = length_matrix - 1; i >= 0; i--) {
        x[i][0] = y[i][0];
        for (int j = i + 1; j < length_matrix; j++) {
            x[i][0] -= U[i][j] * x[j][0];
        }
        x[i][0] /= U[i][i];
    }

    return x;
}

float** create_matrix_P() {
    float** matrix = (float**) calloc(length_matrix, sizeof (float*));

    for (int i = 0; i < length_matrix; i++)
        matrix[i] = (float*) calloc(length_matrix, sizeof (float));

    for (int i = 0; i < length_matrix; i++)
        matrix[i][i] = 1;

    return matrix;
}

void print_matrix(float** matrix) {
    for (int i = 0; i < length_matrix; i++) {
        printf("|");
        for (int j = 0; j < length_matrix; j++) {

            printf("%e ", matrix[i][j]);
        }
        printf("|\n");
    }

    printf("\n\n\n");
}

void copy_matrix(float** matrix, float** copy) {
    for (int i = 0; i < length_matrix; i++) {
        for (int j = 0; j < length_matrix; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
}

//read matrix from file 

float ** read_matrix_MatrixMarket(FILE * file) {
	
    fscanf(file, "%*[^\n]%*c");
    fscanf(file, "%d%*c%d%*c%d%*c", &length_matrix, &length_matrix, &quantity_non_zeros);
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

float mod(float n) {
    return n >= 0 ? n : -1 * n;
}

//encontra o pivo de uma coluna

float pivot(float** matrix, int j) {
    float pivot = -FLT_MAX;
    for (int i = j; i < length_matrix; i++) {
        if ((mod(matrix[i][j]) > pivot) && (matrix[i][j] != 0)) {
            pivot = matrix[i][j];
            pivot_row = i;
        }
    }

    return pivot;
}

//swap two lines of matrix

void swap(float** matrix, int row1, int row2) {
    float* aux = (float*) malloc(length_matrix * sizeof (float));

    for (int i = 0; i < length_matrix; i++)
        aux[i] = matrix[row1][i];

    for (int j = 0; j < length_matrix; j++) {
        matrix[row1][j] = matrix[row2][j];
        matrix[row2][j] = aux[j];
    }
    number_rows_swapped++;
    free(aux);
}

void add_row(float** matrix, int row1, int row2, float k) {
    for (int j = 0; j < length_matrix; j++) {
        if (j <= row1) {
            matrix[row2][j] = 0;
            continue;
        }
        matrix[row2][j] = matrix[row2][j] + (-1) * k * matrix[row1][j];
    }
}

int eliminated(float** matrix) {
    for (int i = 1; i < length_matrix; i++)
        for (int j = 0; j < i; j++)
            if (matrix[i][j])
                return 0;
    return 1;
}

//generate base of upper triangular matrix

void create_utriangular(float** matrix) {
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

void clear_column(float** matrix, float** L, int column, float pivot) {
    for (int i = column + 1; i < length_matrix; i++) {
        float multi = matrix[i][column] / pivot;

        L[i][column] = multi;
        add_row(matrix, column, i, multi);

    }
}

void gauss_elimination(float** matrix, float** L, float** P) {
    for (int j = 0; j < length_matrix; j++) {
        float pivo = pivot(matrix, j);
        if (matrix[j][j] != pivo) {
            swap(matrix, j, pivot_row);
            swap(P, j, pivot_row);
            swap(L, j, pivot_row);
        }
        clear_column(matrix, L, j, pivo);
    }
}

float** transpose_matrix(float** matrix) {
    float** transpose_matrix = create_matrix();

    for (int i = 0; i < length_matrix; i++) {
        for (int j = 0; j < length_matrix; j++) {
            transpose_matrix[i][j] = matrix[j][i];
        }
    }
    return transpose_matrix;
}

void destroy_matrix(float** matrix) {

    for (int i = 0; i < length_matrix; i++)
        free(matrix[i]);
    free(matrix);
}

int return_length_matrix() {
    return length_matrix;
}

int return_quantity_non_zeros() {
    return quantity_non_zeros;
}

float return_omega(){
	return omega;
}

float return_tol(){
	return tol;
}

float return_kmax(){
	return kmax;
}



void copy_vector(float** v1, float** v2){
	for (int i = 0; i < length_matrix ; i++)
		v2[i][0] = v1[i][0];
}

float max(float** v){
	float max = -FLT_MAX;
	
	for (int i = 0 ; i < length_matrix; i++){
		
		
		if (v[i][0] > max)
			max = v[i][0];
	}
	
	return max;
}

float** sub_vector(float** v2 , float** v1){
	float** sub = (float**) malloc(length_matrix*sizeof(float*));
	for (int i = 0; i < length_matrix; i++) 
        sub[i] = (float*) malloc(sizeof (float));
        
	for (int i = 0; i < length_matrix; i++){
		
			sub[i][0] = mod(v2[i][0] - v1[i][0]);
		
	}
	return sub;
}

float error(float** v2, float** v1){
	float e;
    
    float** sub = sub_vector(v2,v1);
    
    e = max(sub);
    destroy_matrix(sub);	
    float** teste= (float**) malloc(length_matrix*sizeof(float*));
	for (int i = 0; i < length_matrix; i++) 
        teste[i] = (float*) malloc(sizeof (float));
	for (int i =0 ; i < length_matrix; i++)
		teste[i][0] = mod (v2[i][0]);
		
	e /= max(teste);
	destroy_matrix(teste);
	return e;
}

float** SOR_solution(float** A, float** b){
	scanf("%f%*c", &tol);
    scanf("%d%*c", &kmax);
    scanf("%f%*c", &omega);
    
    clock_t begin = clock();
	//alocando vetor solução x 
	float** x = (float**) calloc(length_matrix,sizeof(float*));
	for (int i = 0; i < length_matrix; i++) 
        x[i] = (float*) calloc(1,sizeof (float));
	
	//printf("vetor b: \n");
	//print_vector(b);
	int k = 1;
	float err = 1.0;
	
	float** ant = (float**) calloc(length_matrix,sizeof(float*));
	for (int i = 0; i < length_matrix; i++) 
        ant[i] = (float*) calloc(1,sizeof (float));
    
    copy_vector(ant,x);
    
	while ( (k <= kmax) && (err > tol)){
		
		for (int i = 0; i < length_matrix; i++){
			float sum = 0;
			for (int j = 0; j < i; j++)
				sum += A[i][j]*x[j][0];
			for (int j = i + 1; j < length_matrix; j++)
				sum += A[i][j]*x[j][0];
			
			x[i][0] = omega*(b[i][0] - sum)/A[i][i] + (1-omega)*ant[i][0];
			
		}
		//printf("vetor solucao iteracao %d\n\n", k);
			//print_vector(x);
		k++;
		err = error(x, ant);
		copy_vector(x, ant);
	}	
	clock_t end = clock();
	double time = (double)(end - begin)/CLOCKS_PER_SEC;
	printf("tempo de execucao: %.3lf\n", time);
	printf("norma da solucao: %f\n", max(x));
	printf("convergencia obtida com %d iteracoes\n\n", k);
	destroy_matrix(ant);
	return x;
}
