#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "matrixStructures.h"
#include "coordFunctions.h"
#include "helperFunctions.h"

int main(int argc, char** argv){
	
	//TODO
	//do additional thread tests
	// write repoort
	//comment code
	struct FileInfo f1,f2,f3;
	struct ReportData r1;
	double fileProcBegin ,fileProcEnd, calcProcBegin, calcProcEnd;
	
	processCommands(argc,argv,&r1);
	printf("Working..\n");
	fileProcBegin = omp_get_wtime();
	
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task 
			{
				initialiseFileInfo(&f1,r1.fileName1);
				makeCoordMatrix(&f1);
			}
			
			#pragma omp task 
			{
				if(r1.fileName2 != NULL){
					initialiseFileInfo(&f2,r1.fileName2);
					makeCoordMatrix(&f2);
				}
				else{
					f2.matrix = NULL;
					f3.matrix = NULL;
				}
			}
			
		}
	}
	#pragma omp taskwait

	fileProcEnd = omp_get_wtime();
	calcProcBegin = omp_get_wtime();
	
	switch(r1.command){
		case 1: //scalar
			scalarMultiply(&f1,r1.scalarVal,r1.threads);
			break;
		case 2: //trace
			r1.trace = traceCoordCalc(&f1,r1.threads);
			break;
		case 3: //transpose
			transposeMatrixMP(&f1,r1.threads);
			break;
		case 4: // addition
			coordMatrixAddition(&f1,&f2,&f3);
			break;
		case 5: // multiply //r1xc1 r2xc2  c1 == r2.  creates a r1xc2 matrix
			coordMatrixMultiply(&f1,&f2,&f3, r1.threads);
			break;
		default:
			printf("Unknown error\n");
			exit(0);
	}
	
	calcProcEnd = omp_get_wtime();//clock();
	
	r1.fileProcTimeTaken = (fileProcEnd-fileProcBegin);
	r1.calcProcTimeTaken = (calcProcEnd-calcProcBegin);
	printOutputFile(&f1,&f2, &f3,&r1);
	memoryCleanup(&f1,&f2,&f3);
	printf("Complete.\n");

	return 0;
}











