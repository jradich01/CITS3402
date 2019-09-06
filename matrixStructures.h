
struct FileInfo{
	FILE* file;
	int rows,cols,size,threads;
	char dataType[6];
};

struct ReportData{
	char* fileName1;
	char* fileName2;
	char* cmd;
	int log;
	double fileProcTimeTaken;
	int scalarVal;
	int trace;
};