#pragma once

#include "MWSocket.h"


namespace Myway
{
#define NS_SIZE         64 * 1024

#define NS_ERROR         -1
#define NS_OVERFLOW      -2

class MW_ENTRY NetIStream : public AllocObj
{
public:
    NetIStream(Socket * sock);
    ~NetIStream();

    int Refresh();

    int Read(void * buff, int size);
    bool Skip(void * buff, int size);
    int Skip(int size);

    void Clear();
    bool Empty() const;
    int Length() const;
    int Capacity() const;
    
protected:
    Socket *    m_sock;

    char        m_buffer[NS_SIZE];
    int         m_cursor;
    int         m_length;
    int         m_size;
};








class MW_ENTRY NetOStream : public AllocObj
{
public:
    NetOStream(Socket * sock);
    ~NetOStream();

    int Refresh();

    int Write(const void * buff, int size);
    void * GetData();
    const void * GetData() const;

    void Clear();
    bool Empty() const;
    int Length() const;
    int Capacity() const;

protected:
    Socket *    m_sock;

    char        m_buffer[NS_SIZE];
    int         m_length;
    int         m_size;
};

}