#include <string.h>
#include <stdio.h>
#include "CSR.h"
#include "denso.h"
#include <time.h>
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

		    float** b = generate_b(matriz);   	

		    //alocando matriz U
		    float** U = create_matrix();

		    //alocando matriz L
		    float** L = create_matrix();

		    //alocando matrix de permutacoes
		    float** P = create_matrix_P();
		    
			clock_t begin = clock();
		    gauss_elimination(matriz, L, P);
			
		    create_utriangular(L);		    

		    copy_matrix(matriz, U);
    		
		    float** y = solution_Ly_Pb(L, P, b);
		    destroy_matrix(b);
		    destroy_matrix(L);
		    destroy_matrix(P);
		    destroy_matrix(matriz);
		    //print_matrix(U);
		    //print_matrix(L);
		    float** x = solution_Ux_y(U, y);
		    clock_t end = clock();
		    double time = (double)(end - begin)/CLOCKS_PER_SEC;
		    printf("tempo de execucao em segundos: %.3lf\n", time);
		    printf("norma da solucao : %f\n\n", max(x));
			destroy_matrix(x);
			
		    destroy_matrix(y);
		    
		    
		    destroy_matrix(U);
		    
		    
		    
    	
    	}
    	else if (!strcmp(argv[2],"SOR")){
		    float ** matriz = read_matrix_MatrixMarket(file);
		    
		    

		    float** b = generate_b(matriz);		    
		
			float** x = SOR_solution(matriz, b);
			//printf("norma da solucao : %f\n", max(x));
		    destroy_matrix(b);
		    destroy_matrix(x);
		    
		    
		    destroy_matrix(matriz);
		    

		    return 0;
		}
    } else {
        
        
        
    	//ignora primeira linha
    	fscanf(file, "%*[^\n]%*c");
    	
    	set_global(file);
    	
    	int length_matrix = return_length_matrix_CSR();
    	int quantity_non_zeros = return_quantity_non_zeros_CSR();
    	
    	float** matrix =read_matrix_MatrixMarket_CSR(file);
        
        float** matrix_CSR = create_matrix_CSR(matrix);
        destroy_matrixx(matrix);
    	
    	
        
        
       	print_matrix_CSR(matrix_CSR);
       	
       	float** b = (float**)malloc(quantity_non_zeros*sizeof(float*));
       	for (int i = 0; i < length_matrix ; i++)
       		b[i] = (float*)malloc(sizeof(float));
       		
       	generate_b_CSR(matrix_CSR,b);
       	//print_vector(b);
       	float** x = (float**)malloc(quantity_non_zeros*sizeof(float*));
       	for (int i = 0; i < length_matrix ; i++)
       		x[i] = (float*)malloc(sizeof(float));
       	
       	SOR_solution_CSR(matrix_CSR, b, x);
        //print_vector(x);
        destroy_matrixx(b);
        //destroy_matrixx(x);
        //destroy_matrix_CSR(matrix_CSR);
        
        //printf("coluna do elemento 1,1 : %d\n\n", get_column(matriz_CSR,1,2));
        //float** x = SOR_solution_CSR(matriz_CSR, b);
        //print_vector(x);
        
        return 0;

    }
	
    return (EXIT_SUCCESS);
}
