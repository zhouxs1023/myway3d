#include "MWFontManager.h"
#include "MWDynamicTTFont.h"

using namespace Myway;

FontManager::FontManager()
{
    m_Loader = new DefaultFontLoader();
}

FontManager::~FontManager()
{
    List<Font*>::Iterator iter;
    List<Font*>::Iterator end;

    iter = m_Fonts.Begin();
    end = m_Fonts.End();

    while (iter != end)
    {
        delete *iter;
        ++iter;
    }

    m_Fonts.Clear();

    safe_delete(m_Loader);
}

Font * FontManager::CreateFont(const String & sName, FONT_TYPE type)
{
    assert(!GetFont(sName));

    if (type == FONT_DYNAMIC_TTF)
    {
         Font * font = new DynamicTTFont(sName);
         m_Fonts.PushBack(font);
         return font;
    }
    else
    {
        debug_assert(0, "not support other font.");
        return NULL;
    }
}

Font * FontManager::LoadFont(const String & sFileName)
{
    return m_Loader->Load(sFileName);
}

void FontManager::Unload(const String & sName)
{
    List<Font*>::Iterator iter;
    List<Font*>::Iterator end;

    iter = m_Fonts.Begin();
    end = m_Fonts.End();

    while (iter != end && (*iter)->GetName() != sName)
    {
        ++iter;
    }

    assert(iter != end);

    delete *iter;
    m_Fonts.Erase(iter);
}

Font * FontManager::GetFont(const String & sName)
{
    List<Font*>::Iterator iter;
    List<Font*>::Iterator end;

    iter = m_Fonts.Begin();
    end = m_Fonts.End();

    while (iter != end && (*iter)->GetName() != sName)
    {
        ++iter;
    }

    return iter != end ? (*iter) : NULL;
}