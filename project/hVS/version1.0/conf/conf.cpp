#include "conf.h"

#define IPSTRLEN 20
static char s_server[128][IPSTRLEN] = { 0 };
static int s_server_nums = 0;
static int lobby_server_upd_port = 0;

static int error_fd = -1;
static int warning_fd = -1;
static int info_fd = -1;

static int parse(char *buf,char *key, char *value);
static int trim_string(char *buf);
int conf_init()
{
#define CONFBUFSIZE 600
	char buf[CONFBUFSIZE];
	char key[64];
	char value[512];
	FILE *fp = fopen("conf.txt", "r");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	while(fgets(buf, CONFBUFSIZE,fp) != NULL)
	{
		parse(buf, key, value);
		if (strcmp(key, "conf_room_server") == 0) {
			strncpy(s_server[s_server_nums], value, IPSTRLEN - 1);
			//s_server[s_server_nums][IPSTRLEN - 1] = '\0';
			++s_server_nums;
		}
		else if (strcmp(key, "conf_lobby_server_upd_port") == 0) {
			lobby_server_upd_port = atoi(value);
			if (lobby_server_upd_port <= 0) {
				perror("conf_lobby_server_upd_port");
				exit(1);
			}
		}
		else if (strcmp(key, "log_error_file") == 0) {
			error_fd = open(value, O_APPEND  | O_RDWR); //难道要先判断文件是否存在了？
			if (error_fd < -1) {
				perror("log_error_file");
				exit(1);
			}
		}
		else if (strcmp(key, "log_warning_file") == 0) {
			warning_fd = open(value, O_APPEND  | O_RDWR);
			if (warning_fd < -1) {
				perror("log_warning_file");
				exit(1);
			}
		}
		else if (strcmp(key, "log_info_file") == 0) {
			info_fd = open(value, O_APPEND  | O_RDWR);
			if (info_fd < -1) {
				perror("log_info_file");
				exit(1);
			}
		}
		else {
		}
	}

	return 0;
}

int get_room_server(char *server_arr, int *nums)
{

}
int get_lobby_server_udp_port()
{
	return lobby_server_upd_port;
}

int get_error_fd()
{
	return error_fd;
}
int get_warning_fd()
{
	return warning_fd;
}
int get_info_fd()
{
	return info_fd;
}





int parse(char *buf, char *key, char *value)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int isKeyOk = 0;
	int bufLen = strlen(buf);
	for (i = 0; i < bufLen; ++i) 
	{
		if (isKeyOk == 0) {
			if (buf[i] != '=') {
				key[j++] = buf[i];
			}
			else {
				isKeyOk = 1;
			}
		}
		else {
			value[k++] = buf[i];
		}
	}
	key[j++] = '\0';
	value[k++] = '\0';
	trim_string(key);
	trim_string(value);
	return 0;
}
static int trim_string(char *buf)
{
	char *ptemp = buf;
	while (*buf != '\0')
	{
		if (*ptemp != ' ') {
			*ptemp++ = *buf;
		}
		++buf;
	}
	*ptemp = '\0';
	return 0;
}