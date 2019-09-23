//coordFunctions.h 
//cits3402  
//assignment1
//author: jradich

void makeCoordMatrix(struct FileInfo* fInfo);
void scalarMultiply(struct FileInfo* f1, float scalar,int threads);
void printDenseCoordMatrix(struct FileInfo* f1, FILE* file);
float traceCoordCalc(struct FileInfo* fInfo,int threads);
void coordMatrixAddition(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3);
void coordMatrixMultiply(struct FileInfo* f1, struct FileInfo* f2, struct FileInfo* f3, int threads);
void transposeMatrixMP(struct FileInfo* f1,int threads);
