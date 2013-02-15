#include "MWFMMusic.h"
#include "MWResourceManager.h"

namespace Myway
{

FMMusic::FMMusic(const char * name, const char * media)
    : Music(name, media)
{
    m_iChannel = -1;
    m_pStream = ResourceManager::Instance()->OpenResource(media);
    m_pMusic = FSOUND_Stream_Open((const char *)m_pStream->GetData(), FSOUND_LOADMEMORY, 0, m_pStream->Size());
}

FMMusic::~FMMusic()
{
    if (m_pMusic)
    {
        FSOUND_Stream_Stop(m_pMusic);
        FSOUND_Stream_Close(m_pMusic);
    }
}

void FMMusic::SetPosition(int position)
{
    FSOUND_Stream_SetPosition(m_pMusic, position);
}

void FMMusic::SetChannel(int iChannel)
{
    m_iChannel = iChannel;
}

void FMMusic::SetVolume(int iVolume)
{
    if (m_iChannel != -1)
        FSOUND_SetVolume(m_iChannel, iVolume);
}

int FMMusic::GetLength() const
{
    return FSOUND_Stream_GetLength(m_pMusic);
}

int FMMusic::GetPosition() const
{
    return FSOUND_Stream_GetPosition(m_pMusic);
}

int FMMusic::GetChannel() const
{
    return m_iChannel;
}

int FMMusic::GetVolume() const
{
    if (m_iChannel != -1)
        return FSOUND_GetVolume(m_iChannel);

    return 0;
}

void FMMusic::Play()
{
    m_iChannel = FSOUND_Stream_Play(m_iChannel, m_pMusic);
}

void FMMusic::Pause()
{
    FSOUND_Stream_Stop(m_pMusic);
}

void FMMusic::Stop()
{
    FSOUND_Stream_Stop(m_pMusic);
    FSOUND_Stream_SetPosition(m_pMusic, 0);
}

void FMMusic::SetLoop(bool bLoop)
{
    if (bLoop)
    {
        FSOUND_Stream_SetMode(m_pMusic, FSOUND_NORMAL| FSOUND_LOOP_NORMAL);
        FSOUND_Stream_SetLoopCount(m_pMusic, -1);
    }
    else
    {
        FSOUND_Stream_SetMode(m_pMusic, FSOUND_NORMAL| FSOUND_LOOP_NORMAL);
        FSOUND_Stream_SetLoopCount(m_pMusic, 1);
    }
}
}