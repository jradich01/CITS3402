#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "matrixStructures.h"
#include "intFunctions.h"
#include "helperFunctions.h"

int main(int argc, char** argv){
	
	//TODO some cleanup resturctuing
	// commands change
	// write repoort
	// multi thread
	// tweak mm
	struct FileInfo f1;
	struct FileInfo f2;
	struct FileInfo f3;
	struct ReportData r1;
	clock_t fileProcBegin;
	clock_t fileProcEnd;
	clock_t calcProcBegin;
	clock_t calcProcEnd;
	
	printf("v2.001\n");
	processCommands(argc,argv,&r1);
	fileProcBegin = clock();
	
	initialiseFileInfo(&f1,r1.fileName1);
	makeCoordMatrix(&f1);
	if(r1.fileName2 != NULL){
		initialiseFileInfo(&f2,r1.fileName2);
		makeCoordMatrix(&f2);
	}
	else{
		f2.matrix = NULL;
		f3.matrix = NULL;
	}
	fileProcEnd = clock();
	calcProcBegin = clock();
	
	switch(r1.command){
		case 1: //scalar
			scalarMultiply(&f1,r1.scalarVal);
			break;
		case 2: //trace - csr also available
			r1.trace = traceCoordCalc(&f1);
			break;
		case 3: //transpose
			transposeMatrix(&f1);
			break;
		case 4: // addition
			coordMatrixAddition(&f1,&f2,&f3);
			break;
		case 5: // multiply //r1xc1 r2xc2  c1 == r2.  creates a r1xc2 matrix
			coordMatrixMultiply(&f1,&f2,&f3);
			break;
		default:
			printf("Unknown error\n");
			exit(0);
	}
	
	calcProcEnd = clock();
	
	r1.fileProcTimeTaken = (double)(fileProcEnd-fileProcBegin)/CLOCKS_PER_SEC;
	r1.calcProcTimeTaken = (double)(calcProcEnd-calcProcBegin)/CLOCKS_PER_SEC;
	printOutputFile(&f1,&f2, &f3,&r1);
	memoryCleanup(&f1,&f2,&f3);

	return 0;
}











