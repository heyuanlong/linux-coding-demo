#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 1500
#define EPOLL_SIZE 2

char ip[32] = {0};
int port;


int main(int argc, char const *argv[])
{
	char ip[32] = {0};
	int port;
	char buf[MAXBUF];
	char stdinBuf[MAXBUF];
	int sockLink = -1;

	if (argc < 3){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);

	sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		return 0;
	}
	int epollFd = epoll_create(EPOLL_SIZE);
	if(epollFd == -1){
		perror("epoll create");
		close(sockLink);
		return 0;
	}
	struct epoll_event serverEpollEvent,stdinEpollEvent,readyEvent[EPOLL_SIZE];
	serverEpollEvent.events = EPOLLIN;
	serverEpollEvent.data.fd = sockLink;
	stdinEpollEvent.events = EPOLLIN;
	stdinEpollEvent.data.fd = STDIN_FILENO;
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,sockLink,&serverEpollEvent) == -1){
		perror("epoll add sockLink");
		close(sockLink);
		return 0;
	}	
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,STDIN_FILENO,&stdinEpollEvent) == -1){
		perror("epoll add STDIN_FILENO");
		close(sockLink);
		return 0;
	}

	int readyEventNums = 0 ;
	for ( ; ; )
	{
		readyEventNums = epoll_wait(epollFd,readyEvent,EPOLL_SIZE,-1);
		for (int i = 0; i < readyEventNums; ++i)
		{
			if (!(readyEvent[i].events & EPOLLIN)){
				continue;
			}
			if (readyEvent[i].data.fd == sockLink ){
				int recvLen = recv(readyEvent[i].data.fd,buf,MAXBUF-1,0);
				if(recvLen > 0){
					write(STDOUT_FILENO,buf,recvLen);

				}else if (recvLen == 0){
					close(readyEvent[i].data.fd);
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					printf("client is close");
					return 0;

				}else{
					perror("a client have error");
					return 0;
				}
			}
			else{
				int readLen = read(STDIN_FILENO,stdinBuf,MAXBUF);
				if(readLen > 0){
					int sendLen = send(sockLink,stdinBuf,readLen,0);
					if (sendLen < 0){
						perror("sockLink have error");
						return 0;
					}

				}else if (readLen == 0){
					close(readyEvent[i].data.fd);
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					printf("STDIN_FILENO is close");
					return 0;

				}else{
					perror("STDIN_FILENO have error");
					return 0;
				}
			}
		}
	}

	close(sockLink);
	return 0;
}



void * deal(void *param)
{
	char buf[MAXBUF];
	int bufLen=0;
	int type=0;

	int sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		return 0;
	}
	int res = 0;
	int tempSendLen = 0;
	int sendLen = 0;
	for(;;){
		res = getBuf(buf,&bufLen,&type);
		tempSendLen = 0;
		for(;;){
			sendLen = send(sockLink,buf + tempSendLen ,bufLen - tempSendLen,0);
			if(sendLen > 0){
				tempSendLen += sendLen;
				if(tempSendLen == bufLen){
					break;
				}
			}else if (sendLen == 0){		
				perror("client is close");
				return 0;
			}else{
				if( errno == EAGAIN || errno == EWOULDBLOCK){
					continue;
				}else{
					close(sockLink);
					perror("client have error");
					return 0;
				}
			}
		}
	}
	close(sockLink);
}

int connectServer(char *ip,int port)
{
	int clientSoct = socket(AF_INET,SOCK_DGRAM,0);
	if (clientSoct == -1){
		perror("init socket");
		return -1;
		/* code */
	}

	struct sockaddr_in clientSockAddr;
	bzero(&clientSockAddr,sizeof(clientSockAddr));
	clientSockAddr.sin_family=AF_INET;
	clientSockAddr.sin_port = htons(port);
	clientSockAddr.sin_addr.s_addr = inet_addr(ip);

	if (connect(clientSoct,(struct sockaddr *)&clientSockAddr,sizeof(struct sockaddr)) == -1){
		perror("connect socket");
		return -1;
	}
	return clientSoct;
}

int getBuf(char *buf,int *bufLen,int *type)
{
	*type = (int)(random() % 4) + LOGIN;
	switch(*type){
		case LOGIN:	getBufLogin(buf,bufLen);
			break;
		case READY:	getBufReady(buf,bufLen);
			break;
		case MSG:	getBufMsg(buf,bufLen);
			break;
		case LEAVE:	getBufLeave(buf,bufLen);
			break;
		default:
			break;

	}
	return *bufLen;
}

int getBufLogin(char *buf,int *bufLen)
{
	login_t *req = (login_t*)malloc(sizeof(login_t));
	memset(req,0,sizeof(login_t));
	req->head.size = sizeof(login_t);
	req->head.cmd = LOGIN;
	req->userid = random() % 100 + 1000 ;
	strcpy(req->token,"this is token");
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}

int getBufReady(char *buf,int *bufLen)
{
	ready_t *req = (ready_t*)malloc(sizeof(ready_t));
	memset(req,0,sizeof(ready_t));
	req->head.size = sizeof(ready_t);
	req->head.cmd = READY;
	req->userid = random() % 100 + 1000 ;
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}

int getBufMsg(char *buf,int *bufLen)
{
	const char * data="this is msg!!!";
	int msgLen = strlen(data);
	int packetLen = sizeof(msg_t) + msgLen;

	msg_t *req = (msg_t*)malloc(packetLen);
	memset(req,0,sizeof(msg_t));
	req->head.size = packetLen;
	req->head.cmd = MSG;
	req->userid = random() % 100 + 1000 ;	
	req->dataSize = msgLen;
	memcpy(req->data,data,msgLen);
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}

int getBufLeave(char *buf,int *bufLen)
{
	leave_t *req = (leave_t*)malloc(sizeof(leave_t));
	memset(req,0,sizeof(leave_t));
	req->head.size = sizeof(leave_t);
	req->head.cmd = LEAVE;
	req->userid = random() % 100 + 1000 ;
	req->score = random() % 100 + 1000 ;
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}
