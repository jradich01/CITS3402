
void displayCoordMatrixF(struct FileInfo* f1);
void makeCSRMatrixF(struct FileInfo* fInfo);
int traceCSRCalcF(struct FileInfo* fInfo);
void printDenseCSRMatrixF(struct FileInfo* fInfo, FILE* f);
void makeCoordMatrixF(struct FileInfo* fInfo);
void scalarMultiplyF(struct FileInfo* f1, float scalar);
int traceCoordCalcF(struct FileInfo* fInfo);
void transposeMatrixF(struct FileInfo* fInfo);
void coordMatrixAdditionF(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void coordMatrixMultiplyF(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void printDenseCoordMatrixF(struct FileInfo* f1, FILE* file);