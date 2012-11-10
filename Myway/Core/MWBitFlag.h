#pragma once

#include "MWAllocObj.h"

namespace Myway
{

template <class T> 
class BitFlag : public AllocObj
{
public:
    BitFlag();
    BitFlag(T Flags);
    BitFlag(const BitFlag & Flags);
    ~BitFlag();

    BitFlag & operator =(const BitFlag & Right);
    BitFlag & operator =(T Right);
              operator T() const;
    bool      operator ==(const BitFlag & Right) const;
    bool      operator !=(const BitFlag & Right) const;
    bool      operator ==(T Right) const;
    bool      operator !=(T Right) const;

    void      Clear();
    void      SetFlags(T Flags);
    void      ClearFlags(T Flags);
    void      SetBit(int Bit);
    void      SetBit(int Bit, bool bSetting);
    void      ClearBit(int Bit);
    
    bool      Empty() const;
    bool      TestBit(int Bit) const;
    bool      TestFlags(T Flags) const;
    bool      TestAny(T Flags) const;

    int       BitCount() const;
    int       SetCount() const;

public:
    T   Flags;
};

typedef BitFlag<char>    BFlag8;
typedef BitFlag<short>   BFlag16;
typedef BitFlag<int>     BFlag32;

#include "MWBitFlag.inl"

}