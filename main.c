#include <string.h>
#include <stdio.h>
#include "CSR.h"
#include "denso.h"
//program works with square matrix

int main(int argc, char** argv) {


    if ((argv[1] == NULL) || (strcmp(argv[1], "-CSR") && strcmp(argv[1], "-DENSO"))) {
        printf("ERRO: Nao escolheu o tipo de armazenamento\n");
        return 0;
    }
    if (argv[2] == NULL) {
        printf("ERRO: Nao escolheu arquivo da matrix\n");
        return 0;
    }
    

    FILE * file = fopen(argv[2], "r");
    

    if (file == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo da matrix!\n");
        return 0;
    }
    
    /*ARMAZENAMENTO DENSO*/
    if (!strcmp(argv[1], "-DENSO")) {
    	
    	
    	
        double ** matriz = read_matrix_MatrixMarket(file);
        
        double ** matriz_transposta = transpose_matrix(matriz);

        double** b = generate_b(matriz_transposta);
        	

        //alocando matriz U
        double** U = create_matrix();

        //alocando matriz L
        double** L = create_matrix();

        //alocando matrix de permutacoes
        double** P = create_matrix_P();
        ;

        gauss_elimination(matriz_transposta, L, P);

        create_utriangular(L);
        

        copy_matrix(matriz_transposta, U);
        


        
        double** y = solution_Ly_Pb(L, P, b);
        
        double** x = solution_Ux_y(U, y);
        printf("SOLUCAO POR LU: \n");
        print_vector(x);
		destroy_matrix(x);
		
		x = SOR_solution(matriz_transposta, b);
		printf("SOLUCAO POR SOR: \n");
		print_vector(x);	
		
        destroy_matrix(b);
        destroy_matrix(y);
        destroy_matrix(x);
        destroy_matrix(L);
        destroy_matrix(U);
        destroy_matrix(matriz);
        destroy_matrix(matriz_transposta);
        destroy_matrix(P);

        return 0;

    } else  {
        double ** matriz = read_matrix_MatrixMarket(file);
        
        double ** matriz_transposta = transpose_matrix(matriz);
        double** b = generate_b(matriz_transposta);
        double ** matriz_CSR = create_matrix_CSR(matriz_transposta);
        
        print_matrix_CSR(matriz_CSR);
        return 0;

    }

    return (EXIT_SUCCESS);
}
