#include "hMemoryPool.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	initStruct preSizeArr[] = {{40,20000},{60,4000},{88,3000},{120,2000}};
	int preNums = sizeof(preSizeArr) / sizeof(initStruct);
	int i,status;
	char buf[100];
	void *temp;

	initHMemoryPool(preSizeArr,preNums);

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);

	void *tempArr[100000];
	int tempNums=0;
	for ( i = 0; i < 26000; ++i)
	{
		tempArr[i] = getHMemory(19,&status);
		tempNums = i + 1;
	}

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);

	for ( i = 0; i < tempNums; ++i)
	{
		freeHMemory(tempArr[i]);
	}

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);


	read(STDIN_FILENO,buf,sizeof(buf));
	return 0;
}