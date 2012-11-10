#include "MWMouse.h"
#include "MWKeyboard.h"
#include "MWJoystick.h"
#include "MWInputSystem.h"

namespace Myway
{

IMP_SLN(IMouse);
IMouse::IMouse()
{
    INIT_SLN;
}

IMouse::~IMouse()
{
    SHUT_SLN;
}


IMP_SLN(IKeyboard);
IKeyboard::IKeyboard()
{
    INIT_SLN;
}

IKeyboard::~IKeyboard()
{
    SHUT_SLN;
}


IMP_SLN(IJoystick);
IJoystick::IJoystick()
{
    INIT_SLN;
}

IJoystick::~IJoystick()
{
    SHUT_SLN;
}



IMP_SLN(InputSystem);
InputSystem::InputSystem()
{
    INIT_SLN;
}

InputSystem::~InputSystem()
{
    SHUT_SLN;
}

}

