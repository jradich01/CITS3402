
void makeCoordMatrix(struct FileInfo* fInfo);
void makeCSRMatrix(struct FileInfo* fInfo);
void scalarMultiply(struct FileInfo* f1, float scalar);
void printDenseCoordMatrix(struct FileInfo* f1, FILE* file);
float traceCoordCalc(struct FileInfo* fInfo);
float traceCSRCalc(struct FileInfo* fInfo);
void printDenseCSRMatrix(struct FileInfo* fInfo, FILE* f);
void coordMatrixAddition(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void transposeMatrix(struct FileInfo* fInfo);
void coordMatrixMultiply(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
