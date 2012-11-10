#pragma once

#include "MWTString.h"

namespace Myway
{

template <class T>
class Factory
{
    DECLARE_ALLOC();

public:
    Factory() {}
    virtual ~Factory() {}

    virtual const TString128 & GetType() const = 0;

    virtual T * CreateInstance(const TString128 & name) = 0;
    virtual void DestroyInstance(T * myclass) = 0;
};

}