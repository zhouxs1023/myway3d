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
		mIndices.Resize(numTris * 3);
	}

	void ColMesh::Clear()
	{
		mPositions.Clear();
		mIndices.Clear();
	}
}