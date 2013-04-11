#include "MWSound.h"

namespace Myway
{

Sound::Sound(const char * name, const char * media)
: Mover(name)
, mMedia(media)
{
}

Sound::~Sound()
{
}

const char * Sound::GetMediaName() const
{
    return mMedia.c_str();
}

}