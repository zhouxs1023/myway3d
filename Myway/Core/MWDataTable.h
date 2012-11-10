#pragma once

#include "MWAllocObj.h"

namespace Myway
{

class MW_ENTRY DataTable : public AllocObj
{
    friend class Database;

    static const int MAX_COLUMN_LEN = 256;
    static const int MAX_VALUE_LEN = 1024;

    struct Column
    {
        char name[MAX_COLUMN_LEN];
    };

    struct Value
    {
        char val[MAX_VALUE_LEN];
    };

public:
    DataTable();
    ~DataTable();

    void            MoveNext();
    void            Seek(int pos);
    void            Skip(int k);
    bool            Empty() const;
    bool            Endof() const;
    void            Reset();
    int           GetPosition() const;
    int          GetRows() const;
    int          GetColumns() const;

    int	            GetIntByName(const char * name) const;
    float           GetFloatByName(const char * name) const;
    double          GetDoubleByName(const char * name) const;
    const char *    GetStringByName(const char * name) const;

    int           GetInt(int index) const;
    float           GetFloat(int index) const;
    double          GetDouble(int index) const;
    const char *    GetString(int index) const;

protected:
    int          m_rows;
    int          m_cols;			
    int           m_cursor;		
    bool            m_endof;
    Column *		m_columns;
    Value *	        m_values;
};

#include "MWDataTable.inl"

}