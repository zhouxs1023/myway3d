#pragma once

#include "MWMath.h"
#include "MWKeyCode.h"

namespace Myway
{

class MW_ENTRY IKeyboard
{
    DECLARE_ALLOC();
	DECLARE_SINGLETON(IKeyboard);

public:
	IKeyboard();
	virtual ~IKeyboard();

	virtual bool KeyUp(KeyCode key) = 0;
	virtual bool KeyDown(KeyCode key) = 0;

	virtual bool KeyPressed(KeyCode key) = 0;

	virtual bool IsActive() = 0;
};

}