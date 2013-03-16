#pragma once

#include "xUndoRedo.h"

namespace Infinite {

	class xUndoRedo_Move : public xUndoRedo
	{
	public: 
		xUndoRedo_Move(const TString128 & objName, const Vec3 & orgPosition, const Vec3 & newPosition);
		virtual ~xUndoRedo_Move();

		virtual void Undo();
		virtual void Redo();

	protected:
		TString128 mObjName;
		Vec3 mOrgPosition;
		Vec3 mNewPosition;
	};

	class xUndoRedo_Rotate : public xUndoRedo
	{
	public: 
		xUndoRedo_Rotate(const TString128 & objName, const Quat & orgOrientation, const Quat & newOrientation);
		virtual ~xUndoRedo_Rotate();

		virtual void Undo();
		virtual void Redo();

	protected:
		TString128 mObjName;
		Quat mOrgOrientation;
		Quat mNewOrientation;
	};

	class xUndoRedo_Scale : public xUndoRedo
	{
	public:
		xUndoRedo_Scale(const TString128 & objName, float orgScale, float newScale);
		virtual ~xUndoRedo_Scale();

		virtual void Undo();
		virtual void Redo();

	protected:
		TString128 mObjName;
		float mOrgScale;
		float mNewScale;
	};

}
