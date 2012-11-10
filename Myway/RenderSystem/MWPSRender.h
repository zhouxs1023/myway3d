#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class RenderQueue;

class MW_ENTRY PS_Render : public AllocObj
{
    friend class PS_System;

public:
    PS_Render(const String & name, const String & type) : _myParent(NULL), _myName(name), _myType(type) {}
    virtual ~PS_Render() {}

    PS_System * GetPartent() const { return _myParent; }
    const String & GetName() const { return _myName; }
    const String & GetType() const { return _myType; }

    virtual void NotifyResizeQuota() = 0;
    virtual void Update() = 0;
    virtual void AddRenderQueue(RenderQueue * rq) = 0;

protected:
    PS_System * _myParent;
    String      _myName;
    String      _myType;
};

class MW_ENTRY PS_RenderFactory : public Factory<PS_Render>
{
public:
    PS_RenderFactory() {}
    virtual ~PS_RenderFactory() {}
};

}