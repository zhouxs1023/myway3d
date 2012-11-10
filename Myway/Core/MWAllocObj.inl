

inline AllocObj::AllocObj()
{
}


inline AllocObj::~AllocObj()
{
}

inline void * AllocObj::operator new(size_t size, int align, const char * file, int line)
{
    return _alloc(size, align,  file, line);
}

inline void * AllocObj::operator new[](size_t size, int align, const char * file, int line)
{
    return _alloc(size, align, file, line);
}

inline void * AllocObj::operator new(size_t size)
{
    return _alloc(size, MEMORY_ALIGN_NONE, "unknow", 0);
}

inline void * AllocObj::operator new[](size_t size)
{
    return _alloc(size, MEMORY_ALIGN_NONE, "unknow", 0);
}

inline void AllocObj::operator delete(void * p, const char * file, int line)
{
    _free(p, file, line);
}

inline void AllocObj::operator delete[](void * p, const char * file, int line)
{
    _free(p, file, line);
}

inline void AllocObj::operator delete(void * p)
{
    _free(p, "unknow", 0);
}

inline void AllocObj::operator delete[](void * p)
{
    _free(p, "unknow", 0);
}




inline PoolAllocObj::PoolAllocObj()
{
}


inline PoolAllocObj::~PoolAllocObj()
{
}

inline void * PoolAllocObj::operator new(size_t size, int align, const char * file, int line)
{
    return _pool_alloc(size, align,  file, line);
}

inline void * PoolAllocObj::operator new[](size_t size, int align, const char * file, int line)
{
    return _pool_alloc(size, align, file, line);
}

inline void * PoolAllocObj::operator new(size_t size)
{
    return _pool_alloc(size, MEMORY_ALIGN_NONE, "unknow", 0);
}

inline void * PoolAllocObj::operator new[](size_t size)
{
    return _pool_alloc(size, MEMORY_ALIGN_NONE, "unknow", 0);
}

inline void PoolAllocObj::operator delete(void * p, const char * file, int line)
{
    _pool_free(p, file, line);
}

inline void PoolAllocObj::operator delete[](void * p, const char * file, int line)
{
    _pool_free(p, file, line);
}

inline void PoolAllocObj::operator delete(void * p)
{
    _pool_free(p, "unknow", 0);
}

inline void PoolAllocObj::operator delete[](void * p)
{
    _pool_free(p, "unknow", 0);
}
