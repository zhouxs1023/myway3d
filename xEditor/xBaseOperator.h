#pragma once

#include "xEditor.h"

class X_ENTRY xBaseOp
{
protected:
	static int OP_Command;
	static int OP_Id;

public:
	xBaseOp();
	virtual ~xBaseOp();

	virtual void Process() = 0;
	virtual const char * GetIcon() = 0;

protected:
	void Init(void * data);
	void OnCommand(void * data);

protected:
	tEventListener<xBaseOp> mOnCommand;
	tEventListener<xBaseOp> mOnInit;

	int mOperatorId;
};

#define xImplementOp(classname, op) const int classname::##op = xBaseOp::OP_Id++

class xSelectOp : public xBaseOp
{
public:
	static const int eOp_Select;

	virtual void Process();
	virtual const char * GetIcon() { return "select.jpg"; }
};


class xMoveOp : public xBaseOp
{
public:
	static const int eOp_Move;

	virtual void Process();
	virtual const char * GetIcon() { return "move.jpg"; }
};

class xRotateOp : public xBaseOp
{
public:
	static const int eOp_Rotate;

	virtual void Process();
	virtual const char * GetIcon() { return "rotate.jpg"; }
};

class xScaleOp : public xBaseOp
{
public:
	static const int eOp_Scale;

	virtual void Process();
	virtual const char * GetIcon() { return "scale.jpg"; }
};