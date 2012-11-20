#include "stdafx.h"

#include "xBaseOperator.h"
#include "xToolBar.h"

UINT xBaseOp::OP_Command = 2000;
int xBaseOp::OP_Id = AFX_IDW_CONTROLBAR_FIRST + 40;

xBaseOp::xBaseOp()
	: mOnCommand(xToolBar::OnCommand, this, &xBaseOp::OnCommand)
	, mOperatorId(OP_Command++)
	, mOnInit(xApp::OnInit, this, &xBaseOp::Init)
{
}

xBaseOp::~xBaseOp()
{
}

void xBaseOp::Init(void * data)
{
	xToolBar::Instance()->Add(GetIcon(), mOperatorId);
}

void xBaseOp::OnCommand(void * data)
{
	UINT id = *(UINT *)data;

	if (id == mOperatorId)
		Process();
}

const int xSelectOp::eOp_Select = xBaseOp::OP_Id++;
const int xMoveOp::eOp_Move = xBaseOp::OP_Id++;
const int xRotateOp::eOp_Rotate = xBaseOp::OP_Id++;
const int xScaleOp::eOp_Scale = xBaseOp::OP_Id++;

xSelectOp gSelectOp;
xMoveOp gMoveOp;
xRotateOp gRotateOp;
xScaleOp gScaleOp;