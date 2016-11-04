#ifndef __SERVER__
#define __SERVER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>


#include <signal.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <map>
#include <event.h>
#include <event2/bufferevent.h>  

typedef struct fdBuf_s fdBuf_t;
struct fdBuf_s
{
	int fd;
	char *buf;
	char *bufPoint;
	int bufLen;
	int maxLen;
};




int socketinit(int port);

int dealBuf(fdBuf_t * fdBuf);
int dealBufLogin(const char *buf,const int bufLen);
int dealBufReady(const char *buf,const int bufLen);
int dealBufMsg(const char *buf,const int bufLen);
int dealBufLeave(const char *buf,const int bufLen);


int addMap(int fd);
int removeMap(int fd);
fdBuf_t* findMap(int fd);


void acceptCb(int fd, short events, void* arg);
void readCb(bufferevent*bev ,void* arg);
void eventCb(bufferevent*bev  ,short event,void* arg);

void addCalculate();
void* calculate(void *param);

#endif