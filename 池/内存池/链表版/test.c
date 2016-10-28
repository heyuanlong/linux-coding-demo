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
	printHMemory(STDOUT_FILENO);
	for ( i = 0; i < 20; ++i)
	{
		temp = getHMemory(19,&status);
		freeHMemory(temp);
	}
	for ( i = 0; i < 20; ++i)
	{
		temp = getHMemory(34,&status);
		freeHMemory(temp);
	}
	for ( i = 0; i < 20; ++i)
	{
		temp = getHMemory(50,&status);
		freeHMemory(temp);
	}
	for ( i = 0; i < 20; ++i)
	{
		temp = getHMemory(88,&status);
		freeHMemory(temp);
	}
	for ( i = 0; i < 20; ++i)
	{
		temp = getHMemory(121,&status);
		freeHMemory(temp);
	}
	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);


	read(STDIN_FILENO,buf,sizeof(buf));
	return 0;
}