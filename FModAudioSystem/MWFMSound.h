#pragma once

#include "MWFMAudioSystemEntry.h"
#include "MWSound.h"

namespace Myway
{

class FM_ENTRY FMSound : public Sound
{
    static const int len = 32;

public:
    FMSound(const char * name, const char * media);
    virtual ~FMSound();

    virtual void SetPosition(const Vec3 & pos);
    virtual void SetVolume(int iVolume);
    virtual void SetDistance(float dist);

    virtual float GetDistance() const;
    virtual int GetLength() const;
    virtual const Vec3 & GetPosition() const;
    virtual int GetChannel() const;
    virtual int GetVolume() const;

    virtual void Play();
    virtual void Pause();
    virtual void Stop();

    virtual void SetLoop(bool bLoop);

protected:
    FSOUND_SAMPLE * m_pSound;
    DataStreamPtr m_pStream;
    float m_fDistance;
    int m_iChannel;
    Vec3 m_vPosition;
};

}