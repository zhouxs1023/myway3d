#pragma once

#include "MWResource.h"
#include "MWRenderer.h"
#include "MWMaterial.h"
#include "MWSkeleton.h"
#include "MWAnimation.h"
#include "MWColMesh.h"

namespace Myway
{

/* :) SubMesh
-----------------------------------------------------
-----------------------------------------------------
*/
class MW_ENTRY SubMesh : public AllocObj
{
    friend class MeshManager;

public:
    SubMesh();
    ~SubMesh();

	void                    SetPrimitiveCount(int iPrimCount);
	void                    SetPrimitiveType(PRIMITIVE_TYPE pt);

	VertexStream *          GetVertexStream();
	IndexStream *           GetIndexStream();
	int                     GetPrimitiveCount();
	PRIMITIVE_TYPE          GetPrimitiveType();

	Material *				GetMaterial();

	Array<short> &			GetBoneIdMap() { return mBoneIdMaps; }

protected:
	VertexStream            mVertexStream;
	IndexStream             mIndexStream;
	int                     mPrimCount;
	PRIMITIVE_TYPE          mPrimType;
	Material				mMaterial;

	Array<short>			mBoneIdMaps;
};


/* class: Mesh
----------------------------------------------------
@Remark:
    Triangle Mesh.
----------------------------------------------------
*/
class MW_ENTRY Mesh : public Resource, public RefObj
{
    DeclareRefObj();

    friend class MeshManager;

public:
    Mesh(const TString128 & sName);
    ~Mesh();

    const TString128 &          GetName() const;

    void                        SetAabb(const Vec3 & vMin, const Vec3 & vMax);
    void                        SetAabb(const Aabb & bound);
    void                        SetBoundingSphere(const Vec3 & vCenter, float fRadius);
    void                        SetBoundingSphere(const Sphere & sphere);
    const Aabb &                GetAabb() const;
    const Sphere &              GetBoundingSphere() const;

    SubMesh *                   CreateSubMesh();

    SubMesh *                   GetSubMesh(int index);
    int                         GetSubMeshCount();
    void                        RemoveSubMesh(int index);
    void                        RemoveAllSubMehs();

	Skeleton *					GetSkeleton();

	ColMesh *					GetColMesh();
	void						GenColMeshFromRenderMesh();
	void						GenColMesh(Vec3 * verts, int numVerts, int * idxs, int numIdxs);

    void                        CalcuBounds();

	Animation *					CreateAnimation(const TString128 & sAnimation);
	Animation *					GetAnimation(const TString128 & sAnimation);
	int							GetAnimationCount();
	Animation *					GetAnimation(int index);
	void						RemoveAnimation(const TString128 & anim);
	void						RemoveAllAnimation();

    virtual void                Load();
    virtual void                Reload();
    virtual void                Unload();
    virtual void                LoadImp(DataStreamPtr stream);

protected:
    TString128              mName;
    Aabb                    mBound;
    Sphere                  mSphere;
    Array<SubMesh*>         mMeshes;
	Skeleton				mSkeleton;
	ColMesh	*				mColMesh;

	Array<Animation*>      mAnimations;
};

DeclareSmartPtr(Mesh);

}