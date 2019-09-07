
int** makeCoordMatrix(struct FileInfo* fInfo);
void displayCoordMatrix(int** matrix, int size);
int** makeCSRMatrix(struct FileInfo* fInfo);
void displayCSRMatrix(int** matrix,struct FileInfo* fInfo);
void intScalarMultiply(int** matrix,int size, int scalar);
void printDenseCoordMatrix(int** matrix, int size, int rows, int cols);
int intTraceCoordCalc(int** matrix,struct FileInfo* fInfo);
int intTraceCSRCalc(int** matrix, struct FileInfo* fInfo);
void printDenseCSRMatrix(int** matrix, struct FileInfo* fInfo);
int** coordMatrixAddition(int** matrix1, int size1, int** matrix2, int size2, int* matrix3Size);
void transposeMatrix(int** matrix, int size);
void coordMatrixMultiply(int** matrix1, int size1, int rows1, int** matrix2, int size2, int cols2);

