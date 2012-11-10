#include "stdafx.h"
#include "MyDoc.h"


IMPLEMENT_DYNCREATE(MyDoc, CDocument)

BEGIN_MESSAGE_MAP(MyDoc, CDocument)
END_MESSAGE_MAP()


MyDoc::MyDoc()
{
}

MyDoc::~MyDoc()
{
}

void MyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

