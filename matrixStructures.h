
struct FileInfo{
	FILE* file;
	int rows,cols,size;
	char dataType[6];
	int** matrix;
	float* valMatrix;
	char printToken[3];
};

struct ReportData{
	char* fileName1;
	char* fileName2;
	int command;
	char* cmd;
	int log;
	double fileProcTimeTaken;
	double calcProcTimeTaken;
	float scalarVal;
	float trace;
	int threads;
};