#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "matrixStructures.h"
#include "intFunctions.h"
#include "doubleFunctions.h"
#include "helperFunctions.h"

int main(int argc, char** argv){
	
	int command;
	int** intMatrix;
	//float* dmatrix; //need to update evenually
	struct FileInfo f1;
	struct FileInfo f2;
	struct ReportData r1;
	clock_t begin = clock();
	
	printf("v2.001\n");
	command = processCommands(argc,argv,&r1);
	initialiseFileInfo(&f1,r1.fileName1,4);
	if(r1.fileName2 != NULL){
		printf("And herea\n");
		initialiseFileInfo(&f2,r1.fileName2,4);
	}
	printf("And hereb %d\n",command);
	int** intMatrix2;
	int** intMatrix3;
	int matrix3Size = 0;
	switch(command){
		case 1: //scalar
			intMatrix = makeCoordMatrix(&f1);
			intScalarMultiply(intMatrix,f1.size,4);
			break;
		case 2: //trace
			intMatrix = makeCoordMatrix(&f1);
			r1.trace = intTraceCoordCalc(intMatrix,&f1);
			//intMatrix = makeCSRMatrix(&f1);
			//r1.trace = intTraceCSRCalc(intMatrix,&f1);
			break;
		case 3: //transpose
			intMatrix = makeCoordMatrix(&f1);
			
			transposeMatrix(intMatrix,f1.size);
			int temp = f1.rows;
			f1.rows = f1.cols;
			f1.cols = temp; 
			//displayCoordMatrix(intMatrix,f1.size);
			printDenseCoordMatrix(intMatrix,f1.size,f1.rows,f1.cols);
			break;
		case 4: // addition
			if(f1.rows != f2.rows || f1.cols != f2.cols){
				printf("Matrixes must be the same size for addition\n");
				exit(0);
			}
			intMatrix = makeCoordMatrix(&f1); 
			intMatrix2 = makeCoordMatrix(&f2); 
			intMatrix3 = coordMatrixAddition(intMatrix,f1.size,intMatrix2,f2.size,&matrix3Size); 
			printDenseCoordMatrix(intMatrix3,matrix3Size,f1.rows,f1.cols);
			//printf("%d %d",f1.size,f2.size);
			//int intMatrix3Size = coordMatrixAddition(intMatrix,f1.size,intMatrix2,f2.size,intMatrix3);
			//printf("aaa %d\n",matrix3Size);
			//displayCoordMatrix(intMatrix3,matrix3Size);
			break;
		case 5: // multiply //r1xc1 r2xc2  c1 == r2.  creates a r1xc2 matrix
			intMatrix = makeCoordMatrix(&f1);
			intMatrix2 = makeCoordMatrix(&f2);
			intMatrix3 = coordMatrixMultiply(intMatrix,f1.size,f1.rows,f1.cols,intMatrix2,f2.size,f2.rows,f2.cols,&matrix3Size);
			printDenseCoordMatrix(intMatrix3,matrix3Size,f1.rows,f2.cols);
			break;
		default:
			printf("Unknown error\n");
			exit(0);
	}
	
	fclose(f1.file);
	clock_t end =clock();
	r1.fileProcTimeTaken = (double)(end-begin)/CLOCKS_PER_SEC;
	printOutputFile(&f1,&f2,&r1);

	return 0;
}










