

#ifdef MW_PLATFORM_WIN32


inline Mutex::Mutex()
{
    InitializeCriticalSection(&mMutex);
}

inline Mutex::~Mutex()
{
    DeleteCriticalSection(&mMutex);
}


inline void Mutex::Lock()
{
    EnterCriticalSection(&mMutex);
}

inline void Mutex::Unlock()
{
    LeaveCriticalSection(&mMutex);
}

#endif
