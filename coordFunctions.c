//coordFunctions.c 
//cits3402  
//assignment1
//author: jradich

#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"matrixStructures.h"
#include"coordFunctions.h"

//scan file and store non zero values in coordinate format
void makeCoordMatrix(struct FileInfo* fInfo){
	int arrSize = fInfo->size;
	int** coordMatrix = malloc(sizeof(int*)*arrSize);
	float* valMatrix = malloc(sizeof(int)*arrSize);
	int row = 0;
	int col = 0;
	float val = 0;
	int totCount = 1;
	
	for(int i=0;i<arrSize;i++){
		
		if(feof(fInfo->file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(fInfo->file,"%f\n",&val);
		if(val ==0){
			i--;
		}
		else{
			//if value is non zero, insert actual value, row and column 
			coordMatrix[i] = (int*)malloc(sizeof(int)*3);
			row = totCount / fInfo->cols;
			col = totCount % fInfo->cols;
			if(col ==0){
				col = fInfo->cols;
			}
			else{
				row++;
			}
			coordMatrix[i][0] = row-1;
			coordMatrix[i][1] = col-1;
			valMatrix[i] = val;
		}
		totCount++;
	}
	fInfo->matrix = coordMatrix;
	fInfo->valMatrix = valMatrix;
	
}

//scalar multiply - multiplies all value elements by scalar value 
void scalarMultiply(struct FileInfo* f1, float scalar,int threads){
	
	#pragma omp parallel for num_threads(threads)
	for(int i=0; i<f1->size; i++){
		f1->valMatrix[i] *= scalar;
	}
}

//print the coordinate matrix to file in dense format (with 0s included) 
void printDenseCoordMatrix(struct FileInfo* f1, FILE* file){
	int matrixCounter =0;
	float val = 0;
	char printFormat[10];
	strcpy(printFormat,f1->printToken);  //print as either int value or float 
	strcat(printFormat," ");
	for(int i=0; i<f1->rows; i++){   // print a value for all rows and cols 
		for(int j=0; j<f1->cols; j++){
			val = 0;  
			if(matrixCounter<f1->size){  //if theres actually a value for it, print that instead of 0
				if(f1->matrix[matrixCounter][0]==i && f1->matrix[matrixCounter][1]==j){
					val = f1->valMatrix[matrixCounter]; 
					matrixCounter++;
				}
			}		
			fprintf(file,printFormat,val);
		}
		fprintf(file,"\n");
	}
}

//trace calculation - adds the diagonal.  
float traceCoordCalc(struct FileInfo* fInfo,int threads){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int** matrix = fInfo->matrix;
	float tot = 0;
	int m = 0;
	if(rows != cols){ 
		printf("Trace can only be performed on square matricies\n");
		exit(0);
	}
	else{
		#pragma omp parallel for reduction(+:tot) num_threads(threads)
		for(int i=0;i<size;i++){
			if(matrix[i][0] ==  matrix[i][1]){  //if row value and col value are equal, its a diagonal element.  
				tot+= fInfo->valMatrix[i];
			}
		}
	}
	return tot;
}

//addition of 2 matrixes 
void coordMatrixAddition(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3){
	
	if(f1->rows != f2->rows || f1->cols != f2->cols){
		printf("Matrix dimensions must be the same\n");
		exit(0);
	}
	int size1 = f1->size;
	int size2 = f2->size;
	
	int** matrix3 = malloc(sizeof(int*)*(size1+size2));  //might be oversize but will fit worst case scenario
	float* valMatrix3 = malloc(sizeof(float)*(size1+size2));
	float val = 0;
	int size3 = 0;
	int s1Count=0;
	int s2Count=0;
	int nextRow = 0;
	int nextCol = 0;
	int** matrix1 = f1->matrix;
	int** matrix2 = f2->matrix;
	
	//while either matrix is not finished 
	while(s1Count < size1 || s2Count < size2){
		
		//if either matrix is finished but the other still has values, use that value 
		if(s2Count>=size2){
			nextRow = matrix1[s1Count][0];
			nextCol = matrix1[s1Count][1];
			val = f1->valMatrix[s1Count];
			s1Count++;
		}
		else if(s1Count>= size1){
			nextRow = matrix2[s2Count][0];
			nextCol = matrix2[s2Count][1];
			val = f2->valMatrix[s2Count];
			s2Count++;
		}
		else{  //if both matrixes still have values, check if they are in the same location 
		
			if(matrix1[s1Count][0] < matrix2[s2Count][0]){
				nextRow = matrix1[s1Count][0];
				nextCol = matrix1[s1Count][1];
				val = f1->valMatrix[s1Count];
				s1Count++;
			}		
			else if(matrix1[s1Count][0] > matrix2[s2Count][0]){
				nextRow = matrix2[s2Count][0];
				nextCol = matrix2[s2Count][1];
				val = f2->valMatrix[s2Count];
				s2Count++;
			}	
			else{
				nextRow = matrix1[s1Count][0];
				if(matrix1[s1Count][1] < matrix2[s2Count][1]){
					nextCol = matrix1[s1Count][1];
					val = f1->valMatrix[s1Count];
					s1Count++;
				}
				else if(matrix1[s1Count][1] > matrix2[s2Count][1]){
					nextCol = matrix2[s2Count][1];
					val = f2->valMatrix[s2Count];
					s2Count++;
				}
				else{ //values are in same location so add them together 
					nextCol = matrix1[s1Count][1];
					val = f1->valMatrix[s1Count] + f2->valMatrix[s2Count];
					s1Count++;
					s2Count++;
				}
			}
		}
		//put values in new matrix. 
		matrix3[size3] = (int*)malloc(sizeof(int)*2);
		matrix3[size3][0] = nextRow;
		matrix3[size3][1] = nextCol;
		valMatrix3[size3] = val;
		
		size3++;
		
	}
	//copy other required values to new matrix mata data 
	f3->cols = f1->cols;
	f3->rows = f1->rows;
	f3->matrix = matrix3;
	f3->valMatrix = valMatrix3;
	f3->size = size3;
	strcpy(f3->printToken,f1->printToken);
	
}

//swap the row and column values of each element 
void transposeMatrixMP(struct FileInfo* f1,int threads){
	
	float** reg = malloc(sizeof(float*)*f1->rows);
	
	//parellel initialisation of matrix as doesnt need to be done in order. 
	#pragma omp parallel for num_threads(threads)
	for(int i=0;i<f1->rows;i++){
		reg[i] = (float*)malloc(sizeof(float)*f1->cols);
		for(int j = 0; j< f1->cols;j++){
			reg[i][j]=0;
		}
	}
	
	//go through matrix and put them in the registry matrix based on their column value 
	#pragma omp parallel for num_threads(threads)
	for(int i=0; i<f1->size;i++){
		reg[f1->matrix[i][1]][f1->matrix[i][0]] = f1->valMatrix[i];
	}
	
	int c =0;
	//stitch registry matrix together where columns and now the rows.  
	for(int i=0;i<f1->rows;i++){  //cant be threaded as has to go in order. 
		for(int j =0;j<f1->cols;j++){
			if(reg[i][j] != 0){
				f1->matrix[c][0] = i;
				f1->matrix[c][1] = j;
				f1->valMatrix[c] = reg[i][j];
				c++;
			}
		}
	}
	
	#pragma omp parallel for num_threads(threads)
	for(int i=0;i<f1->rows;i++){
		free(reg[i]);
	}
	free(reg);
	
}

//matrix multiplication 
void coordMatrixMultiply(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3, int threads){
	if(f1->cols != f2->rows){
		printf("Columns of Matrix 1 and rows of Matrix 2 must be equal\n");
		exit(0);
	}
	
	int c1=0, c2=0, c3=0, startPoint = 0;
	float val = 0, totVal = 0;
	int size3 = f1->rows * f2->cols;  //worst case may result in full matrix.
	int regSize = f1->rows > f1->cols ? f1->rows : f1->cols;
	int** matrix1 = f1->matrix;
	int** matrix2 = f2->matrix;
	int** matrix3 = malloc(sizeof(int*)*size3);
	float* valMatrix3 = malloc(sizeof(float)*size3);
	float reg[regSize];
	
	//set new matrix meta data 
	f3->rows = f1->rows;
	f3->cols = f2->cols;
	f3-> matrix = matrix3;
	f3->valMatrix = valMatrix3;
	strcpy(f3->printToken,f1->printToken);

	#pragma omp parallel for firstprivate(c1,val,startPoint,c3,totVal,reg) num_threads(threads)
	for(int i=0; i<f1->rows; i++){ //go through first matrix row by row 
		startPoint = c1;
		for(int j=0; j<f2->cols; j++){ // go through second matrix col by col 
			
			c1=startPoint;
			totVal=0;
			
			for(int k = 0;k<regSize;k++){
				reg[k]=0;
			}

			for(int k=0;k<f2->size;k++){ //store values that are in that column in a registry array so the entire
				if(matrix2[k][1] == j){  //matrix array doesn't have to be traversed every time 
					reg[matrix2[k][0]] = f2->valMatrix[k];
				}
			}
		
			while(c1 < f1->size && matrix1[c1][0] <= i){  // so through and see if there are multiplication values 
				if(matrix1[c1][0] == i){                  //for each cell in that row and column 
					val = f1->valMatrix[c1] * reg[matrix1[c1][1]];
					totVal += val;
					
				}								
				c1++;
			}
			
			//store new value in new matrix 
			c3 = (i *f3->cols) + j;
			matrix3[c3] = (int*)malloc(sizeof(int)*2);
			matrix3[c3][0] = i;
			matrix3[c3][1] = j;
			valMatrix3[c3] = totVal;
		}
	}
	
	f3->size = size3;
}





