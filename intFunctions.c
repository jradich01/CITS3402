#include<stdio.h>
#include<stdlib.h>
#include"matrixStructures.h"
#include"intFunctions.h"


int** makeCoordMatrix(struct FileInfo* fInfo){
	int arrSize = fInfo->size;
	int** coordMatrix = malloc(sizeof(int*)*arrSize);
	int row = 0;
	int col = 0;
	int val = 0;
	int totCount = 1;
	
	for(int i=0;i<arrSize;i++){
		
		if(feof(fInfo->file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(fInfo->file,"%d\n",&val);
		if(val ==0){
			i--;
		}
		else{
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
			coordMatrix[i][2] = val;
		}
		totCount++;
	}

	return coordMatrix;
}

void displayCoordMatrix(int** matrix, int size){
	for(int i=0;i<size;i++){
		printf("Row: %d  Col: %d Value: %d\n",matrix[i][0],matrix[i][1],matrix[i][2]);
	}
}

//might change to while loop and stop i-- for each iteration
int** makeCSRMatrix(struct FileInfo* fInfo){
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
	
	return CSRMatrix;
}

void displayCSRMatrix(int** matrix,struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	
	FILE* f = fopen("CSRM.out","w");
	for(int i=0;i<rows;i++){
		fprintf(f,"Row Totals: %d\n",matrix[1][i+1]-matrix[1][i]);
	}
		
	for(int i=0;i<size;i++){
		fprintf(f,"Col: %d\n Value: %d\n",matrix[2][i],matrix[0][i]);
	}
	fclose(f);
}

void intScalarMultiply(int** matrix,int size, int scalar){
	for(int i=0; i<size; i++){
		matrix[i][2] *= scalar;
	}
}

void printDenseCoordMatrix(int** matrix, int size, int rows, int cols){
	int matrixCounter =0;
	int val = 0;
	FILE* file = fopen("DenseCoordMatrix.out","w");
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			val = 0;
			if(matrixCounter<size){
				if(matrix[matrixCounter][0]==i && matrix[matrixCounter][1]==j){
					val = matrix[matrixCounter][2]; 
					matrixCounter++;
				}
			}
			fprintf(file,"%d ",val);
		}
		fprintf(file,"\n");
	}
	fclose(file);
}

int intTraceCoordCalc(int** matrix,struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int tot = 0;
	int m = 0;
	if(rows != cols){
		printf("Trace can only be performed on square matricies\n");
		exit(0);
	}
	else{
		FILE* f = fopen("coord.out","w");
		for(int i=0;i<rows;i++){
			while(m < size && (matrix[m][0] <= i || matrix[m][1] <= i) ){
				if(matrix[m][0] == i && matrix[m][1] == i){
					fprintf(f,"R:%d C:%d V:%d\n",matrix[m][0],matrix[m][1],matrix[m][2]);
					tot+= matrix[m][2];
				}
				m++;
			}
		}
		fclose(f);
	}
	return tot;
}

int intTraceCSRCalc(int** matrix, struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;
	int val = 0;
	int j = 0;
	int count = 0;
	int tot = 0;
	if(rows != cols){
		printf("Trace can only be performed on square matricies\n");
		exit(0);
	}
	else{
		FILE* f = fopen("CSR.out","w");
		for(int i=0;i<rows;i++){
			val = matrix[1][i+1] - matrix[1][i];
			if(val > 0){
				count = 0;
				while(count < val && matrix[2][j + count] <= i ){
					if(matrix[2][j + count] == i){
						fprintf(f,"R:%d Val:%d\n",matrix[2][j + count],matrix[0][j+count]);
						tot+= matrix[0][j + count];
						count = val; 
					}
					count++;
				}
			}
			j+=val;
		}
		fclose(f);
	}
	return tot;
}

void printDenseCSRMatrix(int** matrix, struct FileInfo* fInfo){
	int size = fInfo->size;
	int rows = fInfo->rows;
	int cols = fInfo->cols;	
	int numInRow = 0;
	int totCount = 0;
	int count =0;
	int val = 0;
	
	FILE* f = fopen("printCSR.out","w");
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
	
	fclose(f);
}

int** coordMatrixAddition(int** matrix1, int size1, int** matrix2, int size2, int* matrix3Size){
	int** matrix3 = malloc(sizeof(int*)*(size1+size2));  //might be oversize but will fit worst case scenario
	int val = 0;
	int size3 = 0;
	int s1Count=0;
	int s2Count=0;
	int nextRow = 0;
	int nextCol = 0;
	
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
	*matrix3Size = size3;
	return matrix3;
}

void transposeMatrix(int** matrix, int size){
	int temp = 0;
	int temp2 = 0;
	int temp3 = 0;
	int j=0;

	for(int i=0;i<size;i++){
		temp = matrix[i][0];
		matrix[i][0] = matrix[i][1];
		matrix[i][1] = temp;
	}
	
	for(int i=0;i<size-1;i++){
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
}

/*
void coordMatrixMultiply(int** matrix1, int size1,int rows1, int** matrix2, int size2){
	
	int rowVal = 0;
	int colVal = 0;
	int i=0;
	int k=0;
	int min = 0;
	int max = 0;
	int* index = malloc(sizeof(int)*rows1);
	int indexSize=0;
	int firstIt = 0;
	int val = 0;
	int found = 0;
	int finalVal = 0;
	int matrix2Col =0;
	//highly unlikely matrix will created a bigger one, so just use biggest size.
	int bigger = size1 > size2? size1 : size2;
	int** newMatrix = malloc(sizeof(int*)*bigger;
	
	
	//need to increment rowVal, also newMatrixCol, need to somehow create an array for new values.
	
	while(i<size1 && matrix1[i][0] == rowVal){
		colVal = matrix1[i][1];
		val = matrix[i][2];
		found = 0;
		if(firstIt ==0){
			for(int j=0;j<size2;j++){
				if(matrix[j][1] == rowVal){
					index[indexSize] = j;
					indexSize++;
					if(matrix2[j][0] == colVal){
						val*= matrix2[j][2];
						found = 1;
					}
				}
			}
			if(found ==0){
				val = 0;
			}
			firstIt = 1;
		}
		else{
			k = 0;
			while(k<indexSize && matrix2[index[k]][0] <= colVal && found ==0){
				if(matrix2[index[k]][0] == colVal){
					val *= matrix2[index[k]][2];
					found = 1;
				}
				k++;
			}
			if(found ==0){
				val = 0;
			}
		}
		finalVal += val;		
		i++;
	}
	
	
} */
 
//I think this works as a very inefficient bare bones.
// can be improved that it doesnt record the start and end point for the
// row value, doesn't record where the values are in the second matrix 
// if it has to traverse a second time, but need to get cracking on 
//parallel shite, so get this working, get everything working to spec
// do parallel and then tweak functions.
void coordMatrixMultiply(int** matrix1, int size1, int rows1, int** matrix2, int size2, int cols2){
	
	int c1=0, c2=0, c3=0, val=0, totVal=0;
	int size3 = size1 > size2 ? size1:size2;
	int** matrix3 = malloc(sizeof(int*)*size3);
	for(int i=0; i<rows1; i++){
		for(int j=0; j<cols2; j++){
			c1=0;
			totVal=0;
			while(matrix1[c1][0] == i){
				while(c2<size2){
					c2=0;
					if(matrix1[c1][0] == matrix2[c2][1] && matrix1[c1][1] == matrix2[c2][0]){
						val = matrix1[c1][2] * matrix2[c2][2];
						c2=size2;
					}
				}
				totVal += val;
				c1++;
			}
			if(totVal > 0){
				matrix3[c3] = (int*)malloc(sizeof(int)*3);
				matrix3[c3][0] = i;
				matrix3[c3][1] = j;
				matrix3[c3][2] = totVal;
				c3++;
			}
		}
	}
}





