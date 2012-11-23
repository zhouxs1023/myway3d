#include "stdafx.h"

#include "xBaseOperator.h"
#include "xToolBar.h"

xOpToolBar gOpToolBar;

int xBaseOp::OP_Command = 2000;
int xBaseOp::OP_Id = 2012;
Event xBaseOp::OnChecked;

xBaseOp::xBaseOp()
	: mOnCommand(xBaseOp::OnChecked, this, &xBaseOp::OnCommand)
	, mOperatorId(OP_Command++)
	, mOnInit(xApp::OnInit, this, &xBaseOp::Init)
{
}

xBaseOp::~xBaseOp()
{
}

void xBaseOp::Init(void * data)
{
	xOpToolBar::Instance()->Add(GetIcon(), mOperatorId);
}

void xBaseOp::OnCommand(void * data)
{
	int id = *(int *)data;

	if (id == mOperatorId)
		Process();
}

xImplementOp(xSelectOp, eOp_Select);
xImplementOp(xMoveOp, eOp_Move);
xImplementOp(xRotateOp, eOp_Rotate);
xImplementOp(xScaleOp, eOp_Scale);

xSelectOp gSelectOp;
xMoveOp gMoveOp;
xRotateOp gRotateOp;
xScaleOp gScaleOp;

