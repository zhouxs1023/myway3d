#include "MWFMSound.h"
#include "MWResourceManager.h"

namespace Myway
{

#define DEFAULT_DISTANCE 50.0f
#define DEFAULT_POSITION Vec3::Zero

FMSound::FMSound(const char * name, const char * media)
: Sound(name, media)
{
    m_fDistance = 50.0f;
    m_iChannel = -1;
    m_pStream = ResourceManager::Instance()->OpenResource(media);

    m_pSound = FSOUND_Sample_Load(FSOUND_FREE, (const char *)m_pStream->GetData(),
                               FSOUND_HW3D | FSOUND_LOADMEMORY, 0, 0);

    SetPosition(DEFAULT_POSITION);
    SetDistance(DEFAULT_DISTANCE);
    SetLoop(true);
}

FMSound::~FMSound()
{
    FSOUND_Sample_Free(m_pSound);
}

void FMSound::SetPosition(const Vec3 & pos)
{
    m_vPosition = pos;
    FSOUND_3D_SetAttributes(m_iChannel, (const float*)&m_vPosition, NULL);
}

void FMSound::SetVolume(int iVolume)
{
    if (m_iChannel != -1)
        FSOUND_SetVolume(m_iChannel, iVolume);
}

void FMSound::SetDistance(float dist)
{
    m_fDistance = dist;
    FSOUND_Sample_SetMinMaxDistance(m_pSound, 0.0f, dist);  
}

float FMSound::GetDistance() const
{
    return m_fDistance;
}

int FMSound::GetLength() const
{
    return FSOUND_Sample_GetLength(m_pSound);
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
    if (m_iChannel != -1)
        return FSOUND_GetVolume(m_iChannel);

    return 0;
}

void FMSound::Play()
{
    m_iChannel = FSOUND_PlaySoundEx(FSOUND_FREE, m_pSound, NULL, TRUE);
}

void FMSound::Pause()
{
    if (m_iChannel != -1)
        FSOUND_SetPaused(m_iChannel, TRUE);
}

void FMSound::Stop()
{
    if (m_iChannel != -1)
        FSOUND_SetPaused(m_iChannel, TRUE);
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