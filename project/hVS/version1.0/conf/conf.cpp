#include "conf.h"

#define IPSTRLEN 20
static char s_server[128][IPSTRLEN] = { 0 };
static int s_server_nums = 0;
static int lobby_server_upd_port = 0;

static int parse(char *buf,char *key, char *value);
int conf_init()
{
#define CONFBUFSIZE 600
	char buf[CONFBUFSIZE];
	char key[64];
	char value[512];
	FILE *fp = fopen("conf.txt", "r");
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

}



int parse(char *buf, char *key, char *value)
{

}