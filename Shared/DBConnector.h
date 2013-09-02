#pragma once

#include "mysql.h"

class DBConnector
{
public:
	DBConnector();
	~DBConnector();

	void Connect(const char * host, const char * user, const char * password, const char * db);
	bool IsConnect();

	void Close();

	/*
		Example:
			
			if (dbc.BeginQuery("SELECT * FROM table"))
			{
				while (dbc.FetchRow())
				{
					const char * user = dbc.GetString(0);
					const char * password = dbc.GetString(1);
					int money = dbc.GetInt(2);

					// do something...
				}

				int queryTime = EndQuery();
			}
	*/
	bool BeginQuery(const char * sql);
	int EndQuery();

	int GetNumRows();
	int GetNumFields();

	bool FetchRow();
	const char * GetString(int i);
	int GetInt(int i);
	float GetFloat(int i);

protected:
	MYSQL mysql;
	MYSQL_RES * mysql_result;
	MYSQL_ROW mysql_row;

	bool mConnected;
	unsigned int mQueryTime;
};