#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "intFunctions.h"
#include "doubleFunctions.h"

struct ReportData{
	char* fileName1;
	char* fileName2;
	char* cmd;
	int log;
	double fileProcTimeTaken;
	int scalarVal;
	int trace;
};
int processCommands(int argc, char** argv, struct ReportData* r1);
int arraySearch(char* cmd, char** arr, int size);
void printOutputFile(struct FileInfo* f1, struct FileInfo* f2, struct ReportData* r1);
int getArraySize(FILE* file);
void initialiseFileInfo(struct FileInfo* fInfo, char* fileName, int threads);

int main(int argc, char** argv){
	
	int command;
	int** intMatrix;
	//float* dmatrix; //need to update evenually
	struct FileInfo f1;
	struct FileInfo f2;
	struct ReportData r1;
	clock_t begin = clock();
	
	command = processCommands(argc,argv,&r1);
	initialiseFileInfo(&f1,r1.fileName1,4);
	if(r1.fileName2 != NULL){
		initialiseFileInfo(&f2,r1.fileName2,4);
	}
	
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
		case 4: // addition
		case 5: // multiply
			printf("Not Implemented yet\n");
			exit(0);
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

int processCommands(int argc, char** argv, struct ReportData* r1){
	
	int cell = -1;
	int command = 0;
	int filesReq = 0;
	if(argc<4){
		printf("Not enough parameters\n");
		exit(0);
	}
	if(cell = arraySearch("-sc",argv,argc) >-1){
		int scalarVal = 0;
		if(argc < (cell + 2)){
			printf("Missing scalar multiplier\n");
			exit(0);
		}
		scalarVal = atoi(argv[cell+1]);
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
	else if(cell = arraySearch("-tr",argv,argc) >-1){
		printf("%d\n",cell);
		command = 2;
		filesReq = 1;
		r1->cmd = "tr";
	}
	else if(cell = arraySearch("-ts",argv,argc)>-1){
		command = 3;
		filesReq = 1;
		r1->cmd = "ts";
	}
	else if(cell = arraySearch("-ad",argv,argc)>-1){
		command = 4;
		filesReq= 2;
		r1->cmd = "ad";
	}
	else if(cell = arraySearch("-mm",argv,argc)>-1){
		command = 5;
		filesReq=2;
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
	
	printf("%d %s\n",cell,argv[0]);
	if(argc < (cell + filesReq + 1)){
		printf("Not enough parameters\n");
		exit(0);
	}
	//cell = arraySearch("-f",argv,argc);
	r1->fileName1 = argv[cell+1];
	if(filesReq == 2){
		r1->fileName2 = argv[cell+2];
	}
	
	return command;
}

/*
int processCommands(int argc, char** argv, struct ReportData* r1){
	int filesReq=0;
	int command = 0;
	char* param1;
	
	if(argc<4){
		printf("Not enough parameters");
		exit(0);
	}
	param1 = argv[1];
	if(strcmp(param1,"-sc")==0){
		command = 1;
		filesReq = 1;
		r1->cmd = "sc";
	}
	if(strcmp(param1,"-tr")==0){
		command = 2;
		filesReq = 1;
		r1->cmd="tr";
	} 
	if(strcmp(param1,"-ts")==0){
		command=3;
		filesReq=1;
		r1->cmd="ts";
	}
	if(strcmp(param1,"-ad")==0){
		command = 4;
		filesReq = 2;
		r1->cmd="ad";
	}
	if(strcmp(param1,"-mm")==0){
		command=5;
		filesReq=2;
		r1->cmd="mm";
	}
	if(filesReq==0){
		printf("Invalid paramater\n");
		exit(0);
	}
	
	if(filesReq ==2){
		if(argc<5){
			printf("Not enough parameters\n");
			exit(0);
		}
	}
	if(strcmp(argv[2],"-f")!=0){
		printf("Invalid parameter\n");
		exit(0);
	}
	
	r1->fileName1 = argv[3];
	r1->fileName2 = NULL;
	if(filesReq ==2){
		r1->fileName2 = argv[4];
	}
	
	return command;
}*/


void printOutputFile(struct FileInfo* f1, struct FileInfo* f2, struct ReportData* r1){
	printf("Size %d\n",f1->size);
	printf("V1.995\n\n");
	printf("%s\n",f1->dataType);
	printf("Total time taken: %f\n",r1->fileProcTimeTaken);
	
	FILE* outFile = fopen("output.out","w");
	if(outFile == NULL){
		printf("Could not create output file");
		exit(1);
	}
	fprintf(outFile,"%s\n%s\n",r1->cmd,r1->fileName1);
	if(r1->fileName2 != NULL){
		fprintf(outFile,"%s\n",r1->fileName2);
	}
	fprintf(outFile,"%d\n%s\n%d\n%d\n\n", f1->threads,f1->dataType,f1->rows,f1->cols);
	fprintf(outFile,"File processing time: %f\n",r1->fileProcTimeTaken);
	fprintf(outFile,"Trace: %d\n",r1->trace);
	//printDenseCoordMatrix(outFile,coordMatrix,numCount,rows,cols);
	fclose(outFile);
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

void initialiseFileInfo(struct FileInfo* fInfo, char* fileName, int threads){
	int loc = 0;
	fInfo->file = fopen(fileName,"r");
	if(fInfo->file == NULL){
		printf("Unable to open file: %s\n",fileName);
		exit(0);
	}
	//fInfo->fileName = fileName;
	fInfo->threads = threads;
	fscanf(fInfo->file,"%s %d %d",fInfo->dataType,&fInfo->rows,&fInfo->cols);
	loc = ftell(fInfo->file);
	fInfo->size = getArraySize(fInfo->file);
	fseek(fInfo->file,loc,SEEK_SET);
	
}








