
void displayCoordMatrix(struct FileInfo* f1);
void makeCSRMatrix(struct FileInfo* fInfo);
int traceCSRCalc(struct FileInfo* fInfo);
void printDenseCSRMatrix(struct FileInfo* fInfo, FILE* f);
void makeCoordMatrix(struct FileInfo* fInfo);
void scalarMultiply(struct FileInfo* f1, int scalar);
int traceCoordCalc(struct FileInfo* fInfo);
void transposeMatrix(struct FileInfo* fInfo);
void coordMatrixAddition(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void coordMatrixMultiply(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void printDenseCoordMatrix(struct FileInfo* f1, FILE* file);