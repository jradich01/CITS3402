#include<stdio.h>
#include<stdlib.h>
#include"matrixStructures.h"
#include"floatFunctions.h"

void makeCoordMatrixF(struct FileInfo* fInfo){
	int arrSize = fInfo->size;
	float** coordMatrix = malloc(sizeof(float*)*arrSize);
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
			coordMatrix[i] = (float*)malloc(sizeof(float)*3);
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
			coordMatrix[i][2] = val;
		}
		totCount++;
	}
	fInfo->matrixF = coordMatrix;
	
}

void displayCoordMatrixF(struct FileInfo* f1){
	for(int i=0;i<f1->size;i++){
		printf("Row: %d  Col: %d Value: %f\n",f1->matrixF[i][0],f1->matrixF[i][1],f1->matrixF[i][2]);
	}
}

void makeCSRMatrixF(struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int val = 0;
	int col = 0;
	int j = 0;
	int rowCount = 0;
	int totCount = 1;
	int** CSRMatrix = malloc(sizeof(int*)*3);
	CSRMatrix[0] = (int*)malloc(sizeof(int)*size);
	CSRMatrix[1] = (int*)malloc(sizeof(int)*(rows+1));
	CSRMatrix[2] = (int*)malloc(sizeof(int)*size);
	CSRMatrix[1][0] = 0; j++;
	for(int i=0;i<size;i++){
		if(feof(fInfo->file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(fInfo->file,"%d\n",&val);
		col = totCount % cols;
		if(col ==0){
			col = cols;
		}
		if(val == 0){
			i--;
		}
		else{
			CSRMatrix[0][i] = val;
			CSRMatrix[2][i] = col -1;
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
	
	fInfo->matrix= CSRMatrix;
}

void scalarMultiplyF(struct FileInfo* f1, float scalarF){
	for(int i=0; i<f1->size; i++){
		//printf("%f\n",f1->matrixF[i][2]);
		f1->matrixF[i][2] *= scalarF;
		//printf("%f\n",f1->matrixF[i][2]);
	}
}

void printDenseCoordMatrixF(struct FileInfo* f1, FILE* file){
	int matrixCounter =0;
	float val = 0;
	for(int i=0; i<f1->rows; i++){
		for(int j=0; j<f1->cols; j++){
			val = 0;
			if(matrixCounter<f1->size){
				if(f1->matrixF[matrixCounter][0]==i && f1->matrixF[matrixCounter][1]==j){
					val = f1->matrixF[matrixCounter][2]; 
					matrixCounter++;
				}
			}
			fprintf(file,"%f ",val);
		}
		fprintf(file,"\n");
	}
}

int traceCoordCalcF(struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int** matrix = fInfo->matrix;
	int tot = 0;
	int m = 0;
	if(rows != cols){
		printf("Trace can only be performed on square matricies\n");
		exit(0);
	}
	else{
		for(int i=0;i<rows;i++){
			while(m < size && (matrix[m][0] <= i || matrix[m][1] <= i) ){
				if(matrix[m][0] == i && matrix[m][1] == i){
					tot+= matrix[m][2];
				}
				m++;
			}
		}
	}
	return tot;
}

int traceCSRCalcF(struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int** matrix = fInfo->matrix;
	int val = 0;
	int j = 0;
	int count = 0;
	int tot = 0;
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
						tot+= matrix[0][j + count];
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

void printDenseCSRMatrixF(struct FileInfo* fInfo, FILE* f){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;	
	int numInRow = 0;
	int totCount = 0;
	int count =0;
	int val = 0;
	int** matrix = fInfo->matrix;
	
	for(int j=0;j<rows;j++){
		numInRow = matrix[1][j+1] - matrix[1][j];
		count = 0;
		for(int i=0;i<cols;i++){
			val = 0;
			if(count < numInRow && matrix[2][totCount + count] == i){
				val = matrix[0][totCount + count];
				count++;
			}
			fprintf(f,"%d ",val);
		}
		fprintf(f,"\n");
		totCount += count;
	}
}

void coordMatrixAdditionF(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3){
	
	if(f1->rows != f2->rows || f1->cols != f2->cols){
		printf("Matrix dimensions must be the same\n");
		exit(0);
	}
	int size1 = f1->size;
	int size2 = f2->size;
	
	int** matrix3 = malloc(sizeof(int*)*(size1+size2));  //might be oversize but will fit worst case scenario
	int val = 0;
	int size3 = 0;
	int s1Count=0;
	int s2Count=0;
	int nextRow = 0;
	int nextCol = 0;
	int** matrix1 = f1->matrix;
	int** matrix2 = f2->matrix;
	
	while(s1Count < size1 || s2Count < size2){
		if(matrix1[s1Count][0] < matrix2[s2Count][0]){
			nextRow = matrix1[s1Count][0];
			nextCol = matrix1[s1Count][1];
			val = matrix1[s1Count][2];
			s1Count++;
		}		
		else if(matrix1[s1Count][0] > matrix2[s2Count][0]){
			nextRow = matrix2[s2Count][0];
			nextCol = matrix2[s2Count][1];
			val = matrix2[s2Count][2];
			s2Count++;
		}	
		else{
			nextRow = matrix1[s1Count][0];
			if(matrix1[s1Count][1] < matrix2[s2Count][1]){
				nextCol = matrix1[s1Count][1];
				val = matrix1[s1Count][2];
				s1Count++;
			}
			else if(matrix1[s1Count][1] > matrix2[s2Count][1]){
				nextCol = matrix2[s2Count][1];
				val = matrix2[s2Count][2];
				s2Count++;
			}
			else{
				nextCol = matrix1[s1Count][1];
				val = matrix1[s1Count][2] + matrix2[s2Count][2];
				s1Count++;
				s2Count++;
			}
		}
		matrix3[size3] = (int*)malloc(sizeof(int)*3);
		matrix3[size3][0] = nextRow;
		matrix3[size3][1] = nextCol;
		matrix3[size3][2] = val;
		size3++;
	}
	f3->matrix = matrix3;
	f3->size = size3;
}

void transposeMatrixF(struct FileInfo* f1){
	int temp = 0;
	int temp2 = 0;
	int temp3 = 0;
	int j=0;
	int** matrix = f1->matrix;

	for(int i=0;i<f1->size;i++){
		temp = matrix[i][0];
		matrix[i][0] = matrix[i][1];
		matrix[i][1] = temp;
	}
	
	for(int i=0;i<f1->size-1;i++){
		if(matrix[i+1][0] < matrix[i][0]){
			j=i;
			while(j>=0 && matrix[j+1][0] < matrix[j][0]){
				
				temp = matrix[j+1][0];
				matrix[j+1][0] = matrix[j][0];
				matrix[j][0] = temp;

				temp2 = matrix[j+1][1];
				matrix[j+1][1] = matrix[j][1];
				matrix[j][1] = temp2;

				temp3 = matrix[j+1][2];
				matrix[j+1][2] = matrix[j][2];
				matrix[j][2] = temp3;
				
				j--;					
			}
		}
	}

	temp = f1->rows;
	f1->rows = f1->cols;
	f1->cols = temp;
}
 
//I think this works as a very inefficient bare bones.
// can be improved that it doesnt record the start and end point for the
// row value, doesn't record where the values are in the second matrix 
// if it has to traverse a second time, but need to get cracking on 
//parallel shite, so get this working, get everything working to spec
// do parallel and then tweak functions.
void coordMatrixMultiplyF(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3){
	if(f1->cols != f1->rows){
		printf("Colums of Matrix 1 and rows of Matrix 2 must be equal\n");
		exit(0);
	}
	
	int c1=0, c2=0, c3=0, val=0, totVal=0;
	int size3 = f1->size > f2->size ? f1->size:f2->size;
	size3 *= 2;
	int** matrix1 = f1->matrix;
	int** matrix2 = f2->matrix;
	int** matrix3 = malloc(sizeof(int*)*size3);
	for(int i=0; i<f1->rows; i++){
		for(int j=0; j<f2->cols; j++){
			c1=0;
			totVal=0;
			while(c1 < f1->size && matrix1[c1][0] <= i){
				c2 = 0;
				while(c2<f2->size){
					if(matrix1[c1][0] == i && matrix2[c2][1] == j && matrix1[c1][1] == matrix2[c2][0]){
						val = matrix1[c1][2] * matrix2[c2][2];
						c2=f2->size;
						totVal += val;
					}
					c2++;
				}
				c1++;
			}
			
			if(totVal > 0){
				if(c3 > size3){
					printf("Soz, not enoguh room!!\n");
					exit(0);
				}
				
				matrix3[c3] = (int*)malloc(sizeof(int)*3);
				matrix3[c3][0] = i;
				matrix3[c3][1] = j;
				matrix3[c3][2] = totVal;
				c3++;
			}
		}
	}
	f3->size = c3;
	f3-> matrix = matrix3;
	f3->rows = f1->rows;
	f3->cols = f2->cols;
}





