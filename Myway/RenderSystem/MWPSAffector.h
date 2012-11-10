#pragma once

#include "MWParticle.h"

namespace Myway
{

class MW_ENTRY PS_Affector : public AllocObj
{
    friend class PS_System;

public:
    PS_Affector(const String & name, const String & type) : m_name(name), m_type(type) {}
    virtual ~PS_Affector() {}

    const String & GetName() { return m_name; }
    const String & GetType() { return m_type; }

protected:
    virtual void _Affect(PS_System * p, float fElapsedTime) = 0;

protected:
    String m_name;
    String m_type;
};

class MW_ENTRY PS_AffectorFactory : public Factory<PS_Affector>
{
public:
    PS_AffectorFactory() {}
    virtual ~PS_AffectorFactory() {}
};

}