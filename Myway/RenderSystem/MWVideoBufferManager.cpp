#include "MWVideoBufferManager.h"

namespace Myway
{
    IMP_SLN(VideoBufferManager);

    VideoBufferManager::VideoBufferManager()
    { 
        INIT_SLN; 
    }

    VideoBufferManager::~VideoBufferManager()
    {
        SHUT_SLN;
    }

}