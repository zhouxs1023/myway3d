#pragma once

#include "MWFMAudioSystemEntry.h"
#include "MWFMSound.h"
#include "MWFMMusic.h"
#include "MWAudioSystem.h"

namespace Myway
{

class FM_ENTRY FMAudioSystem : public AudioSystem
{
    typedef HashMap<const char *, FMSound *, String::chash_t, String::cequal_t> SoundMap;
    typedef HashMap<const char *, FMMusic *, String::chash_t, String::cequal_t> MusicMap;

    static const int MAX_CHANEL = 1000;

public:
    FMAudioSystem(HWND hWnd);
    virtual ~FMAudioSystem();

    virtual void Update(const Vec3 & listener);

    virtual Sound * CreateSound(const char * name, const char * media);
    virtual Music * CreateMusic(const char * name, const char * media);

	virtual bool RenameSound(const char * newName, const char * oldName);
	virtual bool RenameMusic(const char * newName, const char * oldName);

    virtual void DestroySound(const char * name);
    virtual void DestroyMusic(const char * name);

    virtual void DestroySound(Sound * pSound);
    virtual void DestroyMusic(Music * pMusic);

    virtual Sound * GetSound(const char * name);
    virtual Music * GetMusic(const char * name);

protected:
    SoundMap    m_mSounds;
    MusicMap    m_mMusics;
};

}