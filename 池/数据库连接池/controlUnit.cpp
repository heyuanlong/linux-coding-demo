#include "controlUnit.h"

controlUnit::controlUnit()
{}

controlUnit::~controlUnit()
{}

void controlUnit::push_back(int fd)
{
	mutex.lock();
	fdDeque.push_back(fd);
	mutex.unlock();
}

int controlUnit::front()
{
	int temp = -1;
	mutex.lock();
	if (!fdDeque.empty()){
		temp = fdDeque.front();
	}
	mutex.unlock();
	return temp;
}

void controlUnit::pop_front()
{
	mutex.lock();
	if (!fdDeque.empty()){
		fdDeque.pop_front();
	}
	mutex.unlock();
}

bool controlUnit::empty()
{
	bool temp;
	mutex.lock();
	temp = fdDeque.empty();
	mutex.unlock();
	return temp;
}

pthread_mutex_t * controlUnit::getMutex()
{
	return mutex.getmutex();
}

pthread_cond_t * controlUnit::getCond()
{
	return &haveProduct;
}

void controlUnit::setNext(controlUnit* n)
{
	next = n;
}

controlUnit * controlUnit::getNext()
{
	return next;
}