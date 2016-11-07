#ifndef __EVENT__H_
#define __EVENT__H_
#include "busi/lobby/control/lobby_manager.h"
#include "net/net.h"
#include "conf/conf.h"


#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int event_lobby_init();
int event_lobby_loop(busi_lobby_callback_handle_t lobby_callback);


int event_lobby_reg_notify(lobby_busi_t *busi);
int event_lobby_login_notify(lobby_busi_t *busi);
int event_lobby_logout_notify(lobby_busi_t *busi);

#endif