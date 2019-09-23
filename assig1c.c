//assig1c.c
//cits3402  
//assignment1
//author: jradich


#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "matrixStructures.h"
#include "coordFunctions.h"
#include "helperFunctions.h"

int main(int argc, char** argv){
	
	struct FileInfo f1,f2,f3;
	struct ReportData r1;
	double fileProcBegin ,fileProcEnd, calcProcBegin, calcProcEnd;
	
	processCommands(argc,argv,&r1);  //process commands entered by user
	printf("Working..\n");
	fileProcBegin = omp_get_wtime();  //begin recording file processing time 
	
	#pragma omp parallel
	{
		#pragma omp single nowait //use parellel block but only spawn tasks from a single thread
		{
			#pragma omp task    //task 1 manages the first file initialisation
			{
				initialiseFileInfo(&f1,r1.fileName1);  //first initialise the file and then create the matrix
				makeCoordMatrix(&f1);
			}
			
			#pragma omp task   //if a second file is required this is handled by a second thread so 
			{					//both files can be loaded in parallel. 
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
	#pragma omp taskwait  //wait for both files to finish loading before continuing

	fileProcEnd = omp_get_wtime();  //stop recording file processing time and start recording 
	calcProcBegin = omp_get_wtime();// specified calculation time. 
	
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
	
	calcProcEnd = omp_get_wtime();//stop recording calculation time 
	
	r1.fileProcTimeTaken = (fileProcEnd-fileProcBegin);  //calculate times 
	r1.calcProcTimeTaken = (calcProcEnd-calcProcBegin);
	printOutputFile(&f1,&f2, &f3,&r1);  //print output to file 
	memoryCleanup(&f1,&f2,&f3);  // clean up allocated memory 
	printf("Complete.\n");

	return 0;
}











