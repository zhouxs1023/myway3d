#include "MWDIInputSystem.h"

namespace Myway
{

//DIInputSystem
DIInputSystem::DIInputSystem(HINSTANCE hInst, HWND hWnd)
{
	m_pKeyboard = NULL;
	m_pMouse = NULL;
	m_pJoystick = NULL;

	//create input
	HRESULT hr = S_OK;
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDISystem, NULL);
	if (FAILED(hr))
	{
		d_assert(0 && "Create Input Device Failed.");
	}
	else
	{
		m_pKeyboard = new DIKeyboard(m_pDISystem, hWnd); 
		m_pMouse = new DIMouse(m_pDISystem, hWnd);
		m_pJoystick = new DIJoystick(m_pDISystem, hWnd);
	}
}

DIInputSystem::~DIInputSystem()
{
	//release object
	safe_delete(m_pKeyboard);
	safe_delete(m_pMouse);
	safe_delete(m_pJoystick);

	if (m_pDISystem)
	{
		m_pDISystem->Release();
		m_pDISystem = NULL;
	}
}


void DIInputSystem::Update()
{
	if (m_pMouse) 
		m_pMouse->Update();

	if (m_pKeyboard)
		m_pKeyboard->Update();

	if (m_pJoystick)
		m_pJoystick->Update();
}

}
