#!/bin/bash

gcc *.c

for name in {bcsstk03.mtx,hor131.mtx,gr_30_30.mtx,orsirr_1.mtx,sherman5.mtx}
do
	echo "<---------------------TESTE MATRIZ $name------------------->" >> log.txt
	echo >> log.txt
	echo "testando LU..." >> log.txt
	./a.out -DENSO LU $name >> log.txt
	for i in {1..9}
	do
		echo "testando SOR DENSO input$i..."
		echo "testando SOR DENSO input$i..." >> log.txt
		./a.out -DENSO SOR $name < input$i.txt >> log.txt  
		#echo "testando CSR..." >> log.txt
		#time ./a.out -CSR $name < input$i.txt >> log.txt
	done
	echo "---------------------TERMINOU------------------------" >> log.txt
	echo >> log.txt
done
