#include "MWStarfield.h"
#include "MWEnvironment.h"
#include "MWAstronomy.h"
#include "Engine.h"
#include <vector>
#include <algorithm>

#pragma warning (push)
#pragma warning (disable : 4244)

namespace Myway { namespace Env {

    Starfield::Starfield()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Starfield");

        mStars = NULL;
        mSize = 0;
        mIterator = 0;

        mMag0PixelSize = 16;
        mMinPixelSize = 4;
        mMaxPixelSize = 6;
        mMagnitudeScale = Math::Pow(100, 0.2f);
        mObserverLatitude = 45;
        mObserverLongitude = 0;

        Resize(BrightStarCatalogueSize);
    }

    Starfield::~Starfield()
    {
        safe_delete (mStars);
    }

    void Starfield::Render()
    {
        if (mSize == 0)
            return ;

        RenderSystem * render = RenderSystem::Instance();
        const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();
        Camera * cam = World::Instance()->MainCamera();

        int width = dp->Width;
        int height = dp->Height;
        float pixFactor = 1.0f / width;
        float magScale = -Math::Log (mMagnitudeScale) / 2;
        float mag0Size = mMag0PixelSize * pixFactor;
        float minSize = mMinPixelSize * pixFactor;
        float maxSize = mMaxPixelSize * pixFactor;
        float aspect = cam->GetAspect();
        float farclip = cam->GetFarClip();
        Vec3 pos = cam->GetPosition();

        ShaderParam * uSizeParam = mTech->GetVertexShaderParamTable()->GetParam("gSizeParam");
        ShaderParam * uTransform = mTech->GetVertexShaderParamTable()->GetParam("gTransform");
        ShaderParam * uAspect = mTech->GetVertexShaderParamTable()->GetParam("gAspect");

        uSizeParam->SetUnifom(magScale, mag0Size, minSize, maxSize);
        uTransform->SetUnifom(pos.x, pos.y, pos.z, farclip);
        uAspect->SetUnifom(aspect, 0, 0, 0);

        render->Render(mTech, &mRender);
    }

    void Starfield::Resize(int size)
    {
        if (mSize == size)
            return ;

        mSize = size;

        if (mSize == 0)
            return ;

        _random();
        _geometry();

        safe_delete (mStars);
    }

    void Starfield::_random()
    {
        safe_delete (mStars);
        mIterator = 0;

        if (mSize > BrightStarCatalogueSize)
            mSize = BrightStarCatalogueSize;

        if (mSize < BrightStarCatalogueSize) 
        {
            std::vector<std::pair<float, int>> vec;
            vec.reserve(BrightStarCatalogueSize);

            for (int i = 0; i < BrightStarCatalogueSize; ++i)
                vec.push_back(std::make_pair(BrightStarCatalogue[i].magn, i));

            sort(vec.begin(), vec.end());

            for (int i = 0; i < mSize; ++i)
                _add(BrightStarCatalogue[vec[i].second]);
        }
        else
        {
            for (int i = 0; i < BrightStarCatalogueSize; ++i)
                _add(BrightStarCatalogue[i]);
        }
    }

    void Starfield::_geometry()
    {
        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        int iVertexCount = mSize * 4;
        int iIndexCount = mSize * 6;
        int iPrimCount = iIndexCount / 3;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT3, DU_TEXCOORD, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 24);
        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * 2);

        float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            for (int i = 0; i < mSize; ++i)
            {
                const Star & star = mStars[i];

                // Determine position at J2000
                double azm, alt;
                Astronomy::convertEquatorialToHorizontal(
                    Astronomy::J2000, mObserverLatitude, mObserverLongitude,
                    star.RightAscension, star.Declination, azm, alt);

                azm = Math::DegreeToRadian(azm);
                alt = Math::DegreeToRadian(alt);

                float z = -Math::Cos(azm) * Math::Cos(alt);
                float x =  Math::Sin(azm) * Math::Cos(alt);
                float y = -Math::Sin(alt);
                float m = star.Magnitude;

                *vert++ = x; *vert++ = y; *vert++ = z;
                *vert++ = -1; *vert++ = 1; *vert++ = m;

                *vert++ = x; *vert++ = y; *vert++ = z;
                *vert++ = 1; *vert++ = 1; *vert++ = m;

                *vert++ = x; *vert++ = y; *vert++ = z;
                *vert++ = -1; *vert++ = -1; *vert++ = m;

                *vert++ = x; *vert++ = y; *vert++ = z;
                *vert++ = 1; *vert++ = -1; *vert++ = m;
            }
        }
        vb->Unlock();

        vxStream->Bind(0, vb, 24);
        vxStream->SetCount(iVertexCount);

        short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
        {
            for (short i = 0; i < (short)mSize; ++i)
            {
                int index = i * 4;

                *idx++ = i;
                *idx++ = i + 1;
                *idx++ = i + 2;

                *idx++ = i + 2;
                *idx++ = i + 1;
                *idx++ = i + 3;
            }
        }
        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLESTRIP;

        mRender.rState.blendMode = BM_ALPHA_BLEND;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_ALWAYS;
    }

    void Starfield::_add(const BrightStarCatalogueEntry & entry)
    {
        Star & s = mStars[mIterator++];

        s.RightAscension = 360 / 24.0f * (
            Math::Abs(entry.rasc_hour) +
            entry.rasc_min / 60.0f +
            entry.rasc_sec / 3600.0f);

        s.Declination = Math::Sign(entry.decl_deg) * (
            Math::Abs(entry.decl_deg) +
            entry.decl_min / 60.0f +
            entry.decl_sec / 3600.0f);

        s.Magnitude = entry.magn;
    }
}}


#pragma warning (pop)
