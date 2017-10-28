#!/bin/bash
#passe como parametro o numero de iteracoes
#este programa vai criar 9 arquivos variando o omega de 0.2 a 1.8

for i in {1..9}
do 
	touch input$i.txt
	echo "0.00001" >> input$i.txt
	echo "$1" >> input$i.txt
	expr 0.2 '*' $i >> input$i.txt
done
