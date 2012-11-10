

template <class T>
inline BitFlag<T>::BitFlag()
{
    Clear();
}

template <class T>
inline BitFlag<T>::BitFlag(T _Flags)
{
    Flags = _Flags;
}

template <class T>
inline BitFlag<T>::BitFlag(const BitFlag & _Flags)
{
    Flags = _Flags.Flags;
}

template <class T>
inline BitFlag<T>::~BitFlag()
{
}

template <class T>
inline BitFlag<T> & BitFlag<T>::operator =(const BitFlag<T> & Right)
{
    Flags = Right.Flags;
    return *this;
}

template <class T>
inline BitFlag<T> & BitFlag<T>::operator =(T Right)
{
    Flags = Right;
}

template <class T>
inline BitFlag<T>::operator T() const
{
    return Flags;
}

template <class T>
inline bool BitFlag<T>::operator ==(const BitFlag<T> & Right) const
{
    return Flags == Right.Flags;
}

template <class T>
inline bool BitFlag<T>::operator !=(const BitFlag<T> & Right) const
{
    return Flags != Right.Flags;
}

template <class T>
inline bool BitFlag<T>::operator ==(T Right) const
{
    return Flags == Right;
}

template <class T>
inline bool BitFlag<T>::operator !=(T Right) const
{
    return Flags != Right;
}

template <class T>
inline void BitFlag<T>::Clear()
{
    Flags = static_cast<T>(0);
}

template <class T>
inline void BitFlag<T>::SetFlags(T _Flags)
{
    Flags |= _Flags;
}

template <class T>
inline void BitFlag<T>::ClearFlags(T _Flags)
{
    Flags &= ~_Flags;
}

template <class T>
inline void BitFlag<T>::SetBit(int Bit)
{
    Flags |= (1 << Bit);
}

template <class T>
inline void BitFlag<T>::SetBit(int Bit, bool bSetting)
{
    if (bSetting)
    {
        Flags |= (1 << Bit);
    }
    else
    {
        Flags &= ~(1 << Bit);
    }
}

template <class T>
inline void BitFlag<T>::ClearBit(int Bit)
{
    Flags &= ~(1 << Bit);
}

template <class T>
inline bool BitFlag<T>::Empty() const
{
    return Flags == static_cast<T>(0);
}

template <class T>
inline bool BitFlag<T>::TestBit(int Bit) const
{
    return Flags & (1 << Bit);
}

template <class T>
inline bool BitFlag<T>::TestFlags(T _Flags) const
{
    return (Flags & _Flags) == _Flags;
}

template <class T>
inline bool BitFlag<T>::TestAny(T _Flags) const
{
    return (Flags & _Flags) != 0;
}

template <class T>
inline int BitFlag<T>::BitCount() const
{
    return sizeof(T) << 3;
}

template <class T>
inline int BitFlag<T>::SetCount() const
{
    int count = 0;
    T temp = Flags;
    int total = BitCount();

    for (int i = 0; i < total; i++)
    {
        count += temp & 1;
        temp >>= 1;
    }

    return count;
}