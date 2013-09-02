#include "StdAfx.h"

#include "NP_LoginRet.h"


NP_LoginRet::NP_LoginRet()
{
    result = 0;
}

NP_LoginRet::~NP_LoginRet()
{
}

short NP_LoginRet::GetPacketId() const
{
    return NPI_LOGIN_RET;
}

short NP_LoginRet::GetPackSize() const
{
    return sizeof(int);
}

void NP_LoginRet::Read(const void * buffer, int size)
{
    ReadBuffer(&result, sizeof(int), buffer, size, 0);
}

void NP_LoginRet::Write(void * buffer, int size)
{
    WriteBuffer(buffer, size, 0, &result, sizeof(int));
}

void NP_LoginRet::SetResult(int x)
{
    result = x;
}

int NP_LoginRet::GetResult() const
{
    return result;
}