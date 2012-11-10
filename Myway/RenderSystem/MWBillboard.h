#pragma once

#include "MWRenderer.h"
#include "MWMover.h"

namespace Myway
{

enum BilloardAlignType
{
    BAT_LEFT_CENTER,
    BAT_LEFT_TOP,
    BAT_LEFT_BOTTOM,

    BAT_CENTER_TOP,
    BAT_CENTER_CENTER,
    BAT_CENTER_BOTTOM,

    BAT_RIGHT_TOP,
    BAT_RIGHT_CENTER,
    BAT_RIGHT_BOTTOM,

    MW_ALIGN_ENUM(BilloardAlignType)
};

class MW_ENTRY Billboard : public AllocObj
{
public:
    Billboard();
    ~Billboard();

    void SetPosition(const Vec3 & pos);
    void SetTexcoord(float left, float top, float right, float bottom);
    void SetColor(const Color4 & color);

    void SetWidth(float width);
    void SetHeight(float height);

    float GetWidth();
    float GetHeight();

    void SetAlignType(BilloardAlignType bat);
    BilloardAlignType GetAlignType();

    void SetVisible(bool bVisible);
    bool GetVisible();

    friend class BillboardSet;

protected:
    void GetVerteces(void * verteces, const Vec3 & right, const Vec3 & up, const Vec3 & dir);

protected:
    Vec3                mosition;
    Vec2                m_texcoord0;
    Vec2                m_texcoord1;
    Color4              m_color;
    float               m_width;
    float               m_height;
    BilloardAlignType   m_align;
    bool                m_bVisible;
};

class MW_ENTRY BillboardSet : public Mover, public Renderer
{
public:
    static const guid128            Guid;

public:
                                    BillboardSet(const String & sName);
    virtual                         ~BillboardSet();

            const String &          GetName() const;

            Billboard *             CreateBillboard();
            Billboard *             CreateBillboard(float width, float height,
                                                    const Vec3 & position,
                                                    const Vec2 & uvLT, const Vec2 & uvRB,
                                                    const Color4 & color);
            Billboard *             GetBillboard(int index);

            void                    DestroyBillboard(Billboard * board);
            void                    DestroyBillboard(int index);
            void                    DestroyAllBillboard();

            int                  GetNumBillboard();

    virtual const guid128 &         GetGuid();

    virtual void                    NotifyCamera(Camera * pCamera);
    virtual void                    AddRenderQueue(RenderQueue * RQ);

    virtual void                    GetWorldPosition(Vec3 * pos);
    virtual void                    GetWorldTransform(Mat4 * form);
    virtual LightList *             GetLightList();

protected:
   String                           m_sName;
   List<Billboard*>                 m_Billboards;
};

}