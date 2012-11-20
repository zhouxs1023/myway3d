#pragma once

#include "xApp.h"


class xBaseOp
{
protected:
	static UINT OP_Command;
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

	UINT mOperatorId;
};

class xSelectOp : public xBaseOp
{
public:
	static const int eOp_Select;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_Select);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\move.ico";
	}
};


class xMoveOp : public xBaseOp
{
public:
	static const int eOp_Move;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_Move);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\move.ico";
	}
};

class xRotateOp : public xBaseOp
{
public:
	static const int eOp_Rotate;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_Rotate);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\rotate.ico";
	}
};

class xScaleOp : public xBaseOp
{
public:
	static const int eOp_Scale;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_Scale);
	}

	virtual const char * GetIcon()
	{
		return "..\\ui\\scale.ico";
	}
};