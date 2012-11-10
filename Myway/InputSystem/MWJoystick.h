#pragma once

#include "MWMath.h"
#include "MWKeyCode.h"

namespace Myway
{

class MW_ENTRY IJoystick
{
    DECLARE_ALLOC();
	DECLARE_SINGLETON(IJoystick);

public:
	IJoystick();
	virtual ~IJoystick();

	virtual bool KeyUp(JoystickKeyCode key) = 0;
	virtual bool KeyDown(JoystickKeyCode key) = 0;

	virtual bool KeyPressed(JoystickKeyCode key) = 0;

	virtual bool IsActive() = 0;
};


}