#include "MWMusic.h"

namespace Myway
{

Music::Music(const char * name, const char * media)
: mName(name)
, mMedia(media)
{
}

Music::~Music()
{
}

const char * Music::GetName() const
{
    return mName.c_str();
}

const char * Music::GetMediaName() const
{
    return mMedia.c_str();
}

}