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

#include "Shape.h"

namespace Infinite {

	class Gizmo
	{
		DECLARE_SINGLETON(Gizmo);

	public:
		Gizmo();
		~Gizmo();

		void Init(Event * sender);
		void Shutdown(Event * sender);
		void Render(Event * sender);
		void Update(float mouseX, float mouseY);

		bool IsPicked() { return mPicked; }

	protected:
		void _initGeo_Move();
		void _initGeo_Move_Render();
		void _initGeo_Rotate();
		void _initGeo_Rotate_Render();
		void _initGeo_Scale();
		void _initGeo_Scale_Render();

		void _renderMove();
		void _renderRotate();
		void _renderScale();

		void _update_Move();
		void _mouseMoved_Move();

		void _update_Rotate();
		void _mouseMoved_Rotate();

		void _update_Scale();
		void _mouseMoved_Scale();

		float _getObjSize(Shape * obj);

	protected:
		Technique * mTech;

		RenderOp * mRender_Move;
		int mNumVertex_Move;
		int mNumIndex_Move;
		Vec3 * mVertex_Move;
		short * mIndex_Move;

		RenderOp * mRender_Rotate;
		int mNumVertex_Rotate;
		int mNumIndex_Rotate;
		Vec3 * mVertex_Rotate;
		short * mIndex_Rotate;

		RenderOp * mRender_Scale;
		int mNumVertex_Scale;
		int mNumIndex_Scale;
		Vec3 * mVertex_Scale;
		short * mIndex_Scale;

		int mPickedAxis;
		bool mPicked;

		Vec3 mOldPosition;
		Quat mOldOrientation;
		float mOldScale;

		float mMouseX;
		float mMouseY;

		tEventListener<Gizmo> OnInit;
		tEventListener<Gizmo> OnShutdown;
		tEventListener<Gizmo> OnRender;
	};

}