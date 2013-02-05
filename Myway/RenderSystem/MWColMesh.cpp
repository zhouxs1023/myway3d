#include "MWColMesh.h"

namespace Myway {

	ColMesh::ColMesh()
	{
	}

	ColMesh::~ColMesh()
	{
	}

	void ColMesh::Alloc(int numVerts, int numTris)
	{
		mPositions.Resize(numVerts);
		mTrangles.Resize(numTris);
	}

	void ColMesh::Clear()
	{
		mPositions.Clear();
		mTrangles.Clear();
	}
}