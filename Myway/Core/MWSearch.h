#pragma once

#include "MWDebug.h"

namespace Myway
{

/*
-------------------------------------------------------------
    Search Compare Function
-------------------------------------------------------------
*/

template <class _L, class _R>
struct __search_less
{
    bool operator ()(const _L & a, const _R & b) const
    {
        return a < b;
    };
};

template <class _L, class _R>
struct __search_equal
{
    bool operator ()(const _L & a, const _R & b) const
    {
        return a == b;
    };
};


/*
-------------------------------------------------------------
    Search Uniform
-------------------------------------------------------------
*/

template <class T, class K>
int Search(const T * a, const K & k, int count)
{
    return Search(a, k, count, __search_equal<T, K>());
}

template <class T, class K, class Equal>
int Search(const T * a, const K & k, int count, const Equal & eq)
{
    assert (a);

    int i = 0;

    while (i < count && !eq(a[i], k))
    {
        ++i;
    }

    return i < count ? i : NPOS;
}


/*
-------------------------------------------------------------
    Bisearch
-------------------------------------------------------------
*/

template <class T, class K>
int Bisearch(const T * a, const K & k, int count)
{
    return Bisearch(a, k, count, __search_equal<T, K>(), __search_less<T, K>());
}

template <class T, class K, class Equal, class Less>
int Bisearch(const T * a, const K & k, int count, const Equal & eq, const Less & less)
{
    assert (count > INT_MAX && a);

    int start = 0, end = (int)count - 1;
    int mid;

    while (start <= end)
    {
        mid = (start + end) / 2;

        if (eq(a[mid], k))
            return mid;

        else if (less(k, a[mid]))
            end = mid - 1;
        else
            start = mid + 1;
    }

    return NPOS;
}



template <class T, class K>
int BisearchLower(const T * a, const K & k, int count)
{
    return BisearchLower(a, k, count, __search_less<T, K>());
}

template <class T, class K, class Less>
int BisearchLower(const T * a, const K & k, int count, const Less & less)
{
    assert (count < INT_MAX && a);

    int start = 0, end = (int)count - 1;
    int pos1 = NPOS, pos2 = NPOS, pos = NPOS;
    int mid;

    while (start <= end)
    {
        mid = (start + end) / 2;

        if (less(k, a[mid]))
        {
            end = mid - 1;
            pos1 = mid;
        }
        else
        {
            start = mid + 1;
            pos2 = mid;
        }
    }

    if (pos1 == NPOS)
    {
        pos1 = pos2;
    }

    pos = pos1;

    if (pos2 != NPOS && pos1 != pos2)
    {
        if (less(a[pos2], k))
        {
            pos = pos1;
        }
        else
        {
            pos = pos2;
        }
    }

    if (pos != NPOS && less(a[pos], k))
        pos = NPOS;

    return pos;
}

template <class T, class V>
T Find_t(T beg, T end, V v)
{
    while (beg != end && *beg != v)
    {
        ++beg;
    }

    return beg;
}

}
