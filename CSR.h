#ifndef CSR_H
#define CSR_H

#include <stdlib.h>
#include <stdio.h>
#include "denso.h"
//funcao que desaloca memoria de matriz
void destroy_matrixx(float** matrix);

//copia o conteudo de v1 para v2
void copy_vectorr(float** v1, float** v2);

//calcula a norma infinita de v
float maxx(float** v);

//calcula o erro: maxx(v2-v1)/maxx(v2)
float errorr(float** v2, float** v1);

//le do arquivo mtx as informacoes da matrix e gera o csr
void create_matrix_CSR(FILE * file,float** matrix_CSR, float* AA, float* JA, float* IA);

//desaloca memoria de matrix csr
void destroy_matrix_CSR(float** m);

//gera o b a partir da funcao do pdf
void generate_b_CSR(float** matrix_CSR,float** b);

//apenas retornando variaveis globais
int return_length_matrix_CSR();

int return_quantity_non_zeros_CSR();

float return_omega_CSR();

float return_tol_CSR();

float return_kmax_CSR();

//imprime o conteudo de AA, JA, IA
void print_matrix_CSR(float** matrix);

//calcula o x
void SOR_solution_CSR(float** matrix_CSR, float** b, float** x);

//conta numero de elementos em uma linha da matrix csr
int number_elements_row(float** A, int row);

//acessa o j-esimo elemento da linha i da matrix A
float access_CSR(float** A, int i, int j);

//atribui valores a variaveis globais
void set_global(FILE * file);

//devolve a coluna do j-esimo elemento da linha i da matrix A
int get_column(float** A, int i, int j);

#endif /* CSR_H */
	
