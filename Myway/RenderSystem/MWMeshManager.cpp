#include "MWMeshManager.h"

using namespace Myway;

/* class: MeshManager
----------------------------------------------------
----------------------------------------------------
*/
IMP_SLN(MeshManager);
MeshManager::MeshManager()
{
    INIT_SLN;
}

MeshManager::~MeshManager()
{
    SHUT_SLN;
    d_assert(mMeshs.Size() == 0);
}

MeshPtr MeshManager::Load(const TString128 & name, const TString128 & source)
{
    MeshPtr mesh = Find(name);

    if (mesh != NULL)
        return mesh;

    mesh = new Mesh(name);
    mesh->SetSourceName(source);

    mMeshs.PushBack(mesh.c_ptr());

    mesh->Load();

    return mesh;
}

MeshPtr MeshManager::CreateMesh(const TString128 & sMeshName)
{
    EXCEPTION_DEBUG(!IsExist(sMeshName), "Mesh: '" + sMeshName + "' exist.");
    Mesh * pMesh = new Mesh(sMeshName);
    mMeshs.PushBack(pMesh);
    return MeshPtr(pMesh);
}

MeshPtr MeshManager::CreateBox(const TString128 & sMeshName,
                               float width, float height, float depth)
{
    MeshPtr pMesh = CreateMesh(sMeshName);

    SubMesh * sm = pMesh->CreateSubMesh();
    

    int iVertexCount = 8;
    int iIndexCount = 12 * 3;
    int iPrimCount = 12;

    VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
    decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
    decl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
    decl->Init();

    sm->GetVertexStream()->SetDeclaration(decl);

    VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 24);

	float * verteces;
	const float half_w = width * 0.5f;
	const float half_h = height * 0.5f;
	const float half_d = depth * 0.5f;
    verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
    {
        Vec3 pos, normal;

        //front
        pos = Vec3(-half_w, half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(-half_w, -half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, -half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        //back
        pos = Vec3(-half_w, half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(-half_w, -half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, -half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;
    }
    buffer->Unlock();

	sm->GetVertexStream()->SetCount(iVertexCount);
	sm->GetVertexStream()->Bind(0, buffer, 24);

    IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
    short * indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
    {
        //front
        *indices++ = 0, *indices++ = 1, *indices++ = 2;
        *indices++ = 2, *indices++ = 1, *indices++ = 3;

        //back
        *indices++ = 5, *indices++ = 4, *indices++ = 7;
        *indices++ = 7, *indices++ = 4, *indices++ = 6;

        //left
        *indices++ = 4, *indices++ = 0, *indices++ = 6;
        *indices++ = 6, *indices++ = 0, *indices++ = 2;

        //right
        *indices++ = 1, *indices++ = 5, *indices++ = 3;
        *indices++ = 3, *indices++ = 5, *indices++ = 7;

        //top
        *indices++ = 4, *indices++ = 5, *indices++ = 0;
        *indices++ = 0, *indices++ = 5, *indices++ = 1;

        //bottom
        *indices++ = 2, *indices++ = 3, *indices++ = 6;
        *indices++ = 6, *indices++ = 3, *indices++ = 7;
    }
    ibuffer->Unlock();

    sm->GetIndexStream()->SetCount(iIndexCount);
    sm->GetIndexStream()->Bind(ibuffer, 0);

    sm->SetPrimitiveCount(iPrimCount);
    sm->SetPrimitiveType(PRIM_TRIANGLELIST);

    pMesh->SetAabb(Vec3(-half_w, -half_h, -half_d), Vec3(half_w, half_h, half_d));
    pMesh->SetBoundingSphere(Vec3(0, 0, 0), Math::Sqrt(half_w * half_w + half_h * half_h + half_d * half_d));

    return pMesh;
}

MeshPtr MeshManager::CreateSphere(const TString128 & sMeshName,
                                  short iRings, short iSegments, float fRadius)
{
    MeshPtr pMesh = CreateMesh(sMeshName);

    SubMesh * sm = pMesh->CreateSubMesh();

    int iVertexCount = (iRings + 1) * (iSegments + 1);
    int iIndexCount = iRings * iSegments * 6;
    int iPrimCount = iIndexCount / 3;

    debug_assert(iIndexCount < 65536, "index buffer too large.");

    VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
    decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
    decl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
    decl->Init();

    sm->GetVertexStream()->SetDeclaration(decl);

    VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 24);

    float * verteces;
    verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
    {
        float fTileRingAngle = (Math::PI_1 / iRings);
        float fTileSegAngle = (Math::PI_2 / iSegments);
        float r;
        short i, j;
        Vec3 pos, normal;

        for (i = 0; i <= iRings; ++i)
        {
            r = fRadius * Math::Sin(i * fTileRingAngle);
            pos.y = fRadius * Math::Cos(i * fTileRingAngle);

            for (j = 0; j <= iSegments; ++j)
            {
                pos.x = r * Math::Cos(j * fTileSegAngle);
                pos.z = r * Math::Sin(j * fTileSegAngle);
                Math::VecNormalize(normal, pos);

                *verteces++ = pos.x;
                *verteces++ = pos.y;
                *verteces++ = pos.z;
                *verteces++ = normal.x;
                *verteces++ = normal.y;
                *verteces++ = normal.z;
            }

        }
    }
    buffer->Unlock();

    sm->GetVertexStream()->Bind(0, buffer, 24);
    sm->GetVertexStream()->SetCount(iVertexCount);

    IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
    short * indices;
    indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
    {
        short row = 0, row_n = 0;
        short i, j;

        for (i = 0; i < iRings; ++i)
        {
            row_n = row + iSegments + 1;

            for (j = 0; j < iSegments; ++j)
            {
                *indices++ = row + j;
                *indices++ = row + j + 1;
                *indices++ = row_n + j;

                *indices++ = row_n + j;
                *indices++ = row + j + 1;
                *indices++ = row_n + j + 1;

            }

            row += iSegments + 1;
        }
    }
    ibuffer->Unlock();

    sm->GetIndexStream()->Bind(ibuffer, 0);
    sm->GetIndexStream()->SetCount(iIndexCount);

    sm->SetPrimitiveCount(iPrimCount);
    sm->SetPrimitiveType(PRIM_TRIANGLELIST);

    pMesh->SetAabb(Vec3(-fRadius, -fRadius, -fRadius), Vec3(fRadius, fRadius, fRadius));
    pMesh->SetBoundingSphere(Vec3(0, 0, 0), fRadius);

    return pMesh;
}

MeshPtr MeshManager::CreateCylinder(const TString128 & sName,
                                    float fRadius, float fHeight, int iRings)
{
    MeshPtr pMesh = CreateMesh(sName);

    SubMesh * sm = pMesh->CreateSubMesh();

    int iVertexCount = (iRings + 1) * 2;
    int iIndexCount = iRings * 6;
    int iPrimCount = iIndexCount / 3;

    debug_assert(iIndexCount < 65536, "index buffer too large.");

    VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
    decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
    decl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
    decl->AddElement(0, 24, DT_FLOAT2, DU_TEXCOORD, 0);
    decl->Init();

    sm->GetVertexStream()->SetDeclaration(decl);

    VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 32);

    float * verteces;
    verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
    {
        float fTileRingAngle = (Math::PI_2 / iRings);
        float x, z, u, rads;

        for (int i = 0; i <= iRings; ++i)
        {
            rads = i * fTileRingAngle;
            u = rads / Math::PI_2;

            Math::SinCos(rads, z, x);

			x *= fRadius;
			z *= fRadius;

            *verteces++ = x;
            *verteces++ = -fHeight / 2;
            *verteces++ = z;
            *verteces++ = x;
            *verteces++ = 0;
            *verteces++ = z;
            *verteces++ = u;
            *verteces++ = 0;

            *verteces++ = x;
            *verteces++ = fHeight / 2;
            *verteces++ = z;
            *verteces++ = x;
            *verteces++ = 0;
            *verteces++ = z;
            *verteces++ = u;
            *verteces++ = 1.0f;
        }
    }
    buffer->Unlock();

    sm->GetVertexStream()->SetCount(iVertexCount);
    sm->GetVertexStream()->Bind(0, buffer, 32);

	/*IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
	short * indices;
	indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
	{
	for (short i = 0; i < iVertexCount; i += 2)
	{
	*indices++ = i + 0;
	*indices++ = i + 1;
	*indices++ = i + 2;

	*indices++ = i + 2;
	*indices++ = i + 1;
	*indices++ = i + 3;
	}
	}
	ibuffer->Unlock();*/

    //sm->GetIndexStream()->SetCount(iIndexCount);
    //sm->GetIndexStream()->Bind(ibuffer, 0);

    sm->SetPrimitiveCount(iPrimCount);
    sm->SetPrimitiveType(PRIM_TRIANGLESTRIP);

	sm->GetMaterial()->SetCullMode(CULL_NONE);

    pMesh->SetAabb(Vec3(-fRadius, 0, -fRadius), Vec3(fRadius, fHeight, fRadius));
    pMesh->SetBoundingSphere(Vec3(0, fHeight * 0.5f, 0), fRadius > fHeight * 0.5f ? fRadius : fHeight * 0.5f);

    return pMesh;
}

MeshPtr MeshManager::CreatePlane(const TString128 & sMeshName,
                                 const Plane & plane,
                                 float width, float height,
                                 int iTileW, int iTileH)
{
    Vec3 a(-width * 0.5f, 0.0f, height * 0.5f);
    Vec3 b(width * 0.5f, 0.0f, height * 0.5f);
    Vec3 c(-width * 0.5f, 0.0f, -height * 0.5f);
    Vec3 d(width * 0.5f, 0.0f, -height * 0.5f);

    Vec3 normal = Vec3(plane.a, plane.b, plane.c);
    Vec3 trans = normal * -plane.d;
    Vec3 axis;

    float dot = Math::VecDot(Vec3::UnitY, normal);

    if (Math::Abs(dot) < (1 - DEFAULT_EPSILON))
    {
        Math::VecCross(axis, Vec3::UnitY, normal);

        float rads = Math::ACos(Math::VecDot(Vec3::UnitY, normal));

        Quat quat;
        Math::QuatFromAxis(quat, axis, rads);

        Math::QuatTranslate(a, quat, a);
        Math::QuatTranslate(b, quat, b);
        Math::QuatTranslate(c, quat, c);
        Math::QuatTranslate(d, quat, d);
    }

	a += trans;
	b += trans;
	c += trans;
	d += trans;

    int iVertexCount = (iTileW + 1) * (iTileH + 1);
    int iIndexCount = iTileW * iTileH * 6;
    int iPrimCount = iIndexCount / 3;

    debug_assert(iIndexCount < 65536, "index buffer too large.");

    MeshPtr pMesh = CreateMesh(sMeshName);
    SubMesh * sm = pMesh->CreateSubMesh();

    VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
    decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
    decl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
    decl->AddElement(0, 24, DT_FLOAT2, DU_TEXCOORD, 0);
    decl->Init();

    sm->GetVertexStream()->SetDeclaration(decl);

    VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 32);

    float * verteces;
    verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
    {
        Vec3 dw = (b - a) / (float)iTileW;
        Vec3 dh = (c - a) / (float)iTileH;
        Vec3 pos_l = a;
        Vec3 pos;

        float start_u = 0;
        float start_v = 0;
        float end_u = 1.0f;
        float end_v = 1.0f;
        float inc_u = 1.0f / (float)iTileW;
        float inc_v = 1.0f / (float)iTileH;
        float u = start_u;
        float v = start_v;

        for (int i = 0; i < iTileH + 1; ++i)
        {
            pos = pos_l;
            u = start_u;

            for (int j = 0; j < iTileW + 1; ++j)
            {
                *verteces++ = pos.x;
                *verteces++ = pos.y;
                *verteces++ = pos.z;

                *verteces++ = normal.x;
                *verteces++ = normal.y;
                *verteces++ = normal.z;

                *verteces++ = u;
                *verteces++ = v;

                pos += dw;
                u += inc_u;
            }

            pos_l += dh;
            v += inc_v;
        }
    }
    buffer->Unlock();

    sm->GetVertexStream()->SetCount(iVertexCount);
    sm->GetVertexStream()->Bind(0, buffer, 32);

    IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
   
    short * indices;
    indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
    {
        int i, j;
        int cur_r = 0;
        int next_r = 0;

        for (i = 0; i < iTileH; ++i)
        {
            next_r = cur_r + iTileW + 1;
            for (j = 0; j < iTileW; ++j)
            {
                *indices++ = cur_r + j;
                *indices++ = cur_r + j + 1;
                *indices++ = next_r + j;

                *indices++ = next_r + j;
                *indices++ = cur_r + j + 1;
                *indices++ = next_r + j + 1;
            }
           
            cur_r += iTileW + 1;
        }
    }
    ibuffer->Unlock();

    sm->GetIndexStream()->SetCount(iIndexCount);
    sm->GetIndexStream()->Bind(ibuffer, 0);

    sm->SetPrimitiveCount(iPrimCount);
    sm->SetPrimitiveType(PRIM_TRIANGLELIST);

    Vec3 vMin, vMax , vCenter;

    Math::VecMinimum(vMin, a, b);
    Math::VecMinimum(vMin, vMin, c);
    Math::VecMinimum(vMin, vMin, d);

    Math::VecMaximum(vMax, a, b);
    Math::VecMaximum(vMax, vMax, c);
    Math::VecMaximum(vMax, vMax, d);

    vCenter = (vMax + vMin) * 0.5f;

    pMesh->SetAabb(vMin, vMax);
    pMesh->SetBoundingSphere(vCenter, Math::VecDistance(vMax, vCenter));

    return pMesh;
}


bool MeshManager::IsExist(const TString128 & sMeshName)
{
    Array<Mesh*>::Iterator iter;
    Array<Mesh*>::Iterator end;

    iter = mMeshs.Begin();
    end = mMeshs.End();

    while (iter != end)
    {
        if ((*iter)->GetName() == sMeshName)
            return TRUE;

        ++iter;
    }

    return  FALSE;
}

MeshPtr MeshManager::Find(const TString128 & sMeshName)
{
    Array<Mesh*>::Iterator iter;
    Array<Mesh*>::Iterator end;

    iter = mMeshs.Begin();
    end = mMeshs.End();

    while (iter != end)
    {
        if ((*iter)->GetName() == sMeshName)
            return MeshPtr(*iter);

        ++iter;
    }

    return  MeshPtr();
}

void MeshManager::Destroy(const TString128 & sMeshName)
{
    Array<Mesh*>::Iterator iter;
    Array<Mesh*>::Iterator end;

    iter = mMeshs.Begin();
    end = mMeshs.End();

    while (iter != end)
    {
        if ((*iter)->GetName() == sMeshName)
        {
            delete (*iter);
            mMeshs.Erase(iter);
            break;
        }

        ++iter;
    }
}

void MeshManager::Destroy(Mesh * pMesh)
{
    Array<Mesh*>::Iterator iter;
    Array<Mesh*>::Iterator end;

    iter = mMeshs.Begin();
    end = mMeshs.End();

    while (iter != end)
    {
        if ((*iter) == pMesh)
        {
            delete (*iter);
            mMeshs.Erase(iter);
            break;
        }

        ++iter;
    }
}

void MeshManager::DestoryAll()
{
    Array<Mesh*>::Iterator iter;
    Array<Mesh*>::Iterator end;

    iter = mMeshs.Begin();
    end = mMeshs.End();

    while (iter != end)
    {
        delete (*iter);
        ++iter;
    }

    mMeshs.Clear();
}

