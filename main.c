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
    FILE * file;
	if(argc == 4)
    	 file = fopen(argv[3], "r");
    else if (argc == 3)
		file = fopen(argv[2], "r");
		
    if (file == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo da matrix!\n");
        return 0;
    }
    
    /*ARMAZENAMENTO DENSO*/
    if (!strcmp(argv[1], "-DENSO")) {
    	
    	if (!strcmp(argv[2], "LU")){
    		float ** matriz = read_matrix_MatrixMarket(file);
        
		    float ** matriz_transposta = transpose_matrix(matriz);

		    float** b = generate_b(matriz_transposta);   	

		    //alocando matriz U
		    float** U = create_matrix();

		    //alocando matriz L
		    float** L = create_matrix();

		    //alocando matrix de permutacoes
		    float** P = create_matrix_P();
		    

		    gauss_elimination(matriz_transposta, L, P);

		    create_utriangular(L);		    

		    copy_matrix(matriz_transposta, U);
    
		    float** y = solution_Ly_Pb(L, P, b);
		    
		    float** x = solution_Ux_y(U, y);
		    printf("SOLUCAO POR LU: \n");
		    print_vector(x);
			destroy_matrix(x);
			destroy_matrix(b);
		    destroy_matrix(y);
		    
		    destroy_matrix(L);
		    destroy_matrix(U);
		    destroy_matrix(matriz);
		    destroy_matrix(matriz_transposta);
		    destroy_matrix(P);
    	
    	}
    	else if (!strcmp(argv[2],"SOR")){
		    float ** matriz = read_matrix_MatrixMarket(file);
		    
		    float ** matriz_transposta = transpose_matrix(matriz);

		    float** b = generate_b(matriz_transposta);
		    	

		    
		
			float** x = SOR_solution(matriz_transposta, b);
			printf("SOLUCAO POR SOR: \n");
			print_vector(x);	
		
		    destroy_matrix(b);
		    destroy_matrix(x);
		    
		    
		    destroy_matrix(matriz);
		    destroy_matrix(matriz_transposta);
		    

		    return 0;
		}
    } else {
        float** matriz = read_matrix_MatrixMarket(file);
        
        float** matriz_transposta = transpose_matrix(matriz);
        float** b = generate_b(matriz_transposta);
        float** matriz_CSR = create_matrix_CSR(matriz_transposta);
        print_matrix_CSR(matriz_CSR);
        printf("coluna do elemento 1,1 : %d\n\n", get_column(matriz_CSR,1,2));
        float** x = SOR_solution_CSR(matriz_CSR, b);
        print_vector(x);
        
        return 0;

    }

    return (EXIT_SUCCESS);
}
