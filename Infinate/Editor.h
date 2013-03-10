#pragma once

#include "Shape.h"
#include "xEvent.h"
#include "xScene.h"
#include "xForest.h"
#include "xEnvironment.h"

namespace Infinite {

	enum eTransformOperator
	{
		eTO_Unknown,
		eTO_Move,
		eTO_Rotate,
		eTO_Scale,
	};

	enum ePick
	{
		PICK_Flag = 0x01,
	};

	class INFI_ENTRY Editor
	{
		DECLARE_SINGLETON(Editor);

	public:
		Editor();
		~Editor();

		void Init();
		void Shutdown();

		ShaderLib * GetHelperShaderLib() { return mHelperShaderLib; }

		void SetSelectedShape(Shape * obj);
		void SetSelectedShapes(Shape ** objs, int size);
		int GetSelectedShapeSize();
		Shape * GetSelectedShape(int index);

		Vec3 GetHitPosition(float fx, float fy);

	protected:
		ShaderLib * mHelperShaderLib;

		Array<Shape*> mSelectedShapes;
		ShapeManager mShapeMgr;

		xScene mScene;
		xForest mForest;
		xEnvironment mEnvironment;
	};

}