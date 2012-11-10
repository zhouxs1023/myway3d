

/* :) VectorX
---------------------------------------------------------------------------------------
    @Remark: 
        vertor template
---------------------------------------------------------------------------------------
*/
template <class T, int num>
inline VectorX<T, num>::VectorX()
{
}

template <class T, int num>
inline VectorX<T, num>::~VectorX()
{
}

template <class T, int num>
inline VectorX<T, num>::VectorX(const T * mem)
{
    for (int i = 0; i < num; ++i)
    {
        m[i] = mem[i];
    }
}

template <class T, int num>
inline VectorX<T, num>::VectorX(const VectorX<T, num> & v)
{
    for (int i = 0; i < num; ++i)
    {
        m[i] = v.m[i];
    }
}

template <class T, int num>
inline VectorX<T, num>& VectorX<T, num>::operator = (const VectorX<T, num> & v)
{
    for (int i = 0; i < num; ++i)
    {
        m[i] = v.m[i];
    }

    return *this;
}

template <class T, int num>
inline VectorX<T, num>::operator T* ()
{
    return m;
}

template <class T, int num>
inline VectorX<T, num>::operator const T* () const
{
    return m;
}

template <class T, int num>
inline bool VectorX<T, num>::operator ==(const VectorX<T, num> & v) const
{
    for (int i = 0; i < num; ++i)
    {
        if (m[i] != v.m[i])
            return false;
    }

    return true;
}

template <class T, int num>
inline bool VectorX<T, num>::operator !=(const VectorX<T, num> & v) const
{
    for (int i = 0; i < num; ++i)
    {
        if (m[i] == v.m[i])
            return false;
    }

    return true;
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator -() const
{
    VectorX<T, num> v;

    for (int i = 0; i < num; ++i)
        v.m[i] = -m[i];

    return v
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator + (const VectorX<T, num> & v) const
{
    VectorX ret;

    for (int i = 0; i < num; ++i)
        ret.m[i] = m[i] + v.m[i];

    return ret;
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator - (const VectorX<T, num> & v) const
{
    VectorX<T, num> ret;

    for (int i = 0; i < num; ++i)
        ret.m[i] = m[i] - v.m[i];

    return ret;
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator * (const VectorX<T, num> & v) const
{
    VectorX<T, num> ret;

    for (int i = 0; i < num; ++i)
        ret.m[i] = m[i] * v.m[i];

    return ret;
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator / (const VectorX<T, num> & v) const
{
    VectorX<T, num> ret;

    for (int i = 0; i < num; ++i)
        ret.m[i] = m[i] / v.m[i];

    return ret;
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator * (T i) const
{
    VectorX<T, num> ret;

    for (int i = 0; i < num; ++i)
        ret.m[i] = m[i] * i;

    return ret;
}

template <class T, int num>
inline VectorX<T, num> VectorX<T, num>::operator / (T i) const
{
    VectorX<T, num> ret;

    for (int i = 0; i < num; ++i)
        ret.m[i] = m[i] / i;

    return ret;
}

template <class T, int num>
inline VectorX<T, num>& VectorX<T, num>::operator += (const VectorX<T, num> & v)
{
    for (int i = 0; i < num; ++i)
        m[i] += v.m[i];

    return *this;
}

template <class T, int num>
inline VectorX<T, num>& VectorX<T, num>::operator -= (const VectorX<T, num> & v)
{
    for (int i = 0; i < num; ++i)
        m[i] -= v.m[i];

    return *this;
}

template <class T, int num>
inline VectorX<T, num>& VectorX<T, num>::operator *= (T i)
{
    for (int i = 0; i < num; ++i)
        m[i] *= v.m[i];

    return *this;
}

template <class T, int num>
inline VectorX<T, num>& VectorX<T, num>::operator /= (T i)
{
    for (int i = 0; i < num; ++i)
        m[i] /= v.m[i];

    return *this;
}

template <class T, int num>
inline T& VectorX<T, num>::operator [](int index)
{
    return m[index];
}

template <class T, int num>
inline const T& VectorX<T, num>::operator [](int index) const
{
    return m[index];
}

template <class T, int num>
inline VectorX<T, num> operator * (T i, const VectorX<T, num> & v)
{
    return v * i;
}

template <class T, int num>
inline VectorX<T, num> operator / (T i, const VectorX<T, num> & v)
{
    return v / i;
}