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