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

#include "MGUI_Entry.h"


namespace Myway {

	class MGUI_ENTRY MGUI_Util
	{
		DECLARE_SINGLETON(MGUI_Util);

	public:
		static const int MAX_TEXT_SIZE = 256;

	protected:
		struct Text3D {

			wchar_t wstr[MAX_TEXT_SIZE];
			MyGUI::IFont * font;
			Vec3 position;
			Color4 color;
			SizeF size;

			Text3D() {
				memset(this, 0, sizeof(Text3D));
			}
		};

	public:
		MGUI_Util();
		~MGUI_Util();

		void PushText3D(const char * str, const char * fontName, const Vec3 & position, const Size2F & size, const Color4 & color);

	protected:
		bool IsVisible(const Vec3 & position, const Size2F & size);
	};
}

