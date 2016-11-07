#include "lobby_manager.h"

lobby_t *s_lobby;


int busi_lobby_callback_handle(lobby_busi_t *busi);
int busi_lobby_reg(lobby_busi_t* busi);
int busi_lobby_login(lobby_busi_t* busi);
int busi_lobby_logout(lobby_busi_t* busi);
int busi_lobby_enter_room(lobby_busi_t* busi);
int busi_lobby_goout_room(lobby_busi_t* busi);



int busi_lobby_init()
{

	s_lobby = (lobby_t*)malloc(sizeof(lobby_t));
	s_lobby->lobby_id = 1;
	s_lobby->room_nums = 0;

}

int busi_lobby_loop()
{
	int res = event_lobby_loop(busi_lobby_callback_handle); //里面是循环
	return res;
}



int busi_lobby_callback_handle(lobby_busi_t *busi)
{
	switch (busi->m_event->m_head.from_type)
	{
	case FROM_TYPE_CLIENT:
		switch (busi->m_event->m_head.cmd)
		{
		case CMD_REG:
			busi_lobby_reg(busi);
			break;
		case CMD_LOGIN:
			busi_lobby_login(busi);
			break;
		case CMD_LOGOUT:
			busi_lobby_logout(busi);
			break;
		case CMD_ENTER_ROOM:
			busi_lobby_enter_room(busi);
			break;
		default:
			break;
		}
		break;
	case FROM_TYPE_ROOM_SERVER:
		switch (busi->m_event->m_head.cmd)
		{
		case CMD_GOOUT_ROOM:
			busi_lobby_goout_room(busi);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
int busi_lobby_reg(lobby_busi_t* busi)
{
	int user_id;
	user_id = busi_lobby_model_reg();
	if (user_id < 0) {
	}
	busi->m_event->user_id = user_id;
	busi_lobby_model_reg_notify(busi);

	return 0;
}
int busi_lobby_login(lobby_busi_t* busi)
{
	int res;
	int is_reg;
	is_reg = busi_lobby_model_is_reg(busi->m_event->user_id);
	if (is_reg < 0) {

	}
	res = busi_lobby_model_login(busi->m_event->user_id);
	if (res < 0) {

	}
	busi_lobby_model_login_notify(busi);
	return 0;
}
int busi_lobby_logout(lobby_busi_t* busi)
{
	int res;
	int is_login;
	is_login = busi_lobby_model_is_login(busi->m_event->user_id);
	if (is_login < 0) {

	}
	res = busi_lobby_model_logout(busi->m_event->user_id);
	if (res < 0) {

	}
	busi_lobby_model_logout_notify(busi);
	return 0;
}
int busi_lobby_enter_room(lobby_busi_t* busi)
{
	int room_id;
	int is_login;
	is_login = busi_lobby_model_is_login(busi->m_event->user_id);
	if (is_login < 0) {

	}
	room_id = busi_lobby_model_enter_room(busi->m_event->user_id,busi->m_event->room_id);
	if (room_id < 0) {

	}
	//回复用户等待。
	return 0;
}
int busi_lobby_goout_room(lobby_busi_t* busi)
{
	int res;
	int is_login;
	is_login = busi_lobby_model_is_login(busi->m_event->user_id);
	if (is_login < 0) {

	}
	res = busi_lobby_model_goout_room(busi->m_event->user_id, busi->m_event->room_id);
	if (res < 0) {

	}
	//回复用户等待。
	return 0;
}