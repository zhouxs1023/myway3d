#pragma once

#include "MWVideoBufferManager.h"

namespace Myway
{

class MW_ENTRY VertexStream : public AllocObj
{
public:
    VertexStream();
    ~VertexStream();

    void SetDeclaration(VertexDeclarationPtr decl);
    VertexDeclarationPtr GetDeclaration() const;

    void Bind(int number, VertexBufferPtr stream, int stride);

    VertexBufferPtr GetStream(int index) const;
    int GetStreamStride(int index) const;

    void SetCount(int size);
    int GetCount() const;

    VertexStream(const VertexStream & r);
    VertexStream & operator =(const VertexStream & r);

public:
    int mCount;
    VertexDeclarationPtr mDeclaration;
    int mStrides[MAX_VERTEX_STREAM];
    VertexBufferPtr mStreams[MAX_VERTEX_STREAM];
};


class MW_ENTRY IndexStream : public AllocObj
{
public:
                    IndexStream();
                    ~IndexStream();

    void            Bind(IndexBufferPtr stream, int startVertex);
    IndexBufferPtr  GetStream() const;
    int             GetStartVertex() const;

    void            SetCount(int size);
    int             GetCount() const;

                    IndexStream(const IndexStream & r);

    IndexStream &   operator =(const IndexStream & r);

protected:
    int             mCount;
    int             mStart;
    IndexBufferPtr  mStream;
};

}
