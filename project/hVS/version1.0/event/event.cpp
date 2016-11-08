#include "event.h"
#define LOBBY_NET_UPD_BUF_SIZE 1500

static int lobby_server_upd_socket = -1;

int event_lobby_init()
{
	int port = get_lobby_server_udp_port();
	lobby_server_upd_socket = net_udp_socket_init(port);
	printf("event_lobby_init lobby_server_upd_socket:%d\n", lobby_server_upd_socket);
	return 0;
}
int event_lobby_loop(busi_lobby_callback_handle_t lobby_callback)
{
	int  head_size = sizeof(lobby_busi_head_t);
	char net_buf[LOBBY_NET_UPD_BUF_SIZE] = { 0 };

	lobby_busi_head_t *head;
	int recv_len;
	struct sockaddr_in addr;
	lobby_busi_t busi;

	for (;;) {
		recv_len = net_upd_recv(lobby_server_upd_socket, net_buf, LOBBY_NET_UPD_BUF_SIZE, &addr);
		printf("net_upd_recv recv_len:%d\n", recv_len);
		if (recv_len < head_size) {
			printf("net_upd_recv recv_len < head_size:%d < %d\n", recv_len, head_size);
			continue;
		}

		head = (lobby_busi_head_t*)net_buf;
		if (recv_len < head->size) {
			printf("net_upd_recv recv_len < head->size:%d < %d\n", recv_len, head->size);
			continue;
		}
		busi.m_event = ((lobby_event_t*)net_buf);
		printf("event_lobby_loop from_type:%d,cmd:%d\n", busi.m_event->m_head.from_type, busi.m_event->m_head.cmd);
		busi.addr = &addr;
		lobby_callback(&busi);
	}

}

int event_lobby_reg_notify(lobby_busi_t *busi)
{
	static lobby_callback_t cback;
	cback.m_head.size = sizeof(lobby_callback_t);
	cback.m_head.from_type = TO_TYPE_CLIENT;
	cback.m_head.cmd = BACK_REG;
	cback.user_id = busi->m_event->user_id;
	cback.room_id = 0;
	cback.status = 0;

	int sendLen = net_upd_send(lobby_server_upd_socket, (const char  *)&cback, cback.m_head.size, busi->addr);
	if (sendLen <= 0) {
		printf("net_upd_send failed\n");
	}
	return 0;
}
int event_lobby_login_notify(lobby_busi_t *busi)
{

}
int event_lobby_logout_notify(lobby_busi_t *busi)
{

}


/*
int event_lobby_cmd(int cmd)
{
	switch (cmd)
	{
	case CMD_REG:
	case CMD_LOGIN:
	case CMD_LOGOUT:
	case CMD_ENTER_ROOM:
	case CMD_GOOUT_ROOM:
		break;
	default:
		return -1;
	}
	return 0;
}
*/