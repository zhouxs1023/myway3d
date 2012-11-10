
#ifdef MW_MYSQL_DATABASE

#ifdef MW_PLATFORM_WIN32
#ifndef _WINSOCKAPI_
#include <winsock.h>
#endif
#endif

#include "mysql.h"

#elif defined(MW_SQLITE3_DATABASE)

#include "sqlite3.h"

#endif



#include "MWDatabase.h"


using namespace Myway;

Database::Database()
: m_sql(NULL)
{
}

Database::~Database()
{
}

bool Database::IsOpen()
{
    return m_sql != NULL;
}

#ifdef MW_SQLITE3_DATABASE

bool Database::Connect(const char * db, const char * host, const char * user, const char * password, int port)
{
    assert (!m_sql);

    return sqlite3_open_v2(db, &(sqlite3*)m_sql, SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK;
}

void Database::Close()
{
    assert (m_sql);

    sqlite3_close((sqlite3*)m_sql);
    m_sql = NULL;
}

bool Database::Execute(const char * sql)
{
    assert (IsOpen());

#ifdef _DEBUG

    char * error = NULL;
    
    if (sqlite3_exec((sqlite3*)m_sql, sql, NULL, NULL, &error) != SQLITE_OK)
    {
        debug_assert(0, error);
        sqlite3_free(error);
        return FALSE;
    }

    return TRUE;

#else

    return sqlite3_exec((sqlite3*)m_sql, sql, NULL, NULL, NULL)  == SQLITE_OK;

#endif

}

bool Database::Execute(const char * sql, DataTable ** table)
{
    assert (IsOpen() && table);

    int result = 0;
    int rows = 0;
    int cols = 0;
    char ** data = NULL;

#ifdef _DEBUG

    char * error = NULL;

    if (sqlite3_get_table((sqlite3*)m_sql, sql, &data, &rows, &cols, &error) != SQLITE_OK)
    {
        debug_assert(0, error);
        sqlite3_free(error);
        return FALSE;
    }

#else

    if (sqlite3_get_table((sqlite3*)m_sql, sql, &data, &rows, &cols, NULL) != SQLITE_OK)
        return FALSE;

#endif

    if (rows <= 0 && cols <= 0)
        return TRUE;

    DataTable * dt = new DataTable();

    dt->m_rows = rows;
    dt->m_cols = cols;

    dt->m_columns = (DataTable::Column *)alloc_mem(DataTable::MAX_COLUMN_LEN * cols);
    dt->m_values = (DataTable::Value *)alloc_mem(DataTable::MAX_VALUE_LEN * cols * rows);

    assert (dt->m_columns && dt->m_values);

    int index = 0;

    for (int i = 0; i < cols; ++i)
    {
        const char * src = data[index++];
        Strcpy(dt->m_columns[i].name, DataTable::MAX_COLUMN_LEN, src ? src : "");
    }

    for (int i = 0; i < rows * cols; ++i)
    {
        const char * src = data[index++];
        Strcpy(dt->m_values[i].val, DataTable::MAX_VALUE_LEN, src ? src : "");
    }

    dt->Reset();

    *table = dt;


    return TRUE;
}

#elif defined(MW_MYSQL_DATABASE)


bool Database::Connect(const char * db, const char * host, const char * user, const char * password, int port)
{
    assert (!m_sql);

    return FALSE;
}

void Database::Close()
{
    assert (m_sql);
}

bool Database::Execute(const char * sql)
{
    assert (IsOpen());

    return FALSE;
}

bool Database::Execute(const char * sql, DataTable ** table)
{
    assert (IsOpen());

    return FALSE;
}

#endif

void Database::FreeTable(DataTable * table)
{
    assert (table);

    safe_free(table->m_columns);
    safe_free(table->m_values);

    safe_delete(table);
}
