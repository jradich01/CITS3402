struct FileInfo{
	FILE* file;
	//char* fileName;
	int rows,cols,size,threads;
	char dataType[6];
};

int** makeCoordMatrix(struct FileInfo* fInfo);
void displayCoordMatrix(int** matrix, int size);
int** makeCSRMatrix(struct FileInfo* fInfo);
void displayCSRMatrix(int** matrix,struct FileInfo* fInfo);
void intScalarMultiply(int** matrix,int size, int scalar);
void printDenseCoordMatrix(FILE* file, int** matrix, struct FileInfo* fInfo);
int intTraceCoordCalc(int** matrix,struct FileInfo* fInfo);
int intTraceCSRCalc(int** matrix, struct FileInfo* fInfo);
void printDenseCSRMatrix(int** matrix, struct FileInfo* fInfo);

