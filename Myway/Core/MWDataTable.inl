


//data table
//

inline DataTable::DataTable()
: m_rows(0),
m_cols(0),
m_cursor(0),
m_endof(TRUE),
m_columns(NULL),
m_values(NULL)
{
}

inline DataTable::~DataTable()
{
}

inline void DataTable::Seek(int pos)
{
    m_cursor = pos;
    m_endof = (m_cursor < 0 || m_cursor >= (int)m_rows);
}

inline void DataTable::Skip(int k)
{
    m_cursor += k;
    m_endof = (m_cursor < 0 || m_cursor >= (int)m_rows);
}

inline void DataTable::MoveNext()
{
    ++m_cursor;
}

inline bool DataTable::Empty() const
{
    return m_rows == 0;
}

inline bool DataTable::Endof() const
{
    return m_endof;
}

inline void DataTable::Reset()
{
    m_cursor = 0;
    m_endof = (m_cursor >= (int)m_rows);
}

inline int DataTable::GetPosition() const
{
    return m_cursor;
}

inline int DataTable::GetRows() const
{ 
    return m_rows; 
}

inline int DataTable::GetColumns() const
{ 
    return m_cols;
}

inline int DataTable::GetIntByName(const char * name) const
{
    assert (!m_endof);

    int i = 0;
    int j = m_cols * m_cursor;

    while (i < m_cols && strncmp(m_values[j + i].val, name, MAX_VALUE_LEN) != 0)
        ++i;

    assert (i < m_cols);

    return atoi(m_values[j + i].val);
}

inline float DataTable::GetFloatByName(const char * name) const
{
    assert (!m_endof);

    int i = 0;
    int j = m_cols * m_cursor;

    while (i < m_cols && strncmp(m_values[j + i].val, name, MAX_VALUE_LEN) != 0)
        ++i;

    assert (i < m_cols);

    return (float)atof(m_values[j + i].val);
}

inline double DataTable::GetDoubleByName(const char * name) const
{
    assert (!m_endof);

    int i = 0;
    int j = m_cols * m_cursor;

    while (i < m_cols && strncmp(m_values[j + i].val, name, MAX_VALUE_LEN) != 0)
        ++i;

    assert (i < m_cols);

    return atof(m_values[j + i].val);
}

inline const char * DataTable::GetStringByName(const char * name) const
{
    assert (!m_endof);

    int i = 0;
    int j = m_cols * m_cursor;

    while (i < m_cols && strncmp(m_values[j + i].val, name, MAX_VALUE_LEN) != 0)
        ++i;

    assert (i < m_cols);

    return m_values[j + i].val;
}

inline int DataTable::GetInt(int index) const
{
    assert (!m_endof && index < m_cols);

    return atoi(m_values[m_cols * m_cursor + index].val);
}

inline float DataTable::GetFloat(int index) const
{
    assert (!m_endof && index < m_cols);

    return (float)atof(m_values[m_cols * m_cursor + index].val);
}

inline double DataTable::GetDouble(int index) const
{
    assert (!m_endof && index < m_cols);

    return atof(m_values[m_cols * m_cursor + index].val);
}

inline const char * DataTable::GetString(int index) const
{
    assert (!m_endof && index < m_cols);

    return m_values[m_cols * m_cursor + index].val;
}
