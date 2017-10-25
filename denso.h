#ifndef DENSO_H
#define DENSO_H

#include <stdlib.h>
#include <stdio.h>

float** generate_b(float** A);

float** create_matrix();

void print_vector(float** vector);

float** create_matrix_P();

void print_matrix(float** matrix);

float** multi_matrix_vector(float** matrix, float** vector);

void copy_matrix(float** matrix, float** copy);

float ** read_matrix_MatrixMarket(FILE * file);

float mod(float n);

float** LU_solution(float** L, float** U, float** P, float** b);

float** solution_Ux_y(float** U, float** y);

float** solution_Ly_Pb(float** L, float** P, float** b);

float pivot(float** matrix, int j);

void swap(float** matrix, int row1, int row2);

void add_row(float** matrix, int row1, int row2, float k);

int eliminated(float** matrix);

void create_utriangular(float** matrix);

void clear_column(float** matrix, float** L, int column, float pivot);

float** gauss_elimination(float** matrix, float** L, float** P);

float** transpose_matrix(float** matrix);

void destroy_matrix(float** matrix);

int return_length_matrix();

void copy_vector(float** v1, float** v2);

float error(float** v2, float** v1);

float** sub_vector(float** v2, float** v1);

int return_quantity_non_zeros();

float return_omega();

float return_tol();

float return_kmax();

float** SOR_solution(float** A, float** b);

#endif
