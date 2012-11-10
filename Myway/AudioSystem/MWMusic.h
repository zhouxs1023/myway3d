#pragma once

#include "MWMath.h"

namespace Myway
{

class MW_ENTRY Music : public AllocObj
{
    static const int len = 32;

public:
    Music(const char * name, const char * media);
    virtual ~Music();

    const char * GetName() const;
    const char * GetMediaName() const;

    virtual void SetPosition(int position) = 0;
    virtual void SetChannel(int iChannel) = 0;
    virtual void SetVolume(int iVolume) = 0;

    virtual int GetLength() const = 0;
    virtual int GetPosition() const = 0;
    virtual int GetChannel() const = 0;
    virtual int GetVolume() const = 0;

    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;

    virtual void SetLoop(bool bLoop) = 0;

protected:
    TString128 mName;
    TString128 mMedia;
};

}