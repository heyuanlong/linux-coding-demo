#ifndef HMEMORY_POOL_H_
#define HMEMORY_POOL_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct blockHead blockHead;
struct blockHead
{
	int size;
	char inPool;
	void *next;
};


typedef struct slot slot;
struct slot
{
	void *pFree;
	int blockSize;
	int freeNums;
	int allNums;
	int maxNums;
};

typedef struct initStruct initStruct;
struct initStruct
{
	int size;
	int nums;
};


int initHMemoryPool(initStruct* arr,int nums);
void* getHMemory(int size,int *status);
int freeHMemory(void *p);
int setHMemoryNums(int size,int maxNums);
int printHMemory(int fd);


#endif