
int* makeIntMatrix(int size, FILE* file);
void displayIntMatrix(int* matrix, int numCount);
int** makeCoordMatrix(int size, FILE* file,int cols);
void displayCoordMatrix(int** matrix, int size);
int** makeCSRMatrix(int size,FILE* file, int rows, int cols);
void displayCSRMatrix(int** matrix,int size, int rows);
void intScalarMultiply(int** matrix,int size, int scalar);
void printDenseCoordMatrix(FILE* file, int** matrix, int size,int rows, int cols);