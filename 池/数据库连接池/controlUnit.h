#ifndef __SERVER__
#define __SERVER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <deque>
#include "mysql.h"
#include "mysqlPool.h"
#include "mutex.h"

class controlUnit
{
public:
	controlUnit();
	~controlUnit();
	void push_back(int fd);
	int front();
	void pop_front();
	bool empty();
	bool emptyNoLock();
	void lock();
	void unlock();
	pthread_mutex_t * getMutex();
	pthread_cond_t * getCond();
	void setNext(controlUnit* n);
	controlUnit * getNext();
private:
	std::deque<int> fdDeque;
	CommonMutex mutex;
	pthread_cond_t haveProduct;
	controlUnit* next;
	/* data */
};

#endif