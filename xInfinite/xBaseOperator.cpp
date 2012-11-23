#include "stdafx.h"

#include "xBaseOperator.h"
#include "xToolBar.h"

int xBaseOp::OP_Command = 2000;
int xBaseOp::OP_Id = 2012;

xBaseOp::xBaseOp()
	: mOnCommand(xOpToolBar::OnChecked, this, &xBaseOp::OnCommand)
	, mOperatorId(OP_Command++)
	, mOnInit(xApp::OnInit, this, &xBaseOp::Init)
{
}

xBaseOp::~xBaseOp()
{
}

void xBaseOp::Init(void * data)
{
	//xOpToolBar::Instance()->Add(GetIcon(), mOperatorId);
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
//xMoveOp gMoveOp;
//xRotateOp gRotateOp;
//xScaleOp gScaleOp;

class test_
{
public:
	test_() : mOnCheck(xOpToolBar::OnChecked1, this, &test_::_OnCheck) {}

	void _OnCheck(void *)
	{
		int i = 0;
	}

protected:
	tEventListener<test_> mOnCheck;
};

test_ gtest;

