
void displayCoordMatrix(int** matrix, int size);
int** makeCSRMatrix(struct FileInfo* fInfo);
void displayCSRMatrix(int** matrix,struct FileInfo* fInfo);
int intTraceCSRCalc(struct FileInfo* fInfo);
void printDenseCSRMatrix(int** matrix, struct FileInfo* fInfo);
void makeCoordMatrix(struct FileInfo* fInfo);
void intScalarMultiply(struct FileInfo* f1, int scalar);
int intTraceCoordCalc(struct FileInfo* fInfo);
void transposeMatrix(struct FileInfo* fInfo);
void coordMatrixAddition(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void coordMatrixMultiply(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void printDenseCoordMatrix(struct FileInfo* f1, FILE* file);