// MyGui Integration.
//   MyGui engine: http://mygui.info
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "GameEntity.h"

class GCORE_ENTRY MGUI_Util
{
	DECLARE_SINGLETON(MGUI_Util);

public:
	static const int MAX_TEXT_SIZE = 64;
	static const int TEXT_VB_SIZE = 4096;
	static const int TEXT_IB_SIZE = 64 * 6;

public:
	struct Text3D {

		wchar_t wstr[MAX_TEXT_SIZE];
		MyGUI::IFont * font;
		Vec3 position;
		Color4 color;
		float charHeight;

		Text3D() {
			memset(this, 0, sizeof(Text3D));
		}
	};

	struct TextVertex {
		Vec3 position;
		Color4 color;
		float u, v;
	};

	struct TextPrimitive
	{
		MyGUI::IFont * font;
		int startVertex;
		int primCount;
	};

public:
	MGUI_Util();
	~MGUI_Util();

	void Init();
	void Shutdown();

	void RenderText3D(const char * str, const char * fontName, const Vec3 & position, float charHeight, const Color4 & color);

protected:
	void _PrepareForRender();
	void _RenderText3DImp();

protected:
	bool IsVisible(const Vec3 & position, const Size2F & size);

	// Listen Event
protected:
	void OnPostUpdateScene_(Event * sender);
	void OnAfterDefferedShading_(Event * sender);

protected:
	tEventListener<MGUI_Util> EvPostUpdateScene;
	tEventListener<MGUI_Util> EvAfterDefferedShading;

	Array<Text3D> mText3DArray;
	Array<Text3D*> mText3DSortedArray; 
	Array<TextPrimitive> mText3DPrimtive;

	VertexBufferPtr mText3DVB;
	IndexBufferPtr mText3DIB;
	RenderOp * mRenderOp;
	Technique * mTech;
};

