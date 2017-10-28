#!/bin/bash

for i in {1..9}
do 
	touch input$i.txt
	echo "0.00001" >> input$i.txt
	echo "1000000" >> input$i.txt
	expr 0.2 '*' $i >> input$i.txt
done
