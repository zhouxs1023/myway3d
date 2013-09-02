#include "StdAfx.h"

#include "DBConnector.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "libmysql.lib")

DBConnector::DBConnector()
{
	mysql_init(&mysql);

	mysql_result = NULL;
	mysql_row = NULL;

	mConnected = false;
	mQueryTime = 0;
}

DBConnector::~DBConnector()
{
	Close();
	mysql_shutdown(&mysql, SHUTDOWN_DEFAULT);
}

void DBConnector::Connect(const char * host, const char * user, const char * password, const char * db)
{
	d_assert (!IsConnect());

	if (mysql_real_connect(&mysql, host, user, password, db, 0, NULL, 0) != NULL)
	{
		mConnected = true;
	}
}

bool DBConnector::IsConnect()
{
	return mConnected;
}

void DBConnector::Close()
{
	if (IsConnect())
		mysql_close(&mysql);

	d_assert(mysql_result != NULL);

	mysql_result = NULL;
	mysql_row = NULL;

	mConnected = false;
	mQueryTime = 0;
}

bool DBConnector::BeginQuery(const char * sql)
{
	d_assert (IsConnect());

	mysql_result = NULL;
	mysql_row = NULL;
	mQueryTime = timeGetTime();

	if (mysql_query(&mysql, sql) == 0)
	{
		mysql_result = mysql_store_result(&mysql);

		return mysql_result != NULL;
	}

	mysql_ping(&mysql);

	if (mysql_query(&mysql, sql) == 0)
	{
		mysql_result = mysql_store_result(&mysql);

		return mysql_result != NULL;
	}

	return false;
}

int DBConnector::EndQuery()
{
	d_assert(mysql_result != NULL);

	mysql_free_result(mysql_result);
	mysql_result = NULL;

	return int(timeGetTime() - mQueryTime);
}

int DBConnector::GetNumRows()
{
	if (mysql_result)
		return (int)mysql_num_rows(mysql_result);

	return 0;
}

int DBConnector::GetNumFields()
{
	if (mysql_result)
		return mysql_num_fields(mysql_result);

	return 0;
}

bool DBConnector::FetchRow()
{
	d_assert(mysql_result != NULL);

	mysql_row = mysql_fetch_row(mysql_result);

	return mysql_row != NULL;
}

const char * DBConnector::GetString(int i)
{
	d_assert (mysql_row != NULL);
	d_assert (i < (int)mysql_num_fields(mysql_result));

	return mysql_row[i];
}

int DBConnector::GetInt(int i)
{
	return atoi(GetString(i));
}

float DBConnector::GetFloat(int i)
{
	return (float)atof(GetString(i));
}