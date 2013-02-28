#include "stdafx.h"

#include "xBaseOperator.h"
#include "xToolBar.h"
#include "xEvent.h"
#include "xApp.h"

int xBaseOp::OP_Command = 2000;
int xBaseOp::OP_Id = 2012;

xBaseOp::xBaseOp()
	: mOnCommand(&xEvent::OnOperatorBarChecked, this, &xBaseOp::OnCommand)
	, mOperatorId(OP_Command++)
	, mOnInit(&xEvent::OnInit, this, &xBaseOp::Init)
{
}

xBaseOp::~xBaseOp()
{
}

void xBaseOp::Init(Event * sender)
{
	xOpToolBar::Instance()->Add(GetIcon(), mOperatorId);
}

void xBaseOp::OnCommand(Event * sender)
{
	int id = *(int *)sender->GetParam(0);

	if (id == mOperatorId)
		Process();
}

xImplementOp(xSelectOp, eOp_Select);

void xSelectOp::Process()
{
	xApp::Instance()->SetOperator(eOp_Select);
}


xImplementOp(xMoveOp, eOp_Move);

void xMoveOp::Process()
{
	xApp::Instance()->SetOperator(eOp_Move);
}



xImplementOp(xRotateOp, eOp_Rotate);

void xRotateOp::Process()
{
	xApp::Instance()->SetOperator(eOp_Rotate);
}




xImplementOp(xScaleOp, eOp_Scale);

void xScaleOp::Process()
{
	xApp::Instance()->SetOperator(eOp_Scale);
}



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
	xObj * obj = xObjManager::Instance()->Get(mObjName.c_str());
	
	if (obj)
	{
		obj->SetPosition(mOrgPosition);
	}
}

void xUndoRedo_Move::Redo()
{
	xObj * obj = xObjManager::Instance()->Get(mObjName.c_str());

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
	xObj * obj = xObjManager::Instance()->Get(mObjName.c_str());

	if (obj)
	{
		obj->SetOrientation(mOrgOrientation);
	}
}

void xUndoRedo_Rotate::Redo()
{
	xObj * obj = xObjManager::Instance()->Get(mObjName.c_str());

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
	xObj * obj = xObjManager::Instance()->Get(mObjName.c_str());

	if (obj)
	{
		obj->SetScale(mOrgScale);
	}
}

void xUndoRedo_Scale::Redo()
{
	xObj * obj = xObjManager::Instance()->Get(mObjName.c_str());

	if (obj)
	{
		obj->SetScale(mNewScale);
	}
}