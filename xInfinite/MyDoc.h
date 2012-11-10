// WorldBuildDoc.h : MyDoc 类的接口
//


#pragma once


class MyDoc : public CDocument
{
    DECLARE_DYNCREATE(MyDoc);
    DECLARE_MESSAGE_MAP()

public:
	MyDoc();
    virtual ~MyDoc();

public:
	virtual void Serialize(CArchive& ar);
};


