#ifndef CSR_H
#define CSR_H

#include <stdlib.h>
#include <stdio.h>
#include "denso.h"

float** create_matrix_CSR();

void print_matrix_CSR(float** matrix);

float** SOR_solution_CSR(float** matrix_CSR, float** b);

int number_elements_row(float** A, int row);

float access_CSR(float** A, int i, int j);
int get_column(float** A, int i, int j);

#endif /* CSR_H */

