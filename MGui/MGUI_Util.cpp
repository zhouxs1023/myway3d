#include "MGUI_Util.h"

namespace Myway {

	IMP_SLN(MGUI_Util);

	MGUI_Util::MGUI_Util()
	{
		msInstance = this;
	}

	MGUI_Util::~MGUI_Util()
	{
		msInstance = NULL;
	}

	void MGUI_Util::PushText3D(const char * str, const char * fontName, const Vec3 & position, const Size2F & size, const Color4 & color)
	{
		int len = strlen(str);
		if (len == 0 && len >= MAX_TEXT_SIZE)
			return ;

		if (!IsVisible(position, size))
			return ;

		Text3D text;

		text.font = MyGUI::FontManager::getInstance().getByName(fontName);
		d_assert (text.font != NULL);

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, text.wstr, MAX_TEXT_SIZE);

		text.font->getGlyphInfo(0);
	}

	bool MGUI_Util::IsVisible(const Vec3 & position, const Size2F & size)
	{
		Camera * cam = World::Instance()->MainCamera();

		Aabb bound;
		Vec3 right = cam->GetRight();
		Vec3 up = cam->GetUp();
		Vec3 halfVec = 0.5f * size.w * right + 0.5f * size.h * up;

		bound.minimum = position - halfVec;
		bound.maximum = position + halfVec;

		return cam->IsVisible(bound);
	}
}