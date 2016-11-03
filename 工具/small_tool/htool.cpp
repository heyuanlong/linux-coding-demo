#include "htool.h"



void msSleep(int millSec)
{
#if (defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_))
	Sleep(millSec);
#else
	usleep(1000 * millSec);
#endif
}


long getFileSize(const char *path)
{
	long len= 0;
	FILE* file = fopen(path,"rb");
	if(NULL == file){
		return -1;
	}
	fseek(file,0L,SEEK_END);
	len = ftell(file);
	fclose(file);
	return len;

}

int checkIsDir(const char *path)
{
	struct stat st;
	if(stat(path,&st) != 0){
		return -1;
	}
	if(st.st_mode & S_IFDIR){
		return 0;
	}
	return -1;
}
int checkIsFile(const char *path)
{
	struct stat st;
	if(stat(path,&st) != 0){
		return -1;
	}
	if(st.st_mode & S_IFREG){
		return 0;
	}
	return -1;
}