#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 1500
#define EPOLL_SIZE 10

char ip[32] = {0};
int port;
int lobbySockLink = -1;
int roomSockLink = -1;

int main(int argc, char const *argv[])
{
	char ip[32] = {0};
	int port;
	char buf[MAXBUF];
	char stdinBuf[MAXBUF];
	

	if (argc < 3){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);

	lobbySockLink = connectServer(ip,port);
	if (lobbySockLink == -1 ){
		return 0;
	}
	int epollFd = epoll_create(EPOLL_SIZE);
	if(epollFd == -1){
		perror("epoll create");
		close(lobbySockLink);
		return 0;
	}
	struct epoll_event serverEpollEvent,stdinEpollEvent,readyEvent[EPOLL_SIZE];
	serverEpollEvent.events = EPOLLIN;
	serverEpollEvent.data.fd = lobbySockLink;
	stdinEpollEvent.events = EPOLLIN;
	stdinEpollEvent.data.fd = STDIN_FILENO;
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,lobbySockLink,&serverEpollEvent) == -1){
		perror("epoll add lobbySockLink");
		close(lobbySockLink);
		return 0;
	}	
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,STDIN_FILENO,&stdinEpollEvent) == -1){
		perror("epoll add STDIN_FILENO");
		close(lobbySockLink);
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
			if (readyEvent[i].data.fd == lobbySockLink ){
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
			else if(readyEvent[i].data.fd == STDIN_FILENO ){
				int readLen = read(STDIN_FILENO,stdinBuf,MAXBUF);
				if(readLen > 0){
					stdinBuf[sendLen] = '\0';
					dealUserMsg(stdinBuf);

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
			else{

			}
		}
	}

	close(lobbySockLink);
	return 0;
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

void dealUserMsg(char *stdinBuf)
{
	char buf[MAXBUF];
	int bufLen;

	if (strcpy(stdinBuf,"reg")){
		getBufReg(buf,&bufLen);
		sendLobby(buf,bufLen);
	}
	else if (strcpy(stdinBuf,"login")){
		getBufLogin(buf,&bufLen);
		sendLobby(buf,bufLen);
	}
	else if (strcpy(stdinBuf,"logout")){
		getBufLogout(buf,&bufLen);	
		sendLobby(buf,bufLen);
	}
	else if (strcpy(stdinBuf,"roomin")){
		getBufRoomIn(buf,&bufLen);
		sendLobby(buf,bufLen);
	}
	else if (strcpy(stdinBuf,"roomout")){
		getBufRoomOut(buf,&bufLen);	
		sendLobby(buf,bufLen);
	}
	else if (strcpy(stdinBuf,"msg")){
		getBufMsg(buf,&bufLen);	
		sendRoom(buf,bufLen);
	}
	else{

	}
}

int sendLobby(const char *buf,const int len)
{
	int len = send(lobbySockLink,buf,len,0);
	return len;
}

int sendRoom(const char *buf,const int len)
{

}


int getBufReg(char *buf,int *bufLen)
{
	lobby_event_t *req = (lobby_event_t*)malloc(sizeof(lobby_event_t));
	memset(req,0,sizeof(lobby_event_t));
	req->m_head.size = sizeof(lobby_event_t);
	req->m_head.from_type = FROM_TYPE_CLIENT;
	req->m_head.cmd = CMD_REG;
	req->user_id = 0;
	req->room_id = 0;
	*bufLen = req->m_head.size
	free(req);
}

int getBufLogin(char *buf,int *bufLen)
{
	lobby_event_t *req = (lobby_event_t*)malloc(sizeof(lobby_event_t));
	memset(req,0,sizeof(lobby_event_t));
	req->m_head.size = sizeof(lobby_event_t);
	req->m_head.from_type = FROM_TYPE_CLIENT;
	req->m_head.cmd = CMD_REG;
	req->user_id = 0;
	req->room_id = 0;
	*bufLen = req->m_head.size
	free(req);
}
int getBufLogout(char *buf,int *bufLen)
{
	lobby_event_t *req = (lobby_event_t*)malloc(sizeof(lobby_event_t));
	memset(req,0,sizeof(lobby_event_t));
	req->m_head.size = sizeof(lobby_event_t);
	req->m_head.from_type = FROM_TYPE_CLIENT;
	req->m_head.cmd = CMD_REG;
	req->user_id = 0;
	req->room_id = 0;
	*bufLen = req->m_head.size
	free(req);
}
int getBufRoomIn(char *buf,int *bufLen)
{
	lobby_event_t *req = (lobby_event_t*)malloc(sizeof(lobby_event_t));
	memset(req,0,sizeof(lobby_event_t));
	req->m_head.size = sizeof(lobby_event_t);
	req->m_head.from_type = FROM_TYPE_CLIENT;
	req->m_head.cmd = CMD_REG;
	req->user_id = 0;
	req->room_id = 0;
	*bufLen = req->m_head.size
	free(req);
}
int getBufRoomOut(char *buf,int *bufLen)
{
	lobby_event_t *req = (lobby_event_t*)malloc(sizeof(lobby_event_t));
	memset(req,0,sizeof(lobby_event_t));
	req->m_head.size = sizeof(lobby_event_t);
	req->m_head.from_type = FROM_TYPE_CLIENT;
	req->m_head.cmd = CMD_REG;
	req->user_id = 0;
	req->room_id = 0;
	*bufLen = req->m_head.size
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

