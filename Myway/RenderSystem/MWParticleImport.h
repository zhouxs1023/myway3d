#pragma once

#include "MWParticleSystem.h"

namespace Myway
{

class PS_Loader
{
public:
    PS_Loader() {}
    virtual ~PS_Loader() {}

    virtual ParticleSystem * Load(const String & name, const String & source) = 0;
    virtual void Save(ParticleSystem * ps, const String & filename) = 0;
};

class PS_Loader_Default : public PS_Loader
{
public:
    PS_Loader_Default() {}
    virtual ~PS_Loader_Default() {}

    virtual ParticleSystem * Load(const String & name, const String & source);
    virtual void Save(ParticleSystem * ps, const String & filename);
};


}