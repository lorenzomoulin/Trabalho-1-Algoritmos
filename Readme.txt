Readme - Trabalho 1 de algoritmos numéricos

O código foi dividido entre o armazenamento denso e 
armazenamento csr além de separar o denso em solução LU e solução SOR.

Instruções de compilação do código: 
Entre no terminal no diretório dos arquivos .c e digite: "gcc -o trabalho *.c" - sem aspas
Será gerado um arquivo trabalho.out e ele poderá ser executado.

Instruções de execução do código:

	Armazenamento denso: 
		Solução LU:
			"./trabalho -DENSO LU exemplo.mtx" - sem aspas
		Solução SOR:
			"./trabalho -DENSO SOR exemplo.mtx" - sem aspas
	Armazenamento csr: 
		Solução SOR:
			"./trabalho -CSR SOR exemplo.mtx" - sem aspas
			
Arquivos de teste: 
Na leitura dos arquivos que contêm a matriz a ser utilizada será ignorada a primeira linha do arquivo .mtx.
Ou seja, executar o programa com arquivos que contêm apenas a primeira linha como comentário.

Exemplo de arquivo de teste:

	%%MatrixMarket matrix coordinate real general
	2 2  4
	1 1  -1
	2 1  8
	1 2  2
	2 2  5
	
Exemplo de compilação e execução do código:

	compilando... 		gcc -o trabalho *.c
	executando... 		./trabalho -DENSO SOR fs1831.mtx
	
Código feito por: 
	Lorenzo Moulin e Daniel Duque
