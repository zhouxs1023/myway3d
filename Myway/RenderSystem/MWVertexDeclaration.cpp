#include "MWVertexDeclaration.h"

namespace Myway {

int VertexDeclaration::GetDeclTypeSize(DECL_TYPE type)
{
    switch (type)
    {
    case DT_FLOAT1: return 4;
    case DT_FLOAT2: return 8;
    case DT_FLOAT3: return 12;
    case DT_FLOAT4: return 16;
    case DT_COLOR:  return 4;
    case DT_UBYTE4: return 4;
    case DT_SHORT2: return 4;
    case DT_SHORT4: return 8;
    }

    d_assert (0);

    return 0;
}

VertexDeclaration::VertexDeclaration()
{
    mElementCount = 0;
}

VertexDeclaration::~VertexDeclaration()
{
}

void VertexDeclaration::AddElement(short StreamNumber, short Offset,
    DECL_TYPE DeclType, DECL_USAGE DeclUsage, unsigned char UsageIndex)
{
    d_assert(StreamNumber < MAX_ELEMENT);

    int index = mElementCount;
    mElementCount++;

    mElements[index].Stream = StreamNumber;
    mElements[index].Offset = Offset;
    mElements[index].Type = DeclType;
    mElements[index].Usage = DeclUsage;
    mElements[index].UsageIndex = UsageIndex;
}

int VertexDeclaration::GetStreamSize(short number)
{
    int size = 0;
    for (int i = 0; i < mElementCount; ++i)
    {
        if (mElements[i].Stream == number)
            size += GetDeclTypeSize(mElements[i].Type); 
    }

    return size;
}

void VertexDeclaration::GetStreamItem(short number, VertexElement * ve, int & count)
{
    count = 0;

    for (int i = 0; i < mElementCount; ++i)
    {
        if (mElements[i].Stream == number)
        {
            ve[count] = mElements[i];
            ++count;
        }
    }
}

void VertexDeclaration::GetAllStreamItem(VertexElement * ve, int & count)
{
    count = 0;

    for (int i = 0; i < mElementCount; ++i)
    {
        ve[count] = mElements[i];

        ++count;
    }
}

void VertexDeclaration::GetElement(VertexElement & ve)
{
    bool flag = FALSE;

    for (int i = 0; i < mElementCount; ++i)
    {
        VertexElement _ve;
        _ve = mElements[i];

        if ((ve.Stream == DECL_UNUSED || ve.Stream == _ve.Stream) &&
            (ve.Usage == _ve.Usage) || (ve.Type == _ve.Type) &&
            (ve.UsageIndex == _ve.UsageIndex))
        {
            ve = _ve;
            flag = TRUE;
            break;
        }
    }

    if (!flag)
        ve.Stream = DECL_UNUSED;
}

const VertexElement * VertexDeclaration::GetElementByUsage(DECL_USAGE usage)
{
    for (int i = 0; i < mElementCount; ++i)
    {
        const VertexElement & ve = mElements[i];

        if (ve.Usage == usage)
        {
            return &ve;
        }
    }

    return NULL;
}


}
