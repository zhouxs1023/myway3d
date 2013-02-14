#pragma once

#include "MWDebug.h"

namespace Myway
{

template <class T>
struct __sort_less
{
    bool operator ()(const T & a, const T & b) const
    {
        return a < b;
    };
};

template <class T>
struct __sort_alloc
{
    T * alloc(int size) const
    {
        return new T[size];
    }

    void free(T * m, int size) const
    {
        delete[] m;
    }
};


/*
-------------------------------------------------------------
    Bubble Sort
-------------------------------------------------------------
*/
template <class T>
void BubbleSort(T * arr, int count)
{
    BubbleSort(arr, count, __sort_less<T>());
}

template <class T, class Func>
void BubbleSort(T * arr, int count, const Func & func)
{
    assert (arr);

    int i, j, p;

    for (i = 0; i < count - 1; ++i)
    {
        p = i;
        for (j = i + 1; j < count; ++j)
        {
            if (func(arr[j], arr[p]))
                p = j;
        }

        T a = arr[i];
        arr[i] = arr[p];
        arr[p] = a;
    }
}



/*
-------------------------------------------------------------
    Insert Sort
-------------------------------------------------------------
*/
template <class T>
void InsertSort(T * arr, int count)
{
    InsertSort(arr, count, __sort_less<T>());
}

template <class T, class Func>
void InsertSort(T * arr, int count, const Func & func)
{
    assert(arr);

    int i, j;

    for(i = 1; i < count; ++i)
    {
        T tmp = arr[i];
        
        for (j = i; j > 0 && func(tmp, arr[j -  1]); --j)
        {
            arr[j] = arr[j - 1];
        }

        arr[j] = tmp;
    }
}



/*
-----------------------------------------------------------
    Shell Sort
-----------------------------------------------------------
*/
template <class T>
void ShellSort(T * arr, int count)
{
    ShellSort(arr, count, __sort_less<T>());
}

template <class T, class Func>
void ShellSort(T * arr, int count, const Func & func)
{
    assert (arr);

    int h = 1;

    while ((h * 3 + 1) < count) 
    {
        h = 3 * h + 1;
    }

    while( h > 0 )
    {
        int i, j;

        for (i = h - 1; i < count; i++) 
        {
            T b = arr[i];
            j = i;

            for(j = i; (j >= h) && func(b, arr[j-h]); j -= h) 
            {
                arr[j] = arr[j-h];
            }

            arr[j] = b;
        }

        h = h / 3;
    }
}


/*
-----------------------------------------------------------
    Merge Sort
-----------------------------------------------------------
*/
template <class T, class Func, class Alloc>
void __Merge(T * arr, int l, int m, int r, const Func & func, const Alloc & alloc)
{
    int beg1 = l;
    int end1 = m;
    int beg2 = m + 1;
    int end2 = r;

    T * tmp = alloc.alloc(r - l + 1);

    int k = 0;
    while (beg1 <= end1 && beg2 <= end2)
    {
        if (func(arr[beg1], arr[beg2]))
        {
            tmp[k++] = arr[beg1++];
        }
        else
        {
            tmp[k++] = arr[beg2++];
        }
    }

    while (beg1 <= end1)
        tmp[k++] = arr[beg1++];

    while (beg2 <= end2)
        tmp[k++] = arr[beg2++];

    for (int i = 0; i < (r - l + 1); ++i)
    {
        arr[r + i] = tmp[i]; 
    }

    alloc.free(tmp, r - l + 1);
}


template <class T, class Func, class Alloc>
void __MergeSort(T * arr, int left, int right, const Func & func, const Alloc & alloc)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        __MergeSort(arr, left, mid);
        __MergeSort(arr, mid + 1, right);
        __Merge(arr, left, mid, right);
    }
}

template <class T, class Func, class Alloc>
void MergeSort(T * arr, int count, const Func & func, const Alloc & alloc)
{
    __MergeSort(arr, 0, count - 1, func, alloc);
}

template <class T>
void MergeSort(T * arr, int count)
{
    __MergeSort(arr, 0, count - 1, __sort_less<T>(), __sort_alloc<T>());
}


/*
-----------------------------------------------------------
    Quick Sort
-----------------------------------------------------------
*/
template <class T, class Func>
int __QPartitions(T * a, int low, int high, const Func & func)
{
    T pivotkey = a[low];

    while (low < high)
    {
        while (low < high && !func(a[high], pivotkey))
        {
            --high;
        }

        a[low] = a[high];

        while (low < high && func(a[low], pivotkey))
        {
            ++low;
        }

        a[high] = a[low];
    }

    a[low] = pivotkey;
    return low;
}

template <class T, class Func>
void __QSort(T * a, int low, int high, const Func & func)
{
    int pivottag = __QPartitions(a, low, high, func);

    if (low < pivottag - 1)
        __QSort(a, low , pivottag - 1, func);

    if (pivottag + 1 < high)
        __QSort(a, pivottag + 1, high, func); 
}


template <class T>
void QuickSort(T * arr, int count)
{
    QuickSort(arr, count, __sort_less<T>());
}


template <class T, class Func>
void QuickSort(T * arr, int count, const Func & func)
{
    assert (arr);

    __QSort(arr, 0, count - 1, func);
}




/*
-----------------------------------------------------------
    Sort:
        Implement QuickSort and InsertSort
-----------------------------------------------------------
*/
#define __sort_factor 20

template <class T, class Func>
void __Sort(T * a, int low, int high, const Func & func)
{
    int count = high - low + 1;

    if (count < __sort_factor)
        InsertSort(a + low, count, func);
    else
    {
        int pivottag = __QPartitions(a, low, high, func);

        if (low < pivottag - 1)
            __Sort(a, low , pivottag - 1, func);

        if (pivottag + 1 < high)
            __Sort(a, pivottag + 1, high, func); 
    }
}

template <class T>
void Sort(T * arr, int count)
{
    Sort(arr, count, __sort_less<T>());
}

template <class T, class Func>
void Sort(T * arr, int count, const Func & func)
{
    assert (arr);

    __Sort(arr, 0, count - 1, func);
}

}