#ifndef DENSO_H
#define DENSO_H

#include <stdlib.h>
#include <stdio.h>

double** create_matrix();

double** read_vector(FILE * file);

void print_vector(double** vector);

double** create_matrix_P();

void print_matrix(double** matrix);

double** multi_matrix_vector(double** matrix, double** vector);

void copy_matrix(double** matrix, double** copy);

double ** read_matrix_MatrixMarket(FILE * file);

double mod(double n);

double** LU_solution(double** L, double** U, double** P, double** b); 

double** solution_Ux_y(double** U, double** y);

double** solution_Ly_Pb(double** L, double** P, double** b);

double pivot(double** matrix, int j);

void swap(double** matrix, int row1, int row2);

void add_row(double** matrix, int row1, int row2, double k);

int eliminated(double** matrix);

void create_utriangular(double** matrix);

void clear_column(double** matrix, double** L, int column, double pivot);

double** gauss_elimination(double** matrix, double** L, double** P);

double** transpose_matrix(double** matrix);

void destroy_matrix(double** matrix);

#endif