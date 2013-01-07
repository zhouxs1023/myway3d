#pragma once

#include "xEditor.h"

class X_ENTRY IDockPane : public CDockablePane
{
public:
	IDockPane(const TString128 & name);
	virtual ~IDockPane() {}

	const TString128 & GetName() const { return mName; }

protected:
	TString128 mName;
};