#pragma once

#include "MWMover.h"

namespace Myway
{

class MW_ENTRY Sound : public Mover
{
	DECLARE_ALLOC();

public:
    Sound(const char * name, const char * media);
    virtual ~Sound();

    const char * GetMediaName() const;

    virtual void SetVolume(int iVolume) = 0;
    virtual void SetDistance(float minDist, float maxDist) = 0;

	virtual float GetMinDistance() const = 0;
    virtual float GetMaxDistance() const = 0;

    virtual int GetLength() const = 0;
    virtual int GetChannel() const = 0;
    virtual int GetVolume() const = 0;

    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;

    virtual void SetLoop(bool bLoop) = 0;

protected:
    TString128 mMedia;
};

}