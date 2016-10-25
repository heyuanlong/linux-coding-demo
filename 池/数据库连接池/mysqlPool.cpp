#include "mysqlPool.h"

static mysqlPool* mysqlPool::mInstance		= NULL;
static int mysqlPool::nums					= 100;
static std::string mysqlPool::smHost		= "localhost";
static std::string mysqlPool::smUser		= "root";
static std::string mysqlPool::smPassword	= "";
static std::string mysqlPool::smDb			= "";

mysqlPool::~mysqlPool()
{
	std::list<MYSQL *>::iterator b,e;
	mLock.lock();
	b = mOutList.begin();
	e = mOutList.end();
	for (; b != e; ++b)
	{
		delete (*b);
	}
	
	b = mInList.begin();
	e = mInList.end();
	for (; b != e; ++b)
	{
		delete (*b);
	}
	mLock.unlock();

}

static mysqlPool* mysqlPool::getInstance()
{
	if (mInstance == NULL)
	{
		static mysqlPool sObj;
		mysqlPool::mInstance = &sObj; 
	}
	return mysqlPool::mInstance;
}

mysqlPool::mysqlPool()
{
	for (int i = 0; i < mysqlPool::nums; ++i)
	{
		MYSQL *temp = (MYSQL*)malloc(sizeof(MYSQL));
		if (temp == NULL){
			printf("malloc failed\n");
			exit(0);
		}
		if(mysql_init(temp) == NULL){
			printf("mysql_init failed\n");
			exit(0);
		}
		if (mysql_real_connect(temp,mysqlPool::smHost,mysqlPool::smUser,mysqlPool::smPassword,mysqlPool::smDb,0,NULL,0) == NULL){
			printf("mysql_real_connect failed\n");
			exit(0);
		}
		mInList.push_front(temp);

	}
}

MYSQL* mysqlPool::getConn()
{
	MYSQL *temp = NULL;
	mLock.lock();
	if (mInList.size() == 0){
		temp = NULL
	}else{
		temp = mInList.front();
		mInList.pop_front();
		mOutList.push_front(temp);
	}
	mLock.unlock();
	return temp;
}

MYSQL* mysqlPool::recycConn(MYSQL *temp)
{

	std::list<MYSQL *>::iterator b,e;
	mLock.lock();
	b = mOutList.begin();
	e = mOutList.end();
	for (; b != e; ++b)
	{
		if( *b == temp){
			break;
		}
	}
	if(b == e){
		mLock.unlock();
		return NULL;
	}

	b = mInList.begin();
	e = mInList.end();
	for (; b != e; ++b)
	{
		if( *b == temp){
			break;
		}
	}
	if(b != e){
		mLock.unlock();
		return NULL;
	}

	mOutList.remove(temp);
	mInList.push_front(temp);
	mLock.unlock();
	return temp;
}

