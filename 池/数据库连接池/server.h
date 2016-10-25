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

#include <sys/epoll.h>
#include <pthread.h>
#include <list>
#include <deque>
#include "mysql.h"
#include "mysqlPool.h"
#include "mutex.h"
#include "controlUnit.h"

int socketinit(int port);
int sendMsg(int originFd,char *msg,int msgLen);

void* deal(void *param);
void cond_sign(controlUnit *cont);
#endif