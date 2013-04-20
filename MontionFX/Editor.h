//////////////////////////////////////////////////////////////////////////
//
// MotionFx: Motion Tool For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "MotionFX.h"
#include "BackCell.h"

namespace MotionFX {

	class MOTIONFX_ENTRY Editor
	{
		DECLARE_SINGLETON(Editor);

	public:
		Editor();
		~Editor();

		void Init();
		void Shutdown();
		void Update();

		ShaderLib * GetHelperShaderLib() { return mHelperShaderLib; }

		void _SetMousePosition(const Point2f & pt) { mMousePosition = pt; }
		Point2f GetMousePosition() { return mMousePosition; }

		void SetFoucs(bool b) { mFoucs = b; }
		bool IsFoucs() { return mFoucs; }

		Actor * GetActor() { return mActor; }
		SceneNode * GetSceneNode() { return mSceneNode; }

	protected:
		ShaderLib * mHelperShaderLib;
		Point2f mMousePosition;
		bool mFoucs;

		Actor * mActor;
		SceneNode * mSceneNode;

		BackCell mBackCell;
	};

}