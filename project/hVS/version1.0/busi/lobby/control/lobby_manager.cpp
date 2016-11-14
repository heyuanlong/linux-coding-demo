#include "lobby_manager.h"
#include "event/event.h"
#include "log/log.h"
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
	event_lobby_init();
	int res = event_lobby_loop(busi_lobby_callback_handle); //里面是循环
	return res;
}



int busi_lobby_callback_handle(lobby_busi_t *busi)
{
	static int  head_size = sizeof(lobby_busi_head_t);

	if (busi->data_len < head_size) {
		printf("net_upd_recv recv_len < head_size:%d < %d\n", busi->data_len, head_size);
		return 0;
	}

	lobby_busi_head_t *head = (lobby_busi_head_t*)busi->data;
	if (busi->data_len < head->size) {
		printf("net_upd_recv recv_len < head->size:%d < %d\n", busi->data_len, head->size);
		return 0;
	}
	
	log_info_fd("busi_lobby_callback_handle from_type:%d,cmd:%d\n", head->from_type, head->cmd);
	switch (head->from_type)
	{
	case FROM_TYPE_CLIENT:
		switch (head->cmd)
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
		switch (head->cmd)
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
	lobby_busi_reg_t *reg = (lobby_busi_reg_t*)busi->data;
	char name[51] = {0};
	char email[51] = { 0 };

	int user_id;
	lobby_callback_t cback;

	sprintf(name, "%s", reg->name);
	sprintf(email, "%s", reg->email);

	user_id = busi_lobby_model_reg(name, email);
	if (user_id < 0) {
		printf("busi_lobby_reg have failed\n");
	}
	cback.m_head.size = sizeof(lobby_callback_t);
	cback.m_head.from_type = TO_TYPE_CLIENT;
	cback.m_head.cmd = BACK_REG;
	cback.user_id = user_id;
	cback.room_id = 0;
	cback.status = 0;

	busi->data = (void*)(&cback);
	busi->data_len = cback.m_head.size;
	event_lobby_reg_notify(busi);

	return 0;
}
int busi_lobby_login(lobby_busi_t* busi)
{
	int res;
	int is_reg;
	is_reg = busi_lobby_model_is_reg(11);
	if (is_reg < 0) {

	}
	res = busi_lobby_model_login(11);
	if (res < 0) {

	}
	event_lobby_login_notify(busi);
	return 0;
}
int busi_lobby_logout(lobby_busi_t* busi)
{
	int res;
	int is_login;
	is_login = busi_lobby_model_is_login(11);
	if (is_login < 0) {

	}
	res = busi_lobby_model_logout(11);
	if (res < 0) {

	}
	event_lobby_logout_notify(busi);
	return 0;
}
int busi_lobby_enter_room(lobby_busi_t* busi)
{
	int room_id;
	int is_login;
	is_login = busi_lobby_model_is_login(11);
	if (is_login < 0) {

	}
	room_id = busi_lobby_model_enter_room(11,0);
	if (room_id < 0) {

	}
	//回复用户等待。
	return 0;
}
int busi_lobby_goout_room(lobby_busi_t* busi)
{
	int res;
	int is_login;
	is_login = busi_lobby_model_is_login(11);
	if (is_login < 0) {

	}
	res = busi_lobby_model_goout_room(11,0);
	if (res < 0) {

	}
	//回复用户等待。
	return 0;
}