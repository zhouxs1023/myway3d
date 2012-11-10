#include "MWMesh.h"
#include "MWMeshManager.h"
#include "MWResourceManager.h"


using namespace Myway;



SubMesh::SubMesh()
: mPrimCount(0)
, mPrimType(PRIM_TRIANGLELIST)
{
}

SubMesh::~SubMesh()
{
}

void SubMesh::SetPrimitiveCount(int count)
{
    mPrimCount = count;
}

void SubMesh::SetPrimitiveType(PRIMITIVE_TYPE pt)
{
    mPrimType = pt;
}

VertexStream * SubMesh::GetVertexStream()
{
    return &mVertexStream;
}

IndexStream * SubMesh::GetIndexStream()
{
    return &mIndexStream;
}

int SubMesh::GetPrimitiveCount()
{
    return mPrimCount;
}

PRIMITIVE_TYPE SubMesh::GetPrimitiveType()
{
    return mPrimType;
}


SubMesh * Mesh::GetSubMesh(int index)
{ 
    return mMeshes[index];
}

int Mesh::GetSubMeshCount()
{ 
    return mMeshes.Size();
}

void Mesh::SetSkeletonName(const TString128 & sName)
{
    mSkeleton = sName;
}

bool Mesh::HasSkeleton()
{ 
    return mSkeleton.Length() != 0; 
}

const TString128 & Mesh::GetSkeletonName()
{ 
    return mSkeleton;
}

const TString128 & Mesh::GetName() const
{
    return mName;
}


SubMesh * Mesh::CreateSubMesh()
{
    SubMesh * sm = new SubMesh();

    mMeshes.PushBack(sm);

    return sm;
}







/* :) Mesh
----------------------------------------------------
----------------------------------------------------
*/
Mesh::Mesh(const TString128 & sName)
: mName(sName),
  mBound(Vec3::Zero, Vec3::Zero),
  mSphere(Vec3::Zero, 0.0f)
{
    mMaterialResource = new MaterialResource();
}

Mesh::~Mesh()
{
    delete mMaterialResource;
    RemoveAllSubMehs();
}

void Mesh::DeleteSelf()
{
    MeshManager::Instance()->Destroy(this);
}

void Mesh::RemoveSubMesh(int index)
{   
    delete mMeshes[index];
    mMeshes.Erase(index);
}

void Mesh::RemoveAllSubMehs()
{
    Array<SubMesh*>::Iterator iter;
    Array<SubMesh*>::Iterator end;

    iter = mMeshes.Begin();
    end = mMeshes.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    mMeshes.Clear();
}

void Mesh::CalcuBounds()
{
    Array<SubMesh*>::Iterator whr = mMeshes.Begin();
    Array<SubMesh*>::Iterator end = mMeshes.End();

    Aabb bound = Aabb::Invalid;

    while (whr != end)
    {
        SubMesh * mesh = *whr;

        VertexStream * vs = mesh->GetVertexStream();
        VertexDeclarationPtr decl = vs->GetDeclaration();

        VertexElement ve;
        ve.Stream = DECL_UNUSED;
        ve.Offset = 0;
        ve.Usage = DU_POSITION;
        ve.Type = DT_FLOAT3;
        ve.UsageIndex = 0;

        decl->GetElement(ve);

        if (ve.Stream != DECL_UNUSED)
        {
            VertexBufferPtr buffer = vs->GetStream(ve.Stream);
            int stride = vs->GetStreamStride(ve.Stream);
            if (buffer.NotNull())
            {
                char * data = (char *)buffer->Lock(0, 0, LOCK_READONLY);

                for (int i = 0; i < vs->GetCount(); ++i)
                {
                    Vec3 pos = *(Vec3*)(data + ve.Offset);

                    Math::VecMinimum(bound.minimum, bound.minimum, pos);
                    Math::VecMaximum(bound.maximum, bound.maximum, pos);
                    
                    data += stride;
                }

                buffer->Unlock();
            }
        }

        ++whr;
    }

    mBound = bound;
    mSphere.center = bound.GetCenter();
    mSphere.radius = Math::VecDistance(mSphere.center, bound.minimum);
}


void Mesh::Load()
{
    if (mLoadState == Resource::LOADED)
        return ;

    if (!CanLoad())
    {
        mLoadState = Resource::LOADED;
        return ;
    }

    ResourceManager::Instance()->GetResourceLoader()->Load(this);

    TString128 source = File::RemoveExternName(GetSourceName()) + ".material";
    const char * group = GetGroupName().c_str();

    mMaterialResource->SetSourceName(source);
    mMaterialResource->SetGroupName(group);
    mMaterialResource->Load();
}

void Mesh::Reload()
{
    if (!CanLoad())
        return ;

    if (mLoadState == Resource::LOADED)
        Unload();

    ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
}

void Mesh::Unload()
{
    if (!CanLoad())
        return ;

    mBound = Aabb(Vec3::Zero, Vec3::Zero);
    mSphere = Sphere(Vec3::Zero, 0.0f);
    RemoveAllSubMehs();
    mSkeleton = "";
}

void Mesh::LoadImp(DataStreamPtr stream)
{
    MeshLoader::Load(this, stream);
}

void Mesh::SetAabb(const Vec3 & vMin, const Vec3 & vMax)
{
    mBound.minimum = vMin;
    mBound.maximum = vMax;
}

void Mesh::SetAabb(const Aabb & bound)
{
    mBound = bound;
}

void Mesh::SetBoundingSphere(const Vec3 & vCenter, float fRadius)
{
    mSphere.center = vCenter;
    mSphere.radius = fRadius;
}

void Mesh::SetBoundingSphere(const Sphere & sphere)
{
    mSphere = sphere;
}

const Aabb& Mesh::GetAabb() const
{
    return mBound;
}

const Sphere & Mesh::GetBoundingSphere() const
{
    return mSphere;
}

MaterialResource * Mesh::GetMaterialSource()
{
    return mMaterialResource;
}