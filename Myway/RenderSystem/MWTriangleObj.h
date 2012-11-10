#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY TriangleObj : public Mover, public Renderer
{
public:
    static const guid128            guid;

public:
                                    TriangleObj(const String & name);
    virtual                         ~TriangleObj();

    virtual const guid128 &         GetGuid();

            const String &          GetName() const;

            void                    SetFrameMode(bool mode);
            bool                    GetFrameMode() const;

            void                    SetPoint(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);

            const Vec3 &            GetPoint1() const;
            const Vec3 &            GetPoint2() const;
            const Vec3 &            GetPoint3() const;

            void                    _UpdateBound();
            void                    _UpdateGeomtry();

    virtual void                    AddRenderQueue(RenderQueue * rq);

    virtual void                    GetWorldPosition(Vec3 * pos);
    virtual void                    GetWorldTransform(Mat4 * form);

protected:
            String                  m_name;

            Vec3                    moint1;
            Vec3                    moint2;
            Vec3                    moint3;

            bool                    m_need_update;
            bool                    m_need_render;

            bool                    mMode;
};

}