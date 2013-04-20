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

	FSOUND_3D_Listener_SetAttributes((const float*)&Vec3::Zero, NULL, 0, 0, 1.0f, 0, 1.0f, 0);
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
        FMSound * pSound = whr->second;

		if (pSound->GetAttachNode())
		{
			float minDist = pSound->GetMinDistance();
			float maxDist = pSound->GetMaxDistance();

			Vec3 dir = pSound->GetPosition() - listener;
			float dist = dir.Length();

			if (dist < maxDist)
			{
				float k = (dist - minDist) / (maxDist - minDist + 0.0001f);

				k = Math::Maximum(0.0f, k);
				k = Math::Minimum(1.0f, k);

				//Vec3 pos = dir * k;

				pSound->Play();

				int vol = pSound->GetVolume() * (1 - k);

				FSOUND_SetVolume(pSound->GetChannel(), vol);
				//FSOUND_Sample_SetMinMaxDistance(pSound->m_pSound, 0, maxDist - minDist);
				FSOUND_3D_SetAttributes(pSound->m_iChannel, (const float*)&Vec3::Zero, NULL);
			}
			else
			{
				pSound->Stop();
			}
		}

        ++whr;
    }

    FSOUND_Update();
}

Sound * FMAudioSystem::CreateSound(const char * name, const char * media)
{
    d_assert (GetSound(name) == 0);

    FMSound * sound = new FMSound(name, media);
    m_mSounds.Insert(sound->GetName().c_str(), sound);

    return sound;
}

Music * FMAudioSystem::CreateMusic(const char * name, const char * media)
{
    d_assert (GetMusic(name) == 0);

    FMMusic * music = new FMMusic(name, media);
    m_mMusics.Insert(music->GetName(), music);

    return music;
}

bool FMAudioSystem::RenameSound(const char * newName, const char * oldName)
{
	FMSound * oldSound = (FMSound *)GetSound(oldName);
	FMSound * newSound = (FMSound *)GetSound(newName);

	if (oldSound == NULL || newSound != NULL)
		return false;

	oldSound->SetName(newName);

	return true;
}

bool FMAudioSystem::RenameMusic(const char * newName, const char * oldName)
{
	FMMusic * oldMusic = (FMMusic *)GetMusic(oldName);
	FMMusic * newMusic = (FMMusic *)GetMusic(newName);

	if (oldMusic == NULL || newMusic != NULL)
		return false;

	oldMusic->_SetName(newName);

	return true;
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
    DestroySound(pSound->GetName().c_str());
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