#pragma once

#include "MWKeyCode.h"
#include "MWMath.h"

namespace Myway
{

class MW_ENTRY IMouse : public AllocObj
{
	DECLARE_SINGLETON(IMouse)

public:
	IMouse();
	virtual ~IMouse();

	virtual bool KeyUp(MouseKeyCode key) = 0;
	virtual bool KeyDown(MouseKeyCode key) = 0;

	virtual bool KeyPressed(MouseKeyCode key) = 0;

    virtual bool MouseMoved() = 0;
	virtual int MouseWheel() = 0;

	virtual Point2i GetPosition() = 0;
	virtual Point2i GetPositionGlobal() = 0;
	virtual Point2f GetPositionUnit() = 0;

    virtual Point2i GetPositionDiff() = 0;
	virtual Point2f GetPositionDiffUnit() = 0;

	virtual bool IsActive() = 0;
};

}