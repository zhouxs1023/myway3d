#pragma once

#include "MWAllocObj.h"

namespace Myway
{

template <class T>
class SmartPtr : public AllocObj
{
public:
    SmartPtr();
    SmartPtr(T * p);
    SmartPtr(const SmartPtr & p);
    ~SmartPtr();

    void Release();
    T *  c_ptr() const;
    bool NotNull() const;
    bool IsNull() const;
    void SetNull();

    SmartPtr & operator =(T * p);
    SmartPtr & operator =(const SmartPtr & p);
    bool operator ==(const SmartPtr & p) const;
    bool operator !=(const SmartPtr & p) const;
    bool operator ==(const T * p) const;
    bool operator !=(const T * p) const;

    T * operator ->() const;

protected:
    T *    ptr;
};


template <class T>
class SharedPtr : public AllocObj
{
public:
    SharedPtr();
    SharedPtr(T * p);
    SharedPtr(const SharedPtr & p);
    ~SharedPtr();

    void Release();
    T * c_ptr() const;

    SharedPtr & operator =(T * p);
    SharedPtr & operator =(const SharedPtr & p);
    bool operator ==(const SharedPtr & p) const;
    bool operator !=(const SharedPtr & p) const;
    bool operator ==(const T * p) const;
    bool operator !=(const T * p) const;

    T * operator ->() const;



protected:
    T * ptr;
};

#include "MWSmartPtr.inl"

#define DeclareSmartPtr(classname)  \
    class classname;                \
    typedef SmartPtr<classname> classname##Ptr

#define DeclareSharedPtr(classname)  \
    typedef SharedPtr<classname> classname##Ptr

}