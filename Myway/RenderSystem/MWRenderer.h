#pragma once

#include "MWMaterial.h"
#include "MWPrimtiveStream.h"
#include "MWLight.h"

namespace Myway
{

class MW_ENTRY Renderer
{
    DECLARE_ALLOC();

public:
                            Renderer();
    virtual                 ~Renderer();

    virtual void            GetWorldPosition(Vec3 * pos);
    virtual void            GetWorldTransform(Mat4 * form);
    virtual void            GetBlendMatrix(Mat4 * forms, int * count);
    virtual void            BeginRendering();
    virtual void            EndRendering();

            void            SetPrimitiveCount(int size);
            void            SetPrimitiveType(PRIMITIVE_TYPE type);

    virtual VertexStream *  GetVertexStream();
    virtual IndexStream *   GetIndexStream();
            int             GetPrimitiveCount() const;
            PRIMITIVE_TYPE  GetPrimitiveType() const;

            void            SetMaterial(const Material * material);
            Material *      GetMaterial();

protected:
    VertexStream            mVertexStream;
    IndexStream             mIndexStream;
    int                     mPrimCount;
    PRIMITIVE_TYPE          mPrimType;

    Material                mMaterial;
};

struct RenderOp
{
    Mat4 xform;
    RenderState rState;
    VertexStream vxStream;
    IndexStream ixStream;
    int iPrimCount;
    PRIMITIVE_TYPE ePrimType;

    RenderOp()
    {
        xform = Mat4::Identity;
        iPrimCount = 0;
        ePrimType = PRIM_TRIANGLELIST;
    }
};

}