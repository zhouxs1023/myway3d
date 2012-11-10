#include "MWSkyDome.h"

using namespace Myway;

SkyDome::SkyDome(const String & sName, const String & sMaterialName, int iRings, int iSegments, float fRadius)
: m_sName(sName),
  mRings(iRings),
  mSegments(iSegments),
  mRadius(fRadius),
  m_bEnable(FALSE)
{
    _Initialize();
    SetMaterial(sMaterialName);
}

SkyDome::~SkyDome()
{
}

const String & SkyDome::GetName() const
{
    return m_sName;
}

int SkyDome::GetRings() const
{
    return mRings;
}

int SkyDome::GetSegments() const
{
    return mSegments;
}

float SkyDome::GetRadius() const
{
    return mRadius;
}

void SkyDome::SetEnable(bool bEnable)
{
    m_bEnable = bEnable;
}

bool SkyDome::GetEnable() const
{
    return m_bEnable;
}

void SkyDome::_Initialize()
{
    int iVertexCount = (mRings + 1) * (mSegments + 1);
    int iIndexCount = mRings * mSegments * 6;
    int iPrimCount = iIndexCount / 3;

    debug_assert(iIndexCount < 65536, "index buffer too large.");

    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->EndDecl();

    mVertexStream.SetDeclaration(decl);


    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(iVertexCount * 12,
                                                                               USAGE_WRITEONLY,
                                                                               POOL_MANAGED);
    float * verteces;
    buffer->Lock(0, 0, (void**)&verteces, LOCK_DISCARD);
    {
        float fTileRingAngle = (Math::PI_1 / mRings);
        float fTileSegAngle = (Math::PI_2 / mSegments);
        float r;
        short i, j;
        Vec3 pos;

        for (i = 0; i <= mRings; ++i)
        {
            r = mRadius * Math::Sin(i * fTileRingAngle);
            pos.y = mRadius * Math::Cos(i * fTileRingAngle);

            for (j = 0; j <= mSegments; ++j)
            {
                pos.x = r * Math::Cos(j * fTileSegAngle);
                pos.z = r * Math::Sin(j * fTileSegAngle);

                *verteces++ = pos.x;
                *verteces++ = pos.y;
                *verteces++ = pos.z;
            }

        }
    }
    buffer->Unlock();

    mVertexStream.SetCount(iVertexCount);
    mVertexStream.Bind(0, buffer, 16);

    //index stream
    IndexBufferPtr ibuffer = VideoBufferManager::Instance().CreateIndexBuffer(iIndexCount * sizeof(short),
                                                                              USAGE_WRITEONLY,
                                                                              POOL_MANAGED,
                                                                              FMT_INDEX16);
    short * indices;
    ibuffer->Lock(0, 0, (void**)&indices, LOCK_DISCARD);
    {
        short row = 0, row_n = 0;
        short i, j;
        short iRings = (short)mRings;
        short iSegments = (short)mSegments;

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

    mIndexStream.SetCount(iIndexCount);
    mIndexStream.Bind(ibuffer, 0);

    SetPrimitiveCount(iPrimCount);
    SetPrimitiveType(PRIM_TRIANGLELIST);
}