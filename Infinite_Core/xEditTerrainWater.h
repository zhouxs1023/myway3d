#pragma once

namespace Infinite {

	class xEditTerrainWater
	{
		struct Brush
		{
			Vec3 position;
			float size;
		};

	public:
		xEditTerrainWater();
		~xEditTerrainWater();

		void SetBrushSize(int size) { mBrushSize = size * 2 + 1; }
		void SetBrushOp(bool bAdd) { mBrushOp_Add = bAdd; }
		void SetHeight(float h) { Environment::Instance()->GetWaterManager()->GetWater()->SetHeight(h); }

		void _Init();
		void _Shutdown();
		void _Update();
		void _Render();

	protected:

		int mBrushSize;
		bool mBrushOp_Add;

		Brush mBrush;

		RenderOp * mRenderOp_Brush;
		Technique * mTech_Brush;

		RenderOp * mRenderOp_WaterHelper;
		Technique * mTech_WaterHelper;
	};

}