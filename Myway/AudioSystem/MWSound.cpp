#include "MWSound.h"

namespace Myway
{

Sound::Sound(const char * name, const char * media)
: mName(name)
, mMedia(media)
{
}

Sound::~Sound()
{
}

const char * Sound::GetName() const
{
    return mName.c_str();
}

const char * Sound::GetMediaName() const
{
    return mMedia.c_str();
}

}