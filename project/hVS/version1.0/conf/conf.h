#ifndef __CONF__H__
#define __CONF__H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int conf_init();
int get_room_server(char *server_arr, int *nums);
int get_lobby_server_udp_port();


#endif __CONF__H__