#pragma once

#include "MWDefine.h"

#ifdef MW_MYSQL_DATABASE

typedef struct MYSQL *_sql;

#elif defined(MW_SQLITE3_DATABASE)

typedef struct sqlite3 *_sql;

#else

typedef void *_sql;

#endif

#include "MWDataTable.h"

namespace Myway
{


class DataTable;

class MW_ENTRY Database : public AllocObj
{
public:
	        Database();
	        ~Database();

	bool    IsOpen();
	bool	Connect(const char * db, const char * host, const char * user, const char * password, int port);
	void	Close();

	bool	Execute(const char * sql);
	bool	Execute(const char * sql, DataTable ** table);

    void    FreeTable(DataTable * table);

protected:
    _sql    m_sql;
};



}