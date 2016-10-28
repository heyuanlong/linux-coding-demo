#include "hMemoryPool.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char const *argv[])
{
	initStruct preSizeArr[] = {{40,20000},{60,4000},{88,3000},{120,2000}};
	int preNums = sizeof(preSizeArr) / sizeof(initStruct);
	int i,j,status;
	char buf[100];
	void *temp;
	clock_t start, finish; 
	int dur;
	time_t timep;



	initHMemoryPool(preSizeArr,preNums);

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);

	void *tempArr[1000000];
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
	
	setHMemoryNums(120,10000);
	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);
	
	setHMemoryNums(100,5000);
	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);
	
	for ( i = 0; i < 100; ++i)
	{
		setHMemoryNums(i,100);
	}

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);


	setHMemoryNums(40,10000);

	time(&timep);
	printf("time(): %d\n",timep);
	for ( j = 0; j < 10; ++j)
	{
		for ( i = 0; i < 10000; ++i)
		{
			tempArr[i] = getHMemory(40,&status);
			tempNums = i + 1;
		}
		for ( i = 0; i < tempNums; ++i)
		{
			freeHMemory(tempArr[i]);
		}
	}

	time(&timep);
	printf("time(): %d\n",timep);

	for ( j = 0; j < 10; ++j)
		{
		for ( i = 0; i < 10000; ++i)
		{
			tempArr[i] = malloc(40);
			tempNums = i + 1;
		}
		for ( i = 0; i < tempNums; ++i)
		{
			freeHMemory(tempArr[i]);
		}
	}
	time(&timep);
	printf("time(): %d\n",timep);

	write(STDOUT_FILENO,"\n",strlen("\n"));
	printHMemory(STDOUT_FILENO);


	read(STDIN_FILENO,buf,sizeof(buf));
	return 0;
}