
int processCommands(int argc, char** argv, struct ReportData* r1);
int arraySearch(char* cmd, char** arr, int size);
void printOutputFile(struct FileInfo* f1, struct FileInfo* f2, struct ReportData* r1);
int getArraySize(FILE* file);
void initialiseFileInfo(struct FileInfo* fInfo, char* fileName, int threads);