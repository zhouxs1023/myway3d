#include "MWPrimtiveStream.h"

using namespace Myway;

VertexStream::VertexStream()
{
	Memzero(mStrides, sizeof(int) * MAX_VERTEX_STREAM);
    Memzero(mInstances, sizeof(int) * MAX_VERTEX_STREAM);
}

VertexStream::~VertexStream()
{
}

void VertexStream::SetDeclaration(VertexDeclarationPtr decl)
{
    mDeclaration = decl;
}

VertexDeclarationPtr VertexStream::GetDeclaration() const
{
    return mDeclaration;
}

void VertexStream::Bind(int number, VertexBufferPtr stream, int stride, int instance)
{
    assert (number < MAX_VERTEX_STREAM);

    mStreams[number] = stream;
    mStrides[number] = stride;
	mInstances[number] = instance;
}

VertexBufferPtr VertexStream::GetStream(int index) const
{
    return mStreams[index];
}

int VertexStream::GetStreamStride(int index) const
{
    return mStrides[index];
}

int VertexStream::GetStreamInstance(int index) const
{
	return mInstances[index];
}

void VertexStream::SetCount(int size)
{
    mCount = size;
}

int VertexStream::GetCount() const
{
    return mCount;
}

VertexStream::VertexStream(const VertexStream & r)
{
    *this = r;
}

VertexStream & VertexStream::operator =(const VertexStream & r)
{
    mCount = r.GetCount();
    mDeclaration = r.GetDeclaration();

    for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
    {
        mStrides[i] = r.GetStreamStride(i);
        mStreams[i] = r.GetStream(i);
    }

    return *this;
}









IndexStream::IndexStream()
: mStart(0),
  mCount(0)
{
}

IndexStream::~IndexStream()
{
}

void IndexStream::Bind(IndexBufferPtr stream, int startVertex)
{
    mStream = stream;
    mStart = startVertex;
}

IndexBufferPtr IndexStream::GetStream() const
{
    return mStream;
}

int IndexStream::GetStartVertex() const
{
    return mStart;
}

void IndexStream::SetCount(int size)
{
    mCount = size;
}

int IndexStream::GetCount() const
{
    return mCount;
}

IndexStream::IndexStream(const IndexStream & r)
{
    *this = r;
}

IndexStream & IndexStream::operator =(const IndexStream & r)
{
    assert (this != &r);

    mStart = r.GetStartVertex();
    mStream = r.GetStream();

    return *this;
}