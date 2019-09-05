#include<stdio.h>
#include<stdlib.h>
#include"intFunctions.h"

int* makeIntMatrix(int size, FILE* file){
	int* arr = malloc(sizeof(int)*size);
	for(int i=0;i<size;i++){
		if(feof(file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(file,"%d",&arr[i]);
		if(arr[i]==0)i--;
	}
	return arr;
}

void displayIntMatrix(int* matrix, int numCount){
	for(int i=0;i<numCount;i++){
		printf("%d\n",matrix[i]);
	}	
}

int** makeCoordMatrix(int size, FILE* file,int cols){
	int** coordMatrix = malloc(sizeof(int*)*size);
	int row = 0;
	int col = 0;
	int val = 0;
	int totCount = 1;
	
	for(int i=0;i<size;i++){
		
		if(feof(file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(file,"%d\n",&val);
		if(val ==0){
			i--;
		}
		else{
			coordMatrix[i] = (int*)malloc(sizeof(int)*3);
			row = totCount / cols;
			col = totCount % cols;
			if(col ==0){
				col = cols;
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
int** makeCSRMatrix(int size,FILE* file, int rows, int cols){
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
		if(feof(file)){
			printf("Not enough numbers provided\n");
			exit(0);
		}
		fscanf(file,"%d\n",&val);
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

void displayCSRMatrix(int** matrix,int size, int rows){
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

void printDenseCoordMatrix(FILE* file, int** matrix, int size,int rows, int cols){
	int matrixCounter =0;
	int val = 0;
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
}

int intTraceCoordCalc(int** matrix,int size,int rows, int cols){
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

int intTraceCSRCalc(int** matrix, int size, int rows, int cols){
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
//not finished, need to add totalCount as well
void printDenseCSRMatrix(int** matrix, int size, int rows, int cols){
	FILE* f = fopen("printCSR.out","w");
	int numInRow = 0;
	int totCount = 0;
	int count =0;
	int val = 0;
	
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





