#include "hMemoryPool.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	initStruct preSizeArr[] = {{48,5000},{60,4000},{88,3000},{120,2000}};
	int preNums = sizeof(preSizeArr) / sizeof(initStruct);
	
	char buf[100];

	initHMemoryPool(preSizeArr,preNums);
	printHMemory(STDOUT_FILENO);


	read(STDIN_FILENO,buf,sizeof(buf));
	return 0;
}