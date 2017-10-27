#ifndef CSR_H
#define CSR_H

#include <stdlib.h>
#include <stdio.h>
#include "denso.h"

void destroy_matrixx(float** matrix);

void copy_vectorr(float** v1, float** v2);

float maxx(float** v);

float errorr(float** v2, float** v1);

void create_matrix_CSR(FILE * file, float** matrix_CSR, float* AA, float* JA, float* IA);

void destroy_matrix_CSR(float** m);

void generate_b_CSR(float** matrix,float** b);

int return_length_matrix_CSR();

int return_quantity_non_zeros_CSR();

float return_omega_CSR();

float return_tol_CSR();

float return_kmax_CSR();

void print_matrix_CSR(float** matrix);

void SOR_solution_CSR(float** matrix_CSR, float** b, float** x);

int number_elements_row(float** A, int row);

float access_CSR(float** A, int i, int j);

void set_global(FILE * file);

int get_column(float** A, int i, int j);

#endif /* CSR_H */
	
