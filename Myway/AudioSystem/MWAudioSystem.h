#pragma once

#include "MWSound.h"
#include "MWMusic.h"

namespace Myway
{

class MW_ENTRY AudioSystem : public AllocObj
{
    DECLARE_SINGLETON(AudioSystem);

public:
    AudioSystem();
    virtual ~AudioSystem();

    virtual void Update(const Vec3 & listener) = 0;

    virtual Sound * CreateSound(const char * name, const char * media) = 0;
    virtual Music * CreateMusic(const char * name, const char * media) = 0;

    virtual void DestroySound(const char * name) = 0;
    virtual void DestroyMusic(const char * name) = 0;

    virtual void DestroySound(Sound * pSound) = 0;
    virtual void DestroyMusic(Music * pMusic) = 0;

    virtual Sound * GetSound(const char * name) = 0;
    virtual Music * GetMusic(const char * name) = 0;
};

}