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
	static const int ClassID = 'Rnd0';

public:
                            Renderer();
    virtual                 ~Renderer();

	virtual int				GetClassID() const { return ClassID; }
	virtual const Aabb &	GetWorldAabb();
    virtual void            GetWorldPosition(Vec3 * pos);
	virtual void            GetWorldTransform(Mat4 * form);
	virtual int				GetBlendMatrix(Mat4 * forms);
    virtual void            BeginRendering();
    virtual void            EndRendering();

            void            SetPrimitiveCount(int size);
            void            SetPrimitiveType(PRIMITIVE_TYPE type);

			VertexStream *  GetVertexStream();
			IndexStream *   GetIndexStream();
            int             GetPrimitiveCount() const;
            PRIMITIVE_TYPE  GetPrimitiveType() const;

            void            SetMaterial(const Material * material);
            Material *      GetMaterial();

	virtual Technique *		GetTechnique(eRenderTechType::enum_t type) { return NULL; }

			void			SetUsingDefferedShading(bool b) { mUsingDefferedShading = b; }
			bool			IsUsingDefferedShading() { return mUsingDefferedShading; }

protected:
    VertexStream            mVertexStream;
    IndexStream             mIndexStream;
    int                     mPrimCount;
    PRIMITIVE_TYPE          mPrimType;

    Material                mMaterial;
	bool					mUsingDefferedShading;
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