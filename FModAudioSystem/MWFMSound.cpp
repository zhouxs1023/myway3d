#include "MWFMSound.h"
#include "MWResourceManager.h"

namespace Myway
{

#define DEFAULT_DISTANCE 50.0f
#define DEFAULT_POSITION Vec3::Zero

FMSound::FMSound(const char * name, const char * media)
: Sound(name, media)
{
	m_fMinDistance = 0;
    m_fMaxDistance = DEFAULT_DISTANCE;
    m_iChannel = -1;
	m_iVolume = 1000;
    m_pStream = ResourceManager::Instance()->OpenResource(media);

    m_pSound = FSOUND_Sample_Load(FSOUND_FREE, (const char *)m_pStream->GetData(),
                               FSOUND_HW3D | FSOUND_LOADMEMORY, 0, m_pStream->Size());

	m_iLength = m_pSound ? FSOUND_Sample_GetLength(m_pSound) : 0;

    SetLoop(true);
}

FMSound::~FMSound()
{
    FSOUND_Sample_Free(m_pSound);
}

void FMSound::SetPosition(const Vec3 & pos)
{
    m_vPosition = pos;
}

void FMSound::SetVolume(int iVolume)
{
	m_iVolume = iVolume;
}

void FMSound::SetDistance(float start, float end)
{
	m_fMinDistance = start;
    m_fMaxDistance = end;
}

float FMSound::GetMinDistance() const
{
    return m_fMinDistance;
}

float FMSound::GetMaxDistance() const
{
	return m_fMaxDistance;
}

int FMSound::GetLength() const
{
    return m_iLength;
}

const Vec3 & FMSound::GetPosition() const
{
    return m_vPosition;
}

int FMSound::GetChannel() const
{
    return m_iChannel;
}

int FMSound::GetVolume() const
{
    return m_iVolume;
}

void FMSound::Play()
{
	if (m_iChannel == -1)
		m_iChannel = FSOUND_PlaySoundEx(FSOUND_FREE, m_pSound, NULL, FALSE);
}

void FMSound::Pause()
{
    if (m_iChannel != -1)
        FSOUND_SetPaused(m_iChannel, TRUE);
}

void FMSound::Stop()
{
    if (m_iChannel != -1)
        FSOUND_StopSound(m_iChannel);

	m_iChannel = -1;
}

void FMSound::SetLoop(bool bLoop)
{
    if (bLoop)
    {
        FSOUND_Sample_SetMode(m_pSound, FSOUND_LOOP_NORMAL);
    }
    else
    {
        FSOUND_Sample_SetMode(m_pSound, FSOUND_LOOP_OFF);
    }
}


}