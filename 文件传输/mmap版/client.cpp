#include "base.h"
#include "client.h"
#include <time.h>

#define MAXLINE 1024

int main(int argc, char const *argv[])
{
	char ip[32] = {0};
	int port;
	char fileName[1024] = {0};
	char buf[MAXLINE];
	int fp = -1;
	int sockLink = -1;

	if (argc < 4){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);
	strcpy(fileName,argv[3]);

	fp = openFile(fileName);
	if (fp == -1){
		return 0;
	}
	unsigned long fileSize = getFileSize(fp);
	if (fileSize == -1){
		close(fp);
		return 0;
	}
	char *mmapFile = (char *)mmap(NULL,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0);
	if (mmapFile == MAP_FAILED){
		perror("mmap");
		close(fp);
		return 0;
	}
	close(fp);

	sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		munmap(mmapFile,fileSize);
		return 0;
	}

	//这个有个bug，粘包没处理
	int sendLen = send(sockLink,fileName,strlen(fileName),0);
	if (sendLen < 0){
		perror("send file name");
		munmap(mmapFile,fileSize);
		close(sockLink);
		return 0 ;
	}
	sendLen = send(sockLink,&fileSize,sizeof(fileSize),0);
	if (sendLen < 0){
		perror("send file size");
		munmap(mmapFile,fileSize);
		close(sockLink);
		return 0 ;
	}

	clock_t start, finish;  
	start = clock();
	unsigned long sendFileSize = 0;
	while( (sendLen = send(sockLink,mmapFile + sendFileSize ,fileSize - sendFileSize,0)) > 0 ){
		sendFileSize += sendLen;
	}
	if (sendFileSize != fileSize){
		perror("send file!");
		munmap(mmapFile,fileSize);
		close(sockLink);
		return 0 ;
	}
	finish = clock();
	int dur = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f\n", dur);
	
	munmap(mmapFile,fileSize);
	close(sockLink);
	return 0;
}


unsigned long getFileSize(int fd)
{
	struct stat file;
	if ( fstat(fd,&file) == -1){
		perror("fstat");
		return -1;
	}
	return file.st_size;
}

int  openFile(char *file)
{
	int temp = open(file,O_RDWR);
	if (temp == -1){
		perror("open file");
		return -1;
	}
	return temp;

}
int connectServer(char *ip,int port)
{
	int clientSoct = socket(AF_INET,SOCK_STREAM,0);
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