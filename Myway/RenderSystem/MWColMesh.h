#pragma once

#include "MWMath.h"

namespace Myway {

	enum MColType
	{
		CT_Unknown,
		CT_Mesh,
		CT_Terrain,
	};

	class MW_ENTRY ColMesh
	{
		DECLARE_ALLOC();

	public:
		ColMesh();
		~ColMesh();

		void Alloc(int numVerts, int numTris);
		void Clear();

		Array<Vec3> & GetPositions() { return mPositions; }
		Array<int> & GetIndices() { return mIndices; } 

	protected:
		Array<Vec3> mPositions;
		Array<int> mIndices;
	};

}
