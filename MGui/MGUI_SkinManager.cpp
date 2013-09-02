#include "MGUI_SkinManager.h"

namespace Myway {

	MGUI_SkinManager * MGUI_SkinManager::msInstance = NULL;

	MGUI_SkinManager::MGUI_SkinManager()
	{
		msInstance = this;
	}

	MGUI_SkinManager::~MGUI_SkinManager()
	{
		Unload();
		msInstance = NULL;
	}

	void MGUI_SkinManager::Load(const char * _source)
	{
		Unload();

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(_source);
		if (stream == NULL)
			return ;

		xml_doc doc;

		XmlHelper::LoadXmlFromMem(doc, (char *)stream->GetData());

		xml_node * skinNode = doc.first_node()->first_node("Skin");

		while (skinNode)
		{
			xml_attri * skinAttri = skinNode->first_attribute("Texture");

			if (skinAttri)
			{
				TString128 skin = skinAttri->value();
				TexturePtr texture = VideoBufferManager::Instance()->Load2DTexture(skin, skin);
				mSkins.PushBack(texture);
			}

			skinNode = skinNode->next_sibling();
		}
	}

	void MGUI_SkinManager::Unload()
	{
		for (int i = 0; i < mSkins.Size(); ++i)
		{
			mSkins[i] = NULL;
		}

		mSkins.Clear();
	}
}