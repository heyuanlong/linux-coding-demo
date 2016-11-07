#include "net.h"

int udp_socket_init(int port)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		return -1;
	}
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = = htons(port);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr)) < 0) {
		return -1;
	}
	return sock;
}

int net_upd_recv(int socket, char *buf, int size, struct sockaddr_in* addr)
{
	int resLen;
	resLen = recvfrom(socket, buf, size, 0,(struct sockaddr*)addr, sizeof(struct sockaddr));
	return resLen;
}

int net_upd_send(int socket, const char  *buf, const int size, struct sockaddr_in* addr)
{
	int sendLen;
	sendLen = sendto(socket, buf, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr));
	return sendLen;
}