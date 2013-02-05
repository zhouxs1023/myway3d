#pragma once

#include "MWMath.h"

namespace Myway {

	class MW_ENTRY ColMesh
	{
		DECLARE_ALLOC();

	public:
		struct STri
		{
			unsigned short i0;
			unsigned short i1;
			unsigned short i2;

			unsigned short mtlId;
		};

	public:
		ColMesh();
		~ColMesh();

		void Alloc(int numVerts, int numTris);
		void Clear();

		Array<Vec3> & GetPositions() { return mPositions; }
		Array<STri> & GetTrangles() { return mTrangles; } 

	protected:
		Array<Vec3> mPositions;
		Array<STri> mTrangles;
	};

}
