#include "denso.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int length_matrix;
int number_rows_swapped = 0;
int pivot_row;
int quantity_non_zeros;
double omega;
int kmax;
double tol ;

double** generate_b(double** A){
	double** b = (double**) malloc(length_matrix* sizeof (double*));
    for (int i = 0; i < length_matrix; i++)
        b[i] = (double*) malloc( sizeof (double));
        
	for (int i = 0; i < length_matrix; i++){
		double sum = 0;
		for (int j = 0; j < length_matrix; j++){
			sum += A[i][j];
		}
		b[i][0] = sum;
	}
	return b;
}


double** create_matrix() {
    double** matrix = (double**) malloc(length_matrix * sizeof (double*));

    for (int i = 0; i < length_matrix; i++) {
        matrix[i] = (double*) malloc(length_matrix * sizeof (double));
    }

    return matrix;
}

//allocate space for vector matrix[length][1]



void print_vector(double** vector) {
    for (int i = 0; i < length_matrix; i++)
        printf("| %lf |\n", vector[i][0]);
}

double** multi_matrix_vector(double** A, double** x) {

    double** c = (double**) calloc(length_matrix, sizeof (double*));
    for (int i = 0; i < length_matrix; i++)
        c[i] = (double*) calloc(1, sizeof (double));

    for (int i = 0; i < length_matrix; i++)
        for (int k = 0; k < length_matrix; k++)
            c[i][0] += A[i][k] * x[k][0];

    return c;
}

//solve Ly=Pb

double** solution_Ly_Pb(double** L, double** P, double** b) {
    double** c = multi_matrix_vector(P, b);


    double** y = (double**) malloc(length_matrix * sizeof (double*));
    for (int i = 0; i < length_matrix; i++)
        y[i] = (double*) malloc(sizeof (double));

    for (int i = 0; i < length_matrix; i++) {
        y[i][0] = c[i][0];
        for (int j = 0; j < i; j++)
            y[i][0] -= L[i][j] * y[j][0];

    }

    destroy_matrix(c);
    return y;
}

double** solution_Ux_y(double** U, double** y) {
    double** x = (double**) malloc(length_matrix * sizeof (double*));
    for (int i = 0; i < length_matrix; i++)
        x[i] = (double*) malloc(sizeof (double));

    for (int i = length_matrix - 1; i >= 0; i--) {
        x[i][0] = y[i][0];
        for (int j = i + 1; j < length_matrix; j++) {
            x[i][0] -= U[i][j] * x[j][0];
        }
        x[i][0] /= U[i][i];
    }

    return x;
}

double** create_matrix_P() {
    double** matrix = (double**) calloc(length_matrix, sizeof (double*));

    for (int i = 0; i < length_matrix; i++)
        matrix[i] = (double*) calloc(length_matrix, sizeof (double));

    for (int i = 0; i < length_matrix; i++)
        matrix[i][i] = 1;

    return matrix;
}

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

double ** read_matrix_MatrixMarket(FILE * file) {
	printf("tolerancia:"); scanf("%lf%*c", &tol);
    printf("numero max de iteracoes: ");scanf("%d%*c", &kmax);
    printf("omega: ");scanf("%lf%*c", &omega);
    fscanf(file, "%*[^\n]%*c");
    fscanf(file, "%d%*c%d%*c%d%*c", &length_matrix, &length_matrix, &quantity_non_zeros);
    double** matrix = (double**) calloc(length_matrix, sizeof (double*));

    int tam = length_matrix, qnt = quantity_non_zeros;

    for (int i = 0; i < length_matrix; i++) {
        matrix[i] = (double*) calloc(length_matrix, sizeof (double));
    }
    int linha = 0, coluna = 0;
    double valor = 0;

    while (fscanf(file, "%d%*c%d%*c%*c%lf%*c", &linha, &coluna, &valor) != EOF) {
        matrix[linha-1][coluna-1] = valor;
    }
    fclose(file);

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
        if (j <= row1) {
            matrix[row2][j] = 0;
            continue;
        }
        matrix[row2][j] = matrix[row2][j] + (-1) * k * matrix[row1][j];
    }
}

int eliminated(double** matrix) {
    for (int i = 1; i < length_matrix; i++)
        for (int j = 0; j < i; j++)
            if (matrix[i][j])
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

double** gauss_elimination(double** matrix, double** L, double** P) {
    for (int j = 0; j < length_matrix; j++) {
        double pivo = pivot(matrix, j);
        if (matrix[j][j] != pivo) {
            swap(matrix, j, pivot_row);
            swap(P, j, pivot_row);
            swap(L, j, pivot_row);
        }
        clear_column(matrix, L, j, pivo);
    }
}

double** transpose_matrix(double** matrix) {
    double** transpose_matrix = create_matrix();

    for (int i = 0; i < length_matrix; i++) {
        for (int j = 0; j < length_matrix; j++) {
            transpose_matrix[i][j] = matrix[j][i];
        }
    }
    return transpose_matrix;
}

void destroy_matrix(double** matrix) {

    for (int i = 0; i < length_matrix; i++)
        free(matrix[i]);
    free(matrix);
}

int retorna_length_matrix() {
    return length_matrix;
}

int retorna_quantity_non_zeros() {
    return quantity_non_zeros;
}

void copy_vector(double** v1, double** v2){
	for (int i = 0; i < length_matrix ; i++)
		v2[i][0] = v1[i][0];
}

double max(double** v){
	double max = -FLT_MAX;
	
	for (int i = 0 ; i < length_matrix; i++){
		
		
		if (v[i][0] > max)
			max = v[i][0];
	}
	
	return max;
}

double** sub_vector(double** v2 , double** v1){
	double** sub = (double**) malloc(length_matrix*sizeof(double*));
	for (int i = 0; i < length_matrix; i++) 
        sub[i] = (double*) malloc(sizeof (double));
        
	for (int i = 0; i < length_matrix; i++){
		
			sub[i][0] = v2[i][0] - v1[i][0];
		
	}
	return sub;
}

double error(double** v2, double** v1){
	double e;
    
    double** sub = sub_vector(v2,v1);
    
    e = max(sub);
    free(sub);	
	e /= max(v2);
	
	return e;
}

double** SOR_solution(double** A, double** b){
	//alocando vetor solução x 
	double** x = (double**) malloc(length_matrix*sizeof(double*));
	for (int i = 0; i < length_matrix; i++) 
        x[i] = (double*) calloc(1,sizeof (double));
	
	printf("vetor b: \n");
	print_vector(b);
	int k = 0;
	double err = 1.0;
	
	double** ant = (double**) malloc(length_matrix*sizeof(double*));
	for (int i = 0; i < length_matrix; i++) 
        ant[i] = (double*) calloc(1,sizeof (double));
    
	while ( k < kmax && err > tol){
		
		for (int i = 0 ; i < length_matrix  ; i++){
			
			
			
			for (int j = 0; j < length_matrix; j++){
				if(i != j)
					x[i][0] -= A[i][j]*x[j][0];
				else 
					continue;
			}
			x[i][0] += b[i][0];
			x[i][0] *= omega/A[i][i];
			x[i][0] += (1-omega)*ant[i][0];
			
			//printf("iteracao %d x%d:%lf\n", k, i, x[i][0]);
		}
		k++;
		err = error(x,ant);
		copy_vector(x, ant);	
		printf("vector x%d\n", k-1);
    	print_vector(ant);
	}	
		
	destroy_matrix(ant);
	return x;
}
