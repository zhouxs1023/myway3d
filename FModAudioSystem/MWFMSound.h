#pragma once

#include "MWFMAudioSystemEntry.h"
#include "MWSound.h"

namespace Myway
{

class FM_ENTRY FMSound : public Sound
{
    static const int len = 32;

	friend class FMAudioSystem;

public:
    FMSound(const char * name, const char * media);
    virtual ~FMSound();

    virtual void SetPosition(const Vec3 & pos);
    virtual void SetVolume(int iVolume);
    virtual void SetDistance(float minDist, float maxDist);

	virtual float GetMinDistance() const;
    virtual float GetMaxDistance() const;

    virtual int GetLength() const;
    virtual const Vec3 & GetPosition() const;
    virtual int GetChannel() const;
    virtual int GetVolume() const;

    virtual void Play();
    virtual void Pause();
    virtual void Stop();

    virtual void SetLoop(bool bLoop);

	void _SetName(const char * name) { mName = name; }

protected:
    FSOUND_SAMPLE * m_pSound;
    DataStreamPtr m_pStream;
	float m_fMinDistance;
    float m_fMaxDistance;
	int m_iVolume;
	int m_iLength;
    int m_iChannel;
    Vec3 m_vPosition;
};

}