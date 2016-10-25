#include "base.h"
#include "server.h"


#define MAXBUF 1500
#define EPOLL_SIZE 2


int main(int argc, char const *argv[])
{

	int port;
	char buf[MAXBUF];
	int online = 0;
	std::list<int> fdList;
	int serversock = -1;
	std::deque<int> clientQueue;
	CommonMutex lock;
	pthread_cond_t has_product;

	mysqlPool::getInstance();

	if (argc < 2){
		printf("arg is too less\n");
		return 0;
	}

	port=atoi(argv[1]);
	serversock=socketinit(port);
	if (serversock == -1)
	{
		perror("socketinit");
		return 0;
	}
	pthread_attr_t attr;
	if (pthread_attr_init(&attr)){
		perror("pthread_attr_init");
		return 0;
	}
	if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
		perror("pthread_attr_setdetachstate");
		return 0;
	}

///------------------回环连表
	controlUnit *first = NULL,pre=NULL,controlGo = NULL;
	for ( int i= 0; i < 4 ; ++i){
		pre = controlGo;
		controlGo = new controlUnit();
		if( NULL == first ){
			first = controlGo;
		}
		controlGo->setNext(pre);

		pthread_t threadId;
		int preturn;
		preturn = pthread_create(&threadId,&attr,deal,(void*)controlGo));
		if (preturn != 0){
			perror("pthread_create")
			continue;
		}
	}
	first->setNext(controlGo);
//-----------------------------

	int epollFd = epoll_create(EPOLL_SIZE);
	if(epollFd == -1){
		perror("epoll create");
		close(serversock);
		return 0;
	}
	struct epoll_event serversockEpollEvent,readyEvent[EPOLL_SIZE];
	serverEpollEvent.events = EPOLLIN;
	serverEpollEvent.data.fd = serversock;
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,serversock,&serverEpollEvent) == -1){
		perror("epoll add serversock");
		close(serversock);
		return 0;
	}

	int acceptClient = 0;
	int readyEventNums = 0 ;
	for ( ;  ; ){
		readyEventNums = epoll_wait(epollFd,readyEvent,EPOLL_SIZE,100);
		if (readyEventNums == 0) //超时了
		{
			cond_sign(first);
		}
		if(readyEventNums > 0)
		{
			if (!(readyEvent[i].events & EPOLLIN)){
				continue;
			}
			//add 和 cond
			int acceptClient = accept(serversock,NULL,NULL);
			if (acceptClient == -1){
				perror("accept client");
				continue;
			}
			printf("a new client:%d\n",acceptClient);		
			controlGo = controlGo->getNext();
			controlGo->push_back(acceptClient);
			pthread_cond_signal(controlGo->getCond());
		}else{
			
		}
	}

	close(serversock);
	return 0;
}

void cond_sign(controlUnit *cont)
{
	controlUnit * first = cont;
	do{
		if (!cont->empty()){
			pthread_cond_signal(cont->getCond());
		}
		cont = cont->getNext();
	}while(first != cont)
}

void* deal(void *param)
{
	controlUnit *cont = (controlUnit*)param;
	MYSQL* sqlConn = NULL;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	char buf[100];

	while(1){
		
		while(cont->empty() || (sqlConn= mysqlPool::getInstance()->getConn())== NULL ){
			pthread_cond_wait(cont->getConn(),cont->getMutex());
		}
		int fd = cont.front();
		cont.pop_front();

		const char * query= "select gameID,productName from test.product_info";
		int re = mysql_query(sqlConn,query);
		if (re == 0){
			res = mysql_store_result(sqlConn);
			while(row = mysql_fetch_row(res)){
				sprintf(buf,"gameID:%s,productName:%s",row[0],row[1]);
				sendMsg(fd,buf,strlen(buf));
			}
		}
		mysqlPool::getInstance()->recycConn(sqlConn);
		
	}
}

int sendMsg(int originFd,char *msg,int msgLen)
{
	int sendLen = 0;
	write(STDOUT_FILENO,msg,msgLen);
	sendLen = send(originFd,msg,msgLen,0);
	if(sendLen < 0){
		perror("send");
	}

}


int socketinit(int port)
{
	int tempSock = socket(AF_INET,SOCK_STREAM,0);
	if(tempSock == -1){
		perror("init socket");
		return -1;
	}
	struct sockaddr_in serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr  = htonl(INADDR_ANY);

	if(bind(tempSock,(struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == -1){
		perror("bind");
		return -1;
	}
	if (listen(tempSock,128) == -1)
	{
		perror("listen");
		return -1;
	}
	return tempSock;

}