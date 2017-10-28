#ifndef DENSO_H
#define DENSO_H

#include <stdlib.h>
#include <stdio.h>

//gera b a partir da funcao do pdf
float** generate_b(float** A);

//aloca espaco para matrix
float** create_matrix();

//imprime o conteudo de um vetor
void print_vector(float** vector);

//cria matrix de permutacoes
float** create_matrix_P();

//imprime conteudo de matrix
void print_matrix(float** matrix);

//multiplica matrix por vetor
float** multi_matrix_vector(float** matrix, float** vector);

//copia conteudo de matrix para outra
void copy_matrix(float** matrix, float** copy);

//le as informacoes do arquivo
float ** read_matrix_MatrixMarket(FILE * file);

//calcula modulo de um valor
float mod(float n);


float** LU_solution(float** L, float** U, float** P, float** b); 

//faz Ux=y
float** solution_Ux_y(float** U, float** y);

//faz Ly=Pb
float** solution_Ly_Pb(float** L, float** P, float** b);

//encontra pivo de uma coluna
float pivot(float** matrix, int j);

//troca duas linhas
void swap(float** matrix, int row1, int row2);

//adiciona uma linha a outra
void add_row(float** matrix, int row1, int row2, float k);

//verifica se matrix foi corretamente eliminada
int eliminated(float** matrix);

//cria matrix triangular inferior
void create_utriangular(float** matrix);

//zera valores da coluna que estao abaixo do pivo
void clear_column(float** matrix, float** L, int column, float pivot);

//aplica o metodo de gauss
void gauss_elimination(float** matrix, float** L, float** P);

//foi usada apenas no comeco, nao deu tempo de apagar
float** transpose_matrix(float** matrix);

//desaloca espaco para matrix
void destroy_matrix(float** matrix);

//calcula norma infinita de v
float max(float** v);

int return_length_matrix();

void copy_vector(float** v1, float** v2);

float error(float** v2, float** v1);

float** sub_vector(float** v2 , float** v1);

int return_quantity_non_zeros();

float return_omega();

float return_tol();

float return_kmax();

float** SOR_solution(float** A, float** b);

#endif
