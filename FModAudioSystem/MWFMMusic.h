#pragma once

#include "MWFMAudioSystemEntry.h"
#include "MWMusic.h"


namespace Myway
{
    
class FM_ENTRY FMMusic : public Music
{
public:
    FMMusic(const char * name, const char * media);
    virtual ~FMMusic();

    virtual void SetPosition(int position);
    virtual void SetChannel(int iChannel);
    virtual void SetVolume(int iVolume);

    virtual int GetLength() const;
    virtual int GetPosition() const;
    virtual int GetChannel() const;
    virtual int GetVolume() const;

    virtual void Play();
    virtual void Pause();
    virtual void Stop();

    virtual void SetLoop(bool bLoop);

protected:
    int m_iChannel;
    FSOUND_STREAM * m_pMusic;
    DataStreamPtr m_pStream;
};

}