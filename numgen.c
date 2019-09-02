
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){
	
	const int NUMS = 1000000;
	int num = 0;
	FILE* file;
	file = fopen("nums.txt","w");
	
	if(file==NULL){
		printf("Error opening file");
		exit(1);
	}
	
	for(int i=0;i<NUMS;i++){
		num = rand() % 10;
		if(num == 0){
			num = rand() % 10 + 1;
		}
		else{
			num = 0;
		}
		fprintf(file,"%d ",num);
	}
	
	fclose(file);
	
}