
#include<stdio.h>
#include<stdlib.h>
#include "doubleFunctions.h"

void displayDoubleMatrix(float* matrix, int numCount){
	for(int i=0;i<numCount;i++){
		printf("%f\n",matrix[i]);
	}	
}

float* makeDoubleMatrix(int size, FILE* file){
	float* arr = malloc(sizeof(float)*size);
	for(int i=0;i<size;i++){
		if(feof(file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(file,"%f",&arr[i]);
		if(arr[i]==0.0)i--;
	}
	return arr;	
}