#pragma once

#include "MWRenderer.h"
#include "MWMover.h"
#include "MWFont.h"
#include "MWFString.h"
#include "MWFontManager.h"

namespace Myway
{

class MW_ENTRY TextObj : public Mover, public Renderer
{
public:
    static const guid128 guid;

public:
    TextObj(const String & name);
    virtual ~TextObj();

    const String &                  GetName() const;

    virtual void                    GetWorldPosition(Vec3 * pos);
    virtual void                    GetWorldTransform(Mat4 * form);

    void                            SetFont(const String & font);
    void                            SetTextSize(float height);
    void                            SetText(const FString & text);

    const guid128 &                 GetGuid();

    void                            NotifyCamera(Camera * cam);
    void                            AddRenderQueue(RenderQueue * rq);

protected:
    void                            _UpdateGeomtry();
    void                            _UpdateText();

protected:
    String                          m_name;
    FString                         m_text;
    ftid                            mtid;
    bool                            m_need_update;
    bool                            m_need_update_text;
    float                           m_height;
    Font *                          mont;
    Mat4                            mMat_world;
    List<ftglyph>                   m_glyphs;
    TexturePtr                      m_texture;
};

}