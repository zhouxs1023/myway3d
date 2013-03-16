#include "stdafx.h"

#include "xBaseOperator.h"
#include "Shape.h"

namespace Infinite {

	xUndoRedo_Move::xUndoRedo_Move(const TString128 & objName, const Vec3 & orgPosition, const Vec3 & newPosition)
		: mObjName(objName)
		, mOrgPosition(orgPosition)
		, mNewPosition(newPosition)
	{
	}

	xUndoRedo_Move::~xUndoRedo_Move()
	{
	}

	void xUndoRedo_Move::Undo()
	{
		Shape * obj = ShapeManager::Instance()->Get(mObjName.c_str());

		if (obj)
		{
			obj->SetPosition(mOrgPosition);
		}
	}

	void xUndoRedo_Move::Redo()
	{
		Shape * obj = ShapeManager::Instance()->Get(mObjName.c_str());

		if (obj)
		{
			obj->SetPosition(mNewPosition);
		}
	}



	xUndoRedo_Rotate::xUndoRedo_Rotate(const TString128 & objName, const Quat & orgOrientation, const Quat & newOrientation)
		: mObjName(objName)
		, mOrgOrientation(orgOrientation)
		, mNewOrientation(newOrientation)
	{
	}

	xUndoRedo_Rotate::~xUndoRedo_Rotate()
	{
	}

	void xUndoRedo_Rotate::Undo()
	{
		Shape * obj = ShapeManager::Instance()->Get(mObjName.c_str());

		if (obj)
		{
			obj->SetOrientation(mOrgOrientation);
		}
	}

	void xUndoRedo_Rotate::Redo()
	{
		Shape * obj = ShapeManager::Instance()->Get(mObjName.c_str());

		if (obj)
		{
			obj->SetOrientation(mNewOrientation);
		}
	}




	xUndoRedo_Scale::xUndoRedo_Scale(const TString128 & objName, float orgScale, float newScale)
		: mObjName(objName)
		, mOrgScale(orgScale)
		, mNewScale(newScale)
	{
	}

	xUndoRedo_Scale::~xUndoRedo_Scale()
	{
	}

	void xUndoRedo_Scale::Undo()
	{
		Shape * obj = ShapeManager::Instance()->Get(mObjName.c_str());

		if (obj)
		{
			obj->SetScale(mOrgScale);
		}
	}

	void xUndoRedo_Scale::Redo()
	{
		Shape * obj = ShapeManager::Instance()->Get(mObjName.c_str());

		if (obj)
		{
			obj->SetScale(mNewScale);
		}
	}


}