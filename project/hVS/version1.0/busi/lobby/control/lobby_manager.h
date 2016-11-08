#ifndef __LOBBY_MANAGER__H__
#define __LOBBY_MANAGER__H__
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#include "busi/lobby/model/lobby.h"
#include "event/event.h"

#define FROM_TYPE_CLIENT 1000
#define CMD_REG 1001
#define CMD_LOGIN 1002
#define CMD_LOGOUT 1003
#define CMD_ENTER_ROOM 1004

#define FROM_TYPE_ROOM_SERVER 2000
#define CMD_GOOUT_ROOM 2001

#define TO_TYPE_CLIENT 1100
#define BACK_REG 1101
#define BACK_LOGIN 1102
#define BACK_LOGOUT 1103
#define BACK_ENTER_ROOM 1104

typedef struct lobby_busi_head_s lobby_busi_head_t;
struct lobby_busi_head_s {
	int size;
	int from_type;
	int cmd;
};

typedef struct lobby_event_s lobby_event_t;
struct lobby_event_s {
	lobby_busi_head_t m_head;
	int user_id;
	int room_id;
};

typedef struct lobby_callback_s lobby_callback_t;
struct lobby_callback_s {
	lobby_busi_head_t m_head;
	int user_id;
	int room_id;
	int status;
};


typedef struct lobby_busi_s lobby_busi_t;
struct lobby_busi_s {
	lobby_event_t *m_event;
	struct sockaddr_in *addr;
};

typedef int(*busi_lobby_callback_handle_t)(lobby_busi_t *busi);


int busi_lobby_init();
int busi_lobby_loop();

#endif __LOBBY_MANAGER__H__