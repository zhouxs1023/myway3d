#include "MWFMAudioSystem.h"

namespace Myway
{

FMAudioSystem::FMAudioSystem(HWND hWnd)
{
    FSOUND_SetHWND(hWnd);
    FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
    FSOUND_SetDriver(0);                   
    FSOUND_Init(44100, MAX_CHANEL, 0);
    FSOUND_SetMixer(FSOUND_MIXER_QUALITY_AUTODETECT);
    FSOUND_SetSpeakerMode(FSOUND_SPEAKERMODE_SURROUND);
}

FMAudioSystem::~FMAudioSystem()
{
    {
        MusicMap::Iterator whr = m_mMusics.Begin();
        MusicMap::Iterator end = m_mMusics.End();

        while (whr != end)
        {
            delete whr->second;
            ++whr;
        }

        m_mMusics.Clear();
    }

    {
        SoundMap::Iterator whr = m_mSounds.Begin();
        SoundMap::Iterator end = m_mSounds.End();

        while (whr != end)
        {
            delete whr->second;
            ++whr;
        }

        m_mSounds.Clear();
    }
}

void FMAudioSystem::Update(const Vec3 & listener)
{
    SoundMap::Iterator whr = m_mSounds.Begin();
    SoundMap::Iterator end = m_mSounds.End();

    while (whr != end)
    {
        Sound * pSound = whr->second;

        float dist = pSound->GetDistance();
        float distSq = Math::VecDistanceSq(listener, pSound->GetPosition());

        if (distSq <= dist * dist)
        {
            FSOUND_SetPaused(pSound->GetChannel(), FALSE);
            FSOUND_3D_Listener_SetAttributes((const float*)&listener, NULL, 0, 0, 1.0f, 0, 1.0f, 0);
        }
        else
        {
            FSOUND_SetPaused(pSound->GetChannel(), TRUE);
        }

        ++whr;
    }

    FSOUND_Update();
}

Sound * FMAudioSystem::CreateSound(const char * name, const char * media)
{
    d_assert (GetSound(name) == 0);

    FMSound * sound = new FMSound(name, media);
    m_mSounds.Insert(sound->GetName(), sound);

    return sound;
}

Music * FMAudioSystem::CreateMusic(const char * name, const char * media)
{
    d_assert (GetMusic(name) == 0);

    FMMusic * music = new FMMusic(name, media);
    m_mMusics.Insert(music->GetName(), music);

    return music;
}

void FMAudioSystem::DestroySound(const char * name)
{
    SoundMap::Iterator whr = m_mSounds.Find(name);
    SoundMap::Iterator end = m_mSounds.End();

    d_assert (whr != end);

    delete whr->second;
    m_mSounds.Erase(whr);
}

void FMAudioSystem::DestroyMusic(const char * name)
{
    MusicMap::Iterator whr = m_mMusics.Find(name);
    MusicMap::Iterator end = m_mMusics.End();

    d_assert (whr != end);

    delete whr->second;
    m_mMusics.Erase(whr);
}

void FMAudioSystem::DestroySound(Sound * pSound)
{
    DestroySound(pSound->GetName());
}

void FMAudioSystem::DestroyMusic(Music * pMusic)
{
    DestroyMusic(pMusic->GetName());
}

Sound * FMAudioSystem::GetSound(const char * name)
{
    SoundMap::Iterator whr = m_mSounds.Find(name);
    SoundMap::Iterator end = m_mSounds.End();

    return whr != end ? whr->second : NULL;
}

Music * FMAudioSystem::GetMusic(const char * name)
{
    MusicMap::Iterator whr = m_mMusics.Find(name);
    MusicMap::Iterator end = m_mMusics.End();

    return whr != end ? whr->second : NULL;
}


}