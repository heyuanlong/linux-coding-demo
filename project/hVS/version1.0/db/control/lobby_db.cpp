#include "lobby_db.h"
#include "db/model/mysql/lobby_mysql.h"
int lobby_db_control_reg_user(const char *name)
{
	return lobby_db_model_reg_user(name);
}