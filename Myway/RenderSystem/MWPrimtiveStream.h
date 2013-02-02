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

    void Bind(int number, VertexBufferPtr stream, int stride, int instance = 0);

    VertexBufferPtr GetStream(int index) const;
    int GetStreamStride(int index) const;
	int GetStreamInstance(int index) const;

	void SetStart(int index) { mStart = index; }
	int GetStart() const { return mStart; }

    void SetCount(int size);
    int GetCount() const;

    VertexStream(const VertexStream & r);
    VertexStream & operator =(const VertexStream & r);

public:
	int mStart;
    int mCount;
    VertexDeclarationPtr mDeclaration;
    int mStrides[MAX_VERTEX_STREAM];
	int mInstances[MAX_VERTEX_STREAM];
    VertexBufferPtr mStreams[MAX_VERTEX_STREAM];
};


class MW_ENTRY IndexStream : public AllocObj
{
public:
                    IndexStream();
                    ~IndexStream();

    void            Bind(IndexBufferPtr stream, int start = 0);
    IndexBufferPtr  GetStream() const;

	void			SetStart(int start)  { mStart = start; }
	int				GetStart() const { return mStart; }

    void            SetCount(int size);
    int             GetCount() const;

                    IndexStream(const IndexStream & r);

    IndexStream &   operator =(const IndexStream & r);

protected:
	int				mStart;
    int             mCount;
    IndexBufferPtr  mStream;
};

}
