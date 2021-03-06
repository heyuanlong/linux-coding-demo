#ifndef __CLIENT_H_
#define __CLIENT_H_

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


#include <pthread.h>

int connectServer(char *ip,int port);

void * deal(void *param);

int getBuf(char *buf,int *bufLen,int *type);
int getBufLogin(char *buf,int *bufLen);
int getBufReady(char *buf,int *bufLen);
int getBufMsg(char *buf,int *bufLen);
int getBufLeave(char *buf,int *bufLen);

void msSleep(int millSec);



#endif