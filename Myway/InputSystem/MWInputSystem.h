#pragma once

#include "MWMath.h"
#include "MWKeyCode.h"
#include "MWKeyboard.h"
#include "MWMouse.h"
#include "MWJoystick.h"

namespace Myway
{

class MW_ENTRY InputSystem : public AllocObj
{
	DECLARE_SINGLETON(InputSystem);

public:
	InputSystem();
	virtual ~InputSystem();

	virtual void Update() = 0;
};

}
