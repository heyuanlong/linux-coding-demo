#ifndef H_TOOL__H_
#define H_TOOL__H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <unistd.h>
#include <errno.h>


void msSleep(int millSec);

long getFileSize(const char *path);
int checkIsDir(const char *path);
int checkIsFile(const char *path);


#endif H_TOOL__H_