#include "MWNetStream.h"

namespace Myway
{

NetIStream::NetIStream(Socket * sock)
{
    m_sock = sock;
    m_size = NS_SIZE;
    m_length = 0;
    m_cursor = 0;

    Memzero(m_buffer, m_size);
}

NetIStream::~NetIStream()
{
}

int NetIStream::Refresh()
{
    int size = Socket::GetReadSize(m_sock);

	if (size == 0)
		return 0;

    if (m_cursor > 0)
    {
        Memcpy(&m_buffer[0], &m_buffer[m_cursor], m_length);
        m_cursor = 0;
    }

    if (m_length + size > m_size)
    {
        return NS_OVERFLOW;
    }
    else
    {
        int rcvs = Socket::Recv(m_sock, &m_buffer[m_length], size);

        if (rcvs == NET_ERROR)
            return NS_ERROR;
        else if (rcvs == NET_ERROR_WOULDBLOCK)
            return 0;

        m_length += rcvs;

        return rcvs;
    }
}

int NetIStream::Read(void * buff, int size)
{
    if (size > m_length)
    {
        return 0;
    }

    Memcpy(buff, &m_buffer[m_cursor], size);

    m_length = m_length - size;
    m_cursor += size;

    return size;
}

bool _cmp(const char * buff1, const char * buff2, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (*buff1++ != *buff2++)
        {
            return false;
        }
    }

    return true;
}

bool NetIStream::Skip(void * buff, int size)
{
    const char * tmp = (const char *)buff;

    int sk = 0;

    while (Length() > size)
    {
        if (!_cmp(&m_buffer[m_cursor], tmp, size))
        {
            Skip(1);
            ++sk;
        }
		else
		{
			Skip(size);
			return true;
		}
    }

	Skip(-sk);

    return false;
}

int NetIStream::Skip(int size)
{
    if (size > m_length)
        size = m_length;

    int newcur = m_cursor + size;

    if (newcur < 0)
        newcur = 0;

    int sk = newcur - m_cursor;
    m_cursor = newcur;
    m_length -= sk;

    return sk;
}

void NetIStream::Clear()
{
    m_cursor = 0;
    m_length = 0;
}

bool NetIStream::Empty() const
{
    return m_length == 0;
}

int NetIStream::Length() const
{
    return m_length;
}

int NetIStream::Capacity() const
{
    return m_size;
}














NetOStream::NetOStream(Socket * sock)
{
    m_sock = sock;
    m_size = NS_SIZE;
    m_length = 0;

    Memzero(m_buffer, m_size);
}

NetOStream::~NetOStream()
{
}

int NetOStream::Refresh()
{
    if (m_length > m_size)
        return NS_OVERFLOW;

    int sum = 0;

    while (m_length > 0)
    {
        int snds = Socket::Send(m_sock, &m_buffer[0], m_length);

        if (snds == NET_ERROR_WOULDBLOCK)
            return 0;
        else if (snds == NET_ERROR)
            return NS_ERROR;
        else if (snds == 0)
            return 0;

        Memcpy(&m_buffer[0], &m_buffer[snds], snds);
        m_length -= snds;
        sum += snds;
    }

    return sum;
}

int NetOStream::Write(const void * buff, int size)
{
    int newlen = m_length + size;

    if (newlen > m_size)
    {
        m_length = newlen;
        return NS_OVERFLOW;
    }

    Memcpy(&m_buffer[m_length], buff, size);
    m_length += size;
    
    return size;
}

void * NetOStream::GetData()
{
    return m_buffer;
}

const void * NetOStream::GetData() const
{
    return m_buffer;
}

void NetOStream::Clear()
{
    m_length = 0;
}

bool NetOStream::Empty() const
{
    return m_length == 0;
}

int NetOStream::Length() const
{
    return m_length;
}

int NetOStream::Capacity() const
{
    return m_size;
}

}
