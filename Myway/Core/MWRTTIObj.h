#pragma once

#include "MWAllocObj.h"

namespace Myway
{

class RTTIObj
{
public:
    RTTIObj(const char * classname, const RTTIObj * baseobj) { mClassName = classname, mBaseObj = baseobj; }

    const char * ClassName() const { return mClassName; }
    const RTTIObj * BaseObj() const { return mBaseObj; }

protected:
    const char * mClassName;
    const RTTIObj * mBaseObj;
};

#define DeclareRTTI() \
public: \
	static const RTTIObj ms_RTTI; \
	\
	virtual const RTTIObj * GetRTTI() const \
	{\
		return &ms_RTTI; \
	}

#define DeclareRootRTTI(classname) \
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
    bool IsTypeOf(const RTTIObj * obj)  const\
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
    bool IsKindOf(const RTTIObj * obj) const \
    { \
        const RTTIObj * rtti = GetRTTI(); \
        while (rtti)                \
        {                           \
            if (rtti == obj)        \
                return true;        \
                                    \
            rtti = rtti->BaseObj(); \
        }                           \
        return false;               \
    } \
    \
    static classname * DynamicCast(const RTTIObj * rtti, classname * obj) \
    {\
        if (obj == NULL)        \
            return 0;           \
                                \
        return obj->IsKindOf(rtti) ? obj : 0; \
    }







#define ImplementRootRTTI(classname) \
    const RTTIObj classname::ms_RTTI(#classname, 0);

#define ImplementRTTI(classname, baseclass) \
    const RTTIObj classname::ms_RTTI(#classname, &baseclass::ms_RTTI);

#define RTTI_TypeOf(classname, obj) \
    classname::IsTypeOf(&classname::ms_RTTI, obj)

#define RTTI_KindOf(classname, obj) \
    classname::IsKindOf(&classname::ms_RTTI, obj)

#define RTTI_StaticCast(classname, obj) \
    ((classname*)obj)

#define RTTI_DynamicCast(classname, obj) \
    ((classname*)classname::DynamicCast(&classname::ms_RTTI, obj))

}