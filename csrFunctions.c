#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "matrixStructures.h"
#include "csrFunctions.h"


void makeCSRMatrix(struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	float val = 0;
	int col = 0;
	int j = 0;
	int rowCount = 0;
	int totCount = 1;
	float* valMatrix = malloc(sizeof(float)*size);
	int** CSRMatrix = malloc(sizeof(int*)*2);
	CSRMatrix[0] = (int*)malloc(sizeof(int)*size);
	CSRMatrix[1] = (int*)malloc(sizeof(int)*(rows+1));
	
	CSRMatrix[1][0] = 0; j++;
	for(int i=0;i<size;i++){
		if(feof(fInfo->file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(fInfo->file,"%f\n",&val);
		col = totCount % cols;
		if(col ==0){
			col = cols;
		}
		if(val == 0){
			i--;
		}
		else{
			valMatrix[i] = val;
			CSRMatrix[0][i] = col - 1;
			rowCount++;
		}
		if(col == cols){
			CSRMatrix[1][j] = rowCount;
			j++;
		}
		totCount++;
	}
	while(j<=rows){ //if above calc doesn't make it to the end because there's lots of 0s, fill in the rest of the totals array
		CSRMatrix[1][j] = rowCount;
		j++;
	}
	fInfo->valMatrix = valMatrix;
	fInfo->matrix= CSRMatrix;
} 

float traceCSRCalc(struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int** matrix = fInfo->matrix;
	int val = 0;
	int j = 0;
	int count = 0;
	float tot = 0;
	if(rows != cols){
		printf("Trace can only be performed on square matricies\n");
		exit(0);
	}
	else{
		for(int i=0;i<rows;i++){
			val = matrix[1][i+1] - matrix[1][i];
			if(val > 0){
				count = 0;
				while(count < val && matrix[2][j + count] <= i ){
					if(matrix[2][j + count] == i){
						tot += fInfo->valMatrix[j+count];
						count = val; 
					}
					count++;
				}
			}
			j+=val;
		}
	}
	return tot;
}

void printDenseCSRMatrix(struct FileInfo* fInfo, FILE* f){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;	
	int numInRow = 0;
	int totCount = 0;
	int count =0;
	float val = 0;
	int** matrix = fInfo->matrix;
	char printFormat[10];
	strcpy(printFormat,fInfo->printToken);
	strcat(printFormat," ");
	
	for(int j=0;j<rows;j++){
		numInRow = matrix[1][j+1] - matrix[1][j];
		count = 0;
		for(int i=0;i<cols;i++){
			val = 0;
			if(count < numInRow && matrix[2][totCount + count] == i){
				fInfo->valMatrix[totCount+count];
				count++;
			}
			fprintf(f,printFormat,val);
		}
		fprintf(f,"\n");
		totCount += count;
	}
}