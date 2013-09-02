#pragma once

#include "NetPack.h"

class NP_ENTRY NP_LoginRet : public NetPacket
{
public:
    enum
    {
        RET_FAILED,

        RET_SUCCESSED,
        RET_NUMBER_ERROR,
        RET_PASSWORD_ERROR,
    };

public:
    NP_LoginRet();
    virtual ~NP_LoginRet();

    virtual short GetPacketId() const;
    virtual short GetPackSize() const;

    virtual void Read(const void * buffer, int size);
    virtual void Write(void * buffer, int size);

    void SetResult(int x);
    int GetResult() const;

protected:
    int result;
};