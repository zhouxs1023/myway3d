#pragma once

//#ifdef INFI_EXPORT
//
//#define INFI_ENTRY __declspec(dllexport)
//
//#else
//
//#define INFI_ENTRY __declspec(dllimport)
//
//#endif

#define INFI_ENTRY



enum eTransformOperator
{
	eTO_Unknown,
	eTO_Move,
	eTO_Rotate,
	eTO_Scale,
};

enum ePick
{
	PICK_Flag = 0x01,
};

enum eAppMode
{
	eAM_Editor,
	eAM_Game,
};