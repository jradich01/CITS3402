
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv){
	
	const int NUMS = 4000000;
	//float num = 0, a = 5.0;
	int num=0;
	FILE* file;
	srand(time(0));
	file = fopen("nums.txt","w");
	
	if(file==NULL){
		printf("Error opening file");
		exit(1);
	}
	
	for(int i=0;i<NUMS;i++){
		num = rand() % 10;
		if(num == 0){
			num = rand() % 1000 + 1;
			//num = (float)rand() / (float)(RAND_MAX/a) * 100.0;
		}
		else{
			//num = 0.0;
			num = 0;
		}
		//fprintf(file,"%.1f ",num);
		fprintf(file,"%d ",num);
	}
	
	fclose(file);
	
}