#ifndef HMEMORY_POOL_H_
#define HMEMORY_POOL_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct blockHead blockHead;
struct blockHead
{
	int size;
	void *next;
};


typedef struct slot slot;
struct slot
{
	void *free;
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
void* getHMemory(int size,int status);
int freeHMemory(void *p);
int setHMemoryNums(int size,int maxNums);
int setHMemoryMaxSize(int maxSize);
int printHMemory(int fd);


#endif