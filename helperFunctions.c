#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "matrixStructures.h"
#include "helperFunctions.h"
#include "coordFunctions.h"

int arraySearch(char* cmd, char** arr, int size){
	int cell = -1;
	for(int i=0;i<size;i++){
		if(strcmp(cmd,arr[i])==0){
			cell = i;
			i = size;
		}
	}
	return cell;
}

void processCommands(int argc, char** argv, struct ReportData* r1){
	
	int cell = -1;
	int command = 0;
	int filesReq = 0;
	if(argc<4){
		printf("Not enough parameters 1\n");
		exit(0);
	}
	if((cell = arraySearch("--sc",argv,argc)) >-1){
		float scalarVal = 0;
		if(argc < (cell + 2)){
			printf("Missing scalar multiplier\n");
			exit(0);
		}
		scalarVal = atof(argv[cell+1]);
		if(scalarVal == 0){
			if(strcmp("0",argv[cell+1])!=0){
				printf("Could not convert scalar param to integer \n");
				exit(0);
			}
		}
		command = 1;
		r1->scalarVal = scalarVal;
		r1->cmd = "sc";
	}
	else if((cell = arraySearch("--tr",argv,argc)) >-1){
		command = 2;
		filesReq = 1;
		r1->cmd = "tr";
	}
	else if((cell = arraySearch("--ts",argv,argc))>-1){
		command = 3;
		filesReq = 1;
		r1->cmd = "ts";
	}
	else if((cell = arraySearch("--ad",argv,argc))>-1){
		command = 4;
		filesReq = 2;
		r1->cmd = "ad";
	}
	else if((cell = arraySearch("--mm",argv,argc))>-1){
		command = 5;
		filesReq = 2;
		r1->cmd = "mm";
	}
	else{
		printf("No command paramater found\n");
		exit(0);
	}
	
	if((cell = arraySearch("-f",argv,argc))<0){
		printf("File command required\n");
		exit(0);
	}
	
	if(argc < (cell + filesReq + 1)){
		printf("Not enough parameters 2\n");
		exit(0);
	}
	r1->fileName1 = argv[cell+1];
	
	if(filesReq == 2){
		r1->fileName2 = argv[cell+2];
	}
	else{
		r1->fileName2 = NULL;
	}
	
	r1->log = 0;
	if((cell = arraySearch("-l",argv,argc))>-1){
		r1->log = 1;
	}

	r1->command = command;
}


void printOutputFile(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3, struct ReportData* r1){
	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char* studentID = "22744833";
	char fileName[128];
	char dateTime[32];
	char* delim1="0";
	char* delim2="0";
	char* delim3="0";
	char* delim4="0";
	
	if(tm.tm_mday >= 10){
		delim1="";
	}
	if(tm.tm_mon >= 10){
		delim2="";
	}
	if(tm.tm_hour >=10){
		delim3="";
	}
	if(tm.tm_min >=10){
		delim4="";
	}

	snprintf(dateTime,sizeof(dateTime),"_%s%d%s%d%d_%s%d%s%d_",delim1,tm.tm_mday,delim2,tm.tm_mon+1,tm.tm_year + 1900,delim3, tm.tm_hour,delim4, tm.tm_min);	
	strcpy(fileName,studentID);
	strcat(fileName,dateTime);
	strcat(fileName,r1->cmd);
	strcat(fileName,".out");
	
	r1->threads = 1;  //update when multi threading
	
	FILE* outFile = fopen(fileName,"w");
	if(outFile == NULL){
		printf("Could not create output file");
		exit(1);
	}
	fprintf(outFile,"%s\n%s\n",r1->cmd,r1->fileName1);
	if(r1->fileName2 != NULL){
		fprintf(outFile,"%s\n",r1->fileName2);
	}
	fprintf(outFile,"%d\n%s\n%d\n%d\n\n", r1->threads,f1->dataType,f1->rows,f1->cols);
	fprintf(outFile,"File processing time: %f\n",r1->fileProcTimeTaken);
	fprintf(outFile,"Calculation processing time: %f\n",r1->calcProcTimeTaken);
	
	if(r1->log ==1){
		if(r1->command == 1 || r1->command == 3){
			printDenseCoordMatrix(f1,outFile);
		}
		else if(r1->command == 4 || r1->command ==5){
			printDenseCoordMatrix(f3,outFile);
		}
		else{
			fprintf(outFile,"Trace: %f\n",r1->trace);
		}
	}

	fclose(outFile);
}	

int getArraySize(FILE* file){
	//char val[10];
	int count =0;
	float val =0;
	while(!feof(file)){
		fscanf(file,"%f\n",&val);
		//if(strcmp(val,"0") !=0 && strcmp(val,"0.0")!=0){
		if(val!=0){
			count++;
			//printf("Yes: %f\n",val);
		}
		//printf("No: %f\n",val);
	}
	//printf("Count: %d\n",count);
	return count;	
}

void initialiseFileInfo(struct FileInfo* fInfo, char* fileName){
	int loc = 0;
	
	fInfo->file = fopen(fileName,"r");
	if(fInfo->file == NULL){
		printf("Unable to open file: %s\n",fileName);
		exit(0);
	}
	fscanf(fInfo->file,"%s %d %d",fInfo->dataType,&fInfo->rows,&fInfo->cols);
	
	if(strcmp(fInfo->dataType,"int")==0){
		strcpy(fInfo->printToken,"%g");
	}
	else{
		strcpy(fInfo->printToken,"%f");
	}
	
	loc = ftell(fInfo->file);
	fInfo->size = getArraySize(fInfo->file);
	fseek(fInfo->file,loc,SEEK_SET);
	
}

void memoryCleanup(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3){
	fclose(f1->file);
	closeMatrix((void*)f1->matrix,f1->size);
	free(f1->valMatrix);
	
	if(f2->matrix != NULL){
		fclose(f2->file);
		closeMatrix((void*)f2->matrix,f2->size);
		free(f2->valMatrix);
	}
	if(f3->matrix != NULL){
		closeMatrix((void*)f3->matrix,f3->size);
		free(f3->valMatrix);
	}
	
}

void closeMatrix(void** matrix, int size){
	for(int i=0;i<size;i++){
		free(matrix[i]);
	}
	free(matrix);
}