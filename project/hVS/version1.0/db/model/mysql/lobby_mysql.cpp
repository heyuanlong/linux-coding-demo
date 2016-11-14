#include "lobby_mysql.h"
#include "base_mysql.h"
#include "log/log.h"

int lobby_db_model_reg_user(const char *name, const char *email)
{
	MYSQL  *mysql;
	char buf[1024];
	mysql = get_mysql_connect();
	if (mysql == NULL) {
		log_error_fd("get_mysql_connect failed\n");
		return -1;
	}
	sprintf(buf, "insert into hvs.user(name,email,inputTime) values('%s','%s',now())", name,email);
	if (mysql_query(mysql, buf) != 0) {
		log_error_fd("sql failed:%s,mysql_errno:%d,mysql_error:%s\n", buf, mysql_errno(mysql), mysql_error(mysql));
		return -1;
	}
	int user_id = mysql_insert_id(mysql);
	return user_id;
}