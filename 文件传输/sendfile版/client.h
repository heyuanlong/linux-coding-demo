#ifndef __CLIENT__
#define __CLIENT__

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


#include <sys/sendfile.h>

int openFile(char *file); 
int connectServer(char *ip,int port);
unsigned long getFileSize(int fd);









#endif