//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once


namespace Infinite {

	class xEditTerrainNavPath
	{
		struct Brush
		{
			float size;
			Vec3 position;
		};

	public:
		enum Op {
			eNone,
			eSet,
			eClear,
		};

	public:
		xEditTerrainNavPath();
		~xEditTerrainNavPath();

		void _Init();
		void _Shutdown();
		void _Update();

		void SetBrushSize(float size) { mBrush.size = size; }
		float GetBrushSize() { return mBrush.size; }

		void SetOperator(Op op) { mOp = op; }
		Op GetOperator() { return mOp; }

	protected:
		void _updateGeometry();

	protected:
		Op mOp;
		Brush mBrush;
		Technique * mTech_Brush;

		RenderOp * mRenderOp;
		int mCurXSection, mCurZSection;
	};

}
