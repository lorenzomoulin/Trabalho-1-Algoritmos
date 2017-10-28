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
    	
    	
        float *AA = (float*) malloc(quantity_non_zeros * sizeof (float));
    	float *JA = (float*) malloc(quantity_non_zeros * sizeof (float));
    	float *IA = (float*) malloc((length_matrix + 1) * sizeof (float));
    	
        float** matrix_CSR = (float**) malloc(3* sizeof (float*));create_matrix_CSR(file,matrix_CSR,AA,JA,IA);
        
       	
       	float** b = (float**)calloc(quantity_non_zeros,sizeof(float*));
       	for (int i = 0; i < length_matrix ; i++)
       		b[i] = (float*)calloc(1,sizeof(float));
       		
       	generate_b_CSR(matrix_CSR,b);
       	
       	float** x = (float**)malloc(quantity_non_zeros*sizeof(float*));
       	for (int i = 0; i < length_matrix ; i++)
       		x[i] = (float*)malloc(sizeof(float));
       		
       	clock_t begin = clock();
       	SOR_solution_CSR(matrix_CSR, b, x);
       	clock_t end = clock();
       	
		printf("tempo de execucao em segundos: %.3lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
        
        destroy_matrixx(b);
        free(AA);
        free(JA);
        free(IA);
        free(matrix_CSR);
        destroy_matrixx(x);
        fclose(file);
        //destroy_matrixx();
        
        return 0;

    }
	
    return (EXIT_SUCCESS);
}
