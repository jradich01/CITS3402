#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "intFunctions.h"
#include "doubleFunctions.h"

int getArraySize(FILE* file);

int main(int argc, char** argv){
	
	FILE* file;
	int numCount=0;
	int rows,cols,loc;
	char dataType[6];
	int* matrix;
	int** coordMatrix;
	float* dmatrix;
	int threads = 4;
	int trace = 0;
	
	char* cmd = "tr";
	char* file1 = "matrix3.in";
	char* file2 = NULL;
	
	clock_t begin = clock();
	
	printf("commands sent: %d\n",argc);
	
	file = fopen(file1,"r");	
	if(file==NULL){
		printf("Cannot open file\n");
		exit(0);
	}
	
	fscanf(file,"%s %d %d",dataType,&rows,&cols);	
	loc = ftell(file);
	numCount = getArraySize(file);
	fseek(file,loc,SEEK_SET);
	
	printf("Size %d\n",numCount);
	printf("V1.993\n\n");
	printf("%s\n",dataType);
	
	//traces are return different values! 
	if(strcmp(dataType,"int")==0){
		//matrix = makeIntMatrix(numCount,file);
		coordMatrix = makeCoordMatrix(numCount,file,cols);
		//displayCoordMatrix(coordMatrix,numCount);
		//coordMatrix = makeCSRMatrix(numCount,file,rows,cols);
		//displayCSRMatrix(coordMatrix,numCount,rows);
		//trace = intTraceCSRCalc(coordMatrix,numCount,rows,cols);
		trace = intTraceCoordCalc(coordMatrix,numCount,rows,cols);
		//intScalarMultiply(coordMatrix,numCount,4);
		
		//coordMatrix = makeCSRMatrix(numCount,file,rows,cols);
		//displayCSRMatrix(coordMatrix,numCount,rows);
		
	}
	else{
		dmatrix = makeDoubleMatrix(numCount,file);
	}
	
	fclose(file);
	clock_t end =clock();
	double fileProcTimeTaken = (double)(end-begin)/CLOCKS_PER_SEC;
	
	
	printf("Total time taken: %f\n",fileProcTimeTaken);
	
	//output log file
	FILE* outFile = fopen("output.out","w");
	if(outFile == NULL){
		printf("Could not create output file");
		exit(1);
	}
	fprintf(outFile,"%s\n%s\n",cmd,file1);
	if(file2 != NULL){
		fprintf(outFile,"%s\n",file2);
	}
	fprintf(outFile,"%d\n%s\n%d\n%d\n\n", threads,dataType,rows,cols);
	fprintf(outFile,"File processing time: %f\n",fileProcTimeTaken);
	fprintf(outFile,"Trace: %d\n",trace);
	//printDenseCoordMatrix(outFile,coordMatrix,numCount,rows,cols);
	fclose(outFile);
	
	return 0;
}

int getArraySize(FILE* file){
	char val[5];
	int count =0;
	while(!feof(file)){
		fscanf(file,"%s",val);
		if(strcmp(val,"0") !=0 && strcmp(val,"0.0")!=0){
			count++;
		}
	}
	return count;	
}