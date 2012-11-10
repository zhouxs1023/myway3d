inline RefObj::RefObj() : mReferenceCount(0)
{
}

inline RefObj::~RefObj()
{
    assert(mReferenceCount == 0);
}

inline int RefObj::IncRef()
{
    ++mReferenceCount;
    return mReferenceCount;
}

inline int RefObj::DecRef()
{
    assert(mReferenceCount > 0);
    --mReferenceCount;
    return mReferenceCount;
}

inline int RefObj::RefCount()
{
    return mReferenceCount;
}