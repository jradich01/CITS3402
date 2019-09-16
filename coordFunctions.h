
void makeCoordMatrix(struct FileInfo* fInfo);
void scalarMultiply(struct FileInfo* f1, float scalar);
void printDenseCoordMatrix(struct FileInfo* f1, FILE* file);
float traceCoordCalc(struct FileInfo* fInfo);
void coordMatrixAddition(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void transposeMatrix(struct FileInfo* fInfo);
void coordMatrixMultiply(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void transposeMatrixMP(struct FileInfo* f1);
