
struct FileInfo{
	FILE* file;
	int rows,cols,size;
	char dataType[6];
	int** matrix;
	float** matrixF;
};

struct ReportData{
	char* fileName1;
	char* fileName2;
	int command;
	char* cmd;
	int log;
	double fileProcTimeTaken;
	double calcProcTimeTaken;
	int scalarVal;
	float scalarValF;
	int trace;
	float traceF;
	int threads;
};