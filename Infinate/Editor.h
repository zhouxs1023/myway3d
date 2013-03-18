#pragma once

#include "Shape.h"
#include "xEvent.h"
#include "xScene.h"
#include "xForest.h"
#include "xEnvironment.h"
#include "xRenderSetting.h"
#include "Gizmo.h"
#include "xUndoRedo.h"
#include "xObjBound.h"

#include "ColourPanel.h"
#include "MessageBox.h"

namespace Infinite {

	enum eOperator
	{
		eOP_Unknown,
		eOP_Pick,
		eOP_Move,
		eOP_Rotate,
		eOP_Scale,
		eOP_Terrain,

		MW_ALIGN_ENUM(eOperator)
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
		void Update();

		ShaderLib * GetHelperShaderLib() { return mHelperShaderLib; }

		void SetSelectedShape(Shape * obj);
		void SetSelectedShapes(Shape ** objs, int size);
		int GetSelectedShapeSize();
		Shape * GetSelectedShape(int index);

		Vec3 GetHitPosition(float fx, float fy);

		void SetOperator(eOperator op);
		eOperator GetOperator() { return mOperator; }

		void _SetMousePosition(const Point2f & pt) { mMousePosition = pt; }
		Point2f GetMousePosition() { return mMousePosition; }

		void SetFoucs(bool b) { mFoucs = b; }
		bool IsFoucs() { return mFoucs; }

		int MessageBox(const char * text, const char * caption, UINT type)
		{
			const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

			return ::MessageBox(dp->hWnd, text, caption, type);
		}

	protected:
		void _unloadScene(Event * sender);

	protected:
		tEventListener<Editor> OnUnloadScene;

		ShaderLib * mHelperShaderLib;

		Array<Shape*> mSelectedShapes;
		ShapeManager mShapeMgr;

		xScene mScene;
		xForest mForest;
		xEnvironment mEnvironment;
		xRenderSetting mRenderSetting;

		ColourPanel * mColorPanel;
		MMessageBox * mMessageBox;

		eOperator mOperator;

		Gizmo mGizmo;
		xUndoRedoManager mUndoRedoManager;
		xObjBound mObjBound;

		Point2f mMousePosition;
		bool mFoucs;
	};

}