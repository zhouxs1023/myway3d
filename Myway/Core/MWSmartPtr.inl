




template <class T>
inline SmartPtr<T>::SmartPtr() 
: ptr(NULL)
{
}

template <class T>
inline SmartPtr<T>::SmartPtr(T * p)
: ptr(NULL)
{
    *this = p;
}

template <class T>
inline SmartPtr<T>::SmartPtr(const SmartPtr<T> & p)
: ptr(NULL)
{
    *this = p;
}

template <class T>
inline SmartPtr<T>::~SmartPtr()
{
    Release();
}


template <class T>
inline T * SmartPtr<T>::c_ptr() const
{
    return ptr;
}

template <class T>
bool SmartPtr<T>::NotNull() const
{
    return ptr != NULL;
}

template <class T>
inline bool SmartPtr<T>::IsNull() const
{
    return ptr == NULL;
}

template <class T>
inline void SmartPtr<T>::SetNull()
{
    Release();
    ptr = NULL;
}

template <class T>
inline SmartPtr<T> & SmartPtr<T>::operator =(T * p)
{
    Release();

    ptr = p;
    if (ptr)
        ptr->IncRef();

    return *this;
}

template <class T>
inline SmartPtr<T> & SmartPtr<T>::operator =(const SmartPtr<T> & p)
{
    Release();

    ptr = p.ptr;
    if (ptr)
        ptr->IncRef();

    return *this;
}

template <class T>
inline bool SmartPtr<T>::operator ==(const SmartPtr<T> & p) const
{
    return ptr == p.ptr;
}

template <class T>
inline bool SmartPtr<T>::operator !=(const SmartPtr<T> & p) const
{
    return ptr != p.ptr;
}

template <class T>
bool SmartPtr<T>::operator ==(const T * p) const
{
    return ptr == p;
}

template <class T>
bool SmartPtr<T>::operator !=(const T * p) const
{
    return ptr != p;
}

template <class T>
inline T * SmartPtr<T>::operator ->() const
{
    return ptr;
}

template <class T>
inline void SmartPtr<T>::Release()
{
    if (ptr && ptr->DecRef() == 0)
    {
        ptr->DeleteSelf();
        ptr = NULL;
    }
}










template <class T>
SharedPtr<T>::SharedPtr()
: ptr(NULL)
{
}

template <class T>
SharedPtr<T>::SharedPtr(T * p)
: ptr(NULL)
{
    *this = p;
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr & p)
: ptr(NULL)
{
    *this == p;
}

template <class T>
SharedPtr<T>::~SharedPtr()
{
    Release();
}

template <class T>
void SharedPtr<T>::Release()
{
    safe_delete(ptr);
}

template <class T>
T * SharedPtr<T>::c_ptr() const
{
    return ptr;
}

template <class T>
SharedPtr<T> & SharedPtr<T>::operator =(T * p)
{
    assert (ptr != NULL && ptr != p);

    Release();
    ptr = p;
}

template <class T>
SharedPtr<T> & SharedPtr<T>::operator =(const SharedPtr & p)
{
    assert (ptr != NULL && ptr != p.ptr);

    Release();
    ptr = p.ptr;

	return *this;
}

template <class T>
bool SharedPtr<T>::operator ==(const SharedPtr & p) const
{
    return ptr == p.ptr;
}

template <class T>
bool SharedPtr<T>::operator !=(const SharedPtr & p) const
{
    return ptr != p.ptr;
}

template <class T>
bool SharedPtr<T>::operator ==(const T * p) const
{
    return ptr == p;
}

template <class T>
bool SharedPtr<T>::operator !=(const T * p) const
{
    return ptr != p;
}

template <class T>
T * SharedPtr<T>::operator ->() const
{
    return ptr;
}