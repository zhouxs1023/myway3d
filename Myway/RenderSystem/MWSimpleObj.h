#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY SimpleObj : public Mover, public Renderer
{
public:
    static const guid128            Guid;

public:
                                    SimpleObj(const String & name);
    virtual                         ~SimpleObj();

            const String &          GetName() const;

            void                    Begin(PRIMITIVE_TYPE type = PRIM_TRIANGLELIST);
            void                    End();

            void                    AddPosition(const Vec3 & pos);
            void                    AddPosition(float x, float y, float z);

            void                    AddNormal(const Vec3 & normal);
            void                    AddNormal(float x, float y, float z);

            void                    AddColor(const Color4 & color);
            void                    AddColor(float r, float g, float b, float a);

            void                    AddTexcoord(const Vec2 & texcoord);
            void                    AddTexcoord(float u, float v);

            void                    AddIndex(short index);

            void                    UpdateBound();
            void                    UpdateGeomtry();

            void                    SetAutoUpdateBound(bool auto_update);
            bool                    GetAutoUpdateBound() const;

    virtual const guid128 &         GetGuid();

    virtual void                    AddRenderQueue(RenderQueue * rq);

    virtual void                    GetWorldPosition(Vec3 * pos);
    virtual void                    GetWorldTransform(Mat4 * form);
    virtual LightList *             GetLightList();

protected:
            Vector<Vec3>            mositions;
            Vector<Vec3>            m_normals;
            Vector<Color4>          m_colors;
            Vector<Vec2>            m_texcoords;
            Vector<short>          mndeces;

            int                  m_decl_flag;
            bool                    m_begined;

            String                  m_name;

            bool                    m_need_update;
            bool                    m_need_render;
            bool                    m_auto_update_bound;
};

}