#pragma once

#include "MWDIInputSystemEntry.h"
#include "MWMouse.h"

namespace Myway
{

#define MOUSE_KEY_SIZES 8

class DI_ENTRY DIMouse : public IMouse
{
public:
	DIMouse(LPDIRECTINPUT8 input, HWND hwnd);
	virtual ~DIMouse();

	virtual bool KeyUp(MouseKeyCode key);
	virtual bool KeyDown(MouseKeyCode key);
	virtual bool KeyPressed(MouseKeyCode key);

    virtual bool MouseMoved();
	virtual int MouseWheel();

	virtual Point2i GetPosition();
	virtual Point2i GetPositionGlobal();
	virtual Point2f GetPositionUnit();

    virtual Point2i GetOldPosition();
    virtual Point2i GetOldPositionGlobal();
    virtual Point2f GetOldPositionUnit();

	virtual Point2i GetPositionDiff();
    virtual Point2f GetPositionDiffUnit();

	virtual bool IsActive();

	void Update();

protected:
	LPDIRECTINPUTDEVICE8 m_device;

	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_oldMouseState;
	HWND m_hWnd;

    Point2i m_oldPositionLocal;
    Point2i m_oldPositionGlobal;
    Point2f m_oldPositionUnit;

	Point2i m_positionLocal;
	Point2i m_positionGlobal;
	Point2f m_positionUnit;

	bool m_bActive;
};

}