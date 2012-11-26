#pragma once

#include "MWAllocObj.h"

namespace Myway
{

class RTTIObj : public AllocObj
{
public:
    RTTIObj(const char * classname, const RTTIObj * baseobj) { mClassName = classname, mBaseObj = baseobj; }

    const char * ClassName() const { return mClassName; }
    const RTTIObj * BaseObj() const { return mBaseObj; }

protected:
    const char * mClassName;
    const RTTIObj * mBaseObj;
};

#define DeclareRootRTTIObj(classname) \
public: \
    static const RTTIObj ms_RTTI; \
    \
    virtual const RTTIObj * GetRTTI() const \
    {\
        return &ms_RTTI; \
    }\
    \
    static bool IsTypeOf(const RTTIObj * rtti, const classname * obj)  \
    {\
        if (!obj)                   \
            return false;           \
                                    \
        return obj->IsTypeOf(rtti); \
    }\
    \
    bool IsTypeOf(const RTTIObj * obj) \
    { \
        return GetRTTI() == obj; \
    } \
    \
    static bool IsKindOf(const RTTIObj * rtti, const classname * obj)  \
    {\
        if (!obj)                   \
            return false;           \
                                    \
        return obj->IsKindOf(rtti); \
    }\
    \
    bool IsKindOf(const RTTIObj * obj) \
    { \
        RTTIObj * rtti = GetRTTI(); \
        while (rtti)                \
        {                           \
            if (rtti == obj)        \
                return true;        \
                                    \
            rtti = rtti->BaseObj(); \
        }                           \
        return false                \
    } \
    \
    static classname * DynamicCast(const NiRTTI* rtti, classname* obj) const \
    {\
        if (obj)                \
            return 0;           \
                                \
        return obj->DynamicCast(rtti); \
    }\
    \
    classname * DynamicCast(const NiRTTI* rtti) const \
    {\
        return (IsKindOf(rtti) ? (classname*) this : 0 ); \
    }\

#define DeclareRTTI \
public: \
    static const RTTIObj ms_RTTI; \
    \
    virtual const RTTIObj * GetRTTI() const     { return &ms_RTTI; }

#define ImplementRootRTTI(classname) \
    const RTTIObj classname::ms_RTTI(#classname, 0);

#define ImplementRTTI(classname, baseclass) \
    const RTTIObj classname::ms_RTTI(#classname, &baseclass::ms_RTTI);

#define _IsTypeOf(classname, obj) \
    classname::IsTypeOf(&classname::ms_RTTI, obj)

#define _IsKindOf(classname, obj) \
    classname::IsKindOf(&classname::ms_RTTI, obj)

#define _StaticCast(classname, obj) \
    (classname*) obj

#define _DynamicCast(classname, obj) \
    classname::DynamicCast(&classname::ms_RTTI, obj)

}