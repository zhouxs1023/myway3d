#pragma once

#include "MWAllocObj.h"

namespace Myway
{

#define DECLARE_SINGLETON(classname)  \
protected:                          \
    static classname * msInstance;  \
                                    \
public:                             \
    static classname *         Instance() { d_assert(msInstance); return msInstance; }

#define IMP_SLN(classname) classname * classname::msInstance = NULL

#define INIT_SLN d_assert(!msInstance); msInstance = this
#define SHUT_SLN d_assert(msInstance); msInstance = NULL


}