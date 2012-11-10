#pragma once

#include "MWRenderer.h"
#include "MWMover.h"

namespace Myway
{

class MW_ENTRY LineObj : public Mover, public Renderer
{
public:
    static const guid128 guid;

public:
                                    LineObj(const String & name);
    virtual                         ~LineObj();

            const String &          GetName() const;

            void                    PointTo(const Vec3 & p);
            void                    MoveAt(const Vec3 & p);

            const Vec3 &            GetPoint1() const;
            const Vec3 &            GetPoint2() const;

    virtual const guid128 &         GetGuid();

    virtual void                    AddRenderQueue(RenderQueue * rq);

    virtual void                    UpdateBound();
    virtual void                    UpdateGeomtry();

    virtual void                    GetWorldPosition(Vec3 * pos);
    virtual void                    GetWorldTransform(Mat4 * form);

protected:
    String          m_name;

    Vec3            moint1;
    Vec3            moint2;

    bool            m_need_update;
    bool            m_need_render;
};


}