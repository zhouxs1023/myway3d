#include "MWTerrain.h"
#include "MWTerrainLod.h"
#include "MWTerrainSection.h"
#include "MWWorld.h"
#include "MWImage.h"
#include "MWResourceManager.h"
#include "MWEnvironment.h"
#include "MWRenderEvent.h"

namespace Myway {

Terrain::Terrain(const TString128 & source)
: mPositions(NULL)
, mNormals(NULL)
, mTexcoords(NULL)
, mxVertSize(0)
, myVertSize(0)
, mVertSize(0)
, mxSectionSize(0)
, mySectionSize(0)
, mBound(0, 0, 0, 0, 0, 0)
{
    LoadConfig(source);
    mLod = new TerrainLod(TnConst::kMaxDetailLevel);

    Init();

    mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Terrain");

    RenderEvent::OnPreVisibleCull += this;
}

Terrain::~Terrain()
{
    safe_delete (mLod);
    safe_delete_array(mPositions);
    safe_delete_array(mNormals);
    safe_delete_array(mTexcoords);

    for (int i = 0; i < mSections.Size(); ++i)
    {
        delete mSections[i];
        World::Instance()->DestroySceneNode(mSceneNodes[i]);
    }

    mSceneNodes.Clear();
    mSections.Clear();

    RenderEvent::OnPreVisibleCull -= this;
}


void Terrain::Init()
{
    float * heights = LoadHightmap();

    mxSectionSize = (mxVertSize - 1) / (TnConst::kSectionVertexSize - 1);
    mySectionSize = (myVertSize - 1) / (TnConst::kSectionVertexSize - 1);

    mPositions = new Vec3[mVertSize];
    mNormals = new Vec3[mVertSize];
    mTexcoords = new Vec2[mVertSize];

    CalcuPositions(heights, 0, mConfig.height, mConfig.width, 0);
    CalcuNormals();
    CalcuTexcoords(0, 0, 1, 1);
    CalcuBounds();

    free_m(heights);

    d_assert (!((mxVertSize - 1) % (TnConst::kSectionVertexSize - 1)) &&
              !((myVertSize - 1) % (TnConst::kSectionVertexSize - 1)) &&
              Math::IsPowerOfTwo(myVertSize - 1) &&
              Math::IsPowerOfTwo(myVertSize - 1));

    mWeightmap[0] = VideoBufferManager::Instance()->Load2DTexture(mConfig.weightmap0, mConfig.weightmap0);
    mWeightmap[1] = VideoBufferManager::Instance()->Load2DTexture(mConfig.weightmap1, mConfig.weightmap1);

    for (int i = 0; i < TnConst::kMaxLayers; ++i)
    {
        mLayer[i] = VideoBufferManager::Instance()->Load2DTexture(mConfig.layers[i].texture, mConfig.layers[i].texture);
        mUVScale[i] = mConfig.layers[i].scale;
    }

    AllocSection();
}

void Terrain::AllocSection()
{
    TString128 name = "TerrainSection";

    for (int y = 0; y < mySectionSize; ++y)
    {
        for (int x = 0; x <  mxSectionSize; ++x)
        {
            TerrainSection * section = new TerrainSection(this, x, y);
            SceneNode * node = World::Instance()->CreateSceneNode(name + x + " " + y);

            node->Attach(section);

            mSections.PushBack(section);
            mSceneNodes.PushBack(node);
        }
    }
}

float * Terrain::LoadHightmap()
{
    ImagePtr image = VideoBufferManager::Instance()->LoadImageSource(mConfig.hightmap, mConfig.mResourceGroup, IMAGE_FILTER_NONE);

    d_assert (image != NULL);

    mxVertSize = image->GetSrcWidth();
    myVertSize = image->GetSrcHeight();
    mVertSize = mxVertSize * myVertSize;

    float * height = (float *)alloc_mem(sizeof(float) * mxVertSize * myVertSize);

    FORMAT fmt = image->GetFormat();
    if (fmt == FMT_A8 || fmt == FMT_L8)
    {
        LockedRect rc;
        image->Lock(rc);
        {
            int index = 0;
            unsigned char * data = 0;
            float scale = 1.0f / 255.0f * mConfig.hight;

            for (int i = 0; i < myVertSize; ++i)
            {
                data = (unsigned char*)rc.pData + i * rc.Pitch;
                for (int j = 0; j < mxVertSize; ++j)
                {
                    unsigned char h = *data++;
                    height[index++] = h * scale;
                }
            }
        }
        image->Unlock();
    }
    else if (fmt == FMT_L16)
    {
        LockedRect rc;
        image->Lock(rc);
        {
            int index = 0;
            unsigned short * data = 0;
            float scale = 1.0f / 65535.0f * mConfig.hight;

            for (int i = 0; i < myVertSize; ++i)
            {
                // data = (short*)rc.pData + i * rc.Pitch; (bug, rc.pitch is bytes)
                data = (unsigned short*)((unsigned char*)rc.pData + i * rc.Pitch);
                for (int j = 0; j < mxVertSize; ++j)
                {
                    height[index++] = float(*data++) * scale;
                }
            }
        }
        image->Unlock();
    }

    return height;
}


const Vec3 & Terrain::GetPosition(int x, int y) const
{
    d_assert (x < mxVertSize && y < myVertSize);

    return mPositions[y * mxVertSize + x];
}

const Vec3 & Terrain::GetNormal(int x, int y) const
{
    d_assert (x < mxVertSize && y < myVertSize);

    return mNormals[y * mxVertSize + x];
}

const Vec2 & Terrain::GetTexcoord(int x, int y) const
{
    d_assert (x < mxVertSize && y < myVertSize);

    return mTexcoords[y * mxVertSize + x];
}

const Vec3 * Terrain::GetPositions() const
{
    return mPositions;
}

const Vec3 * Terrain::GetNormals() const
{
    return mNormals;
}

const Vec2 * Terrain::GetTexcoords() const
{
    return mTexcoords;
}

void Terrain::CalcuPositions(const float * pHeights, float xStart, float yStart, float xEnd, float yEnd)
{
    float start_x = xStart;
    float start_z = yStart;
    float end_x = xEnd;
    float end_z = yEnd;
    float inc_x = (end_x - start_x) / (mxVertSize - 1);
    float inc_z = (end_z - start_z) / (myVertSize - 1);

    float x = start_x;
    float z = start_z;

    int index = 0;
    for (int i = 0; i < myVertSize; ++i)
    {
        x = start_x;
        for (int j = 0; j < mxVertSize; ++j)
        {
            mPositions[index].x = x;
            mPositions[index].z = z;
            mPositions[index].y = pHeights[index];

            ++index;
            x += inc_x;
        }

        z += inc_z;
    }
}

void Terrain::CalcuNormals()
{
    Memzero(mNormals, mVertSize * sizeof(Vec3));

    int tri1_1 = 0;
    int tri1_2 = 0;
    int tri1_3 = 0;
    int tri2_1 = 0;
    int tri2_2 = 0;
    int tri2_3 = 0;
    int xtiles = mxVertSize - 1;
    int ytiles = myVertSize - 1;
    int row_c = 0;
    int row_n = 0;
    Vec3 dir1, dir2;
    Vec3 normal;

    for (int i = 0; i < ytiles; ++i)
    {
        row_n = row_c + mxVertSize;

        for (int j = 0; j < xtiles; ++j)
        {
            tri1_1 = row_c + j;
            tri1_2 = row_c + j + 1;
            tri1_3 = row_n + j;

            tri2_1 = row_n + j;
            tri2_2 = row_c + j + 1;
            tri2_3 = row_n + j + 1;

            dir1 = mPositions[tri1_2] - mPositions[tri1_1];
            dir2 = mPositions[tri1_3] - mPositions[tri1_1];

            Math::VecCrossN(normal, dir1, dir2);
            mNormals[tri1_1] += normal;
            mNormals[tri1_2] += normal;
            mNormals[tri1_3] += normal;

            dir1 = mPositions[tri2_2] - mPositions[tri2_1];
            dir2 = mPositions[tri2_3] - mPositions[tri2_1];
            Math::VecCrossN(normal, dir1, dir2);
            mNormals[tri2_1] += normal;
            mNormals[tri2_2] += normal;
            mNormals[tri2_3] += normal;
        }

        row_c += mxVertSize;
    }

    for (int i = 0; i < mVertSize; ++i)
    {
        Math::VecNormalize(mNormals[i], mNormals[i]);
    }
}

void Terrain::CalcuTexcoords(float startu, float startv, float endu, float endv)
{
    float start_u = startu;
    float start_v = startv;
    float end_u = endu;
    float end_v = endv;
    float inc_u = (end_u - start_u) / (mxVertSize - 1);
    float inc_v = (end_v - start_v) / (myVertSize - 1);

    float u = start_u;
    float v = start_v;

    int index = 0;
    for (int i = 0; i < myVertSize; ++i)
    {
        u = start_u;
        for (int j = 0; j < mxVertSize; ++j)
        {
            mTexcoords[index].x = Math::FastClamp(u, startu, endu);
            mTexcoords[index].y = Math::FastClamp(v, startv, endv);

            ++index;
            u += inc_u;
        }

        v += inc_v;
    }
}

void Terrain::CalcuBounds()
{
    Vec3 minimum = mPositions[0];
    Vec3 maximum = mPositions[0];

    for (int i = 1; i < mVertSize; ++i)
    {
        Math::VecMinimum(minimum, minimum, mPositions[i]);
        Math::VecMaximum(maximum, maximum, mPositions[i]);
    }

    mBound.minimum = minimum;
    mBound.maximum = maximum;
}

TerrainSection * Terrain::GetSection(int x, int y)
{
    d_assert (x < mxSectionSize && y < mySectionSize);

    int index = y * mxSectionSize + x;

    return mSections[index];
}

float Terrain::GetHeight(int x, int y)
{
    d_assert (x < mxVertSize && y < myVertSize);

    return GetHeight(y * mxVertSize + x);
}

float Terrain::GetHeight(int index)
{
    d_assert (index < mVertSize);

    return mPositions[index].y;
}

float Terrain::GetHeight(float x, float z)
{
    const Vec3 & start = mPositions[0];
    const Vec3 & end = mPositions[mVertSize - 1];

    float fx = (x - start.x) / (end.x - start.x) * (mxVertSize - 1);
    float fz = (z - start.z) / (end.z - start.z) * (myVertSize - 1);

    int ix = (int) fx;
    int iz = (int) fz;

    d_assert(ix >= 0 && ix <= mxVertSize - 1 &&
             iz >= 0 && iz <= myVertSize - 1);

    float dx = fx - ix;
    float dz = fz - iz;

    float a = GetHeight(ix, iz);
    float b = GetHeight(ix + 1, iz);
    float c = GetHeight(ix, iz + 1);
    float d = GetHeight(ix + 1, iz + 1);
    float m = (b + c) * 0.5f;
    float h1, h2, final;

    if (dx + dz <= 1.0f)
    {
        d = m + (m - a);
    }
    else
    {
        a = m + (m - d);
    }

    h1 = a * (1.0f - dx) + b * dx;
    h2 = c * (1.0f - dx) + d * dx; 
    final = h1 * (1.0f - dz) + h2 * dz;

    return final;
}

Vec3 Terrain::GetPosition(const Ray & ray)
{
    Vec3 result(0, 0, 0);
    const int iMaxCount = 1000;

    if (ray.origin > mBound.minimum &&
        ray.origin < mBound.maximum)
    {
        
        int i = 0;
        Vec3 pos = ray.origin;
        float y = 0;

        if (ray.direction == Vec3::UnitY)
        {
            y = GetHeight(pos.x, pos.z);
            if (pos.y <= y)
            {
                result = Vec3(pos.x, y, pos.z);
            }
        }
        else if (ray.direction == Vec3::NegUnitY)
        {
            y = GetHeight(pos.x, pos.z);
            if (pos.y >= y)
            {
                result = Vec3(pos.x, y, pos.z);
            }
        }
        else
        {
            while (pos.x > mBound.minimum.x && pos.x < mBound.maximum.x &&
                pos.z > mBound.minimum.z && pos.z < mBound.maximum.z &&
                i++ < iMaxCount)
            {
                y = GetHeight(pos.x, pos.z);
                if (pos.y <= y)
                {
                    result = Vec3(pos.x, y, pos.z);
                    break;
                }

                pos += ray.direction;
            }
        }
    }

    return result;
}

const Aabb & Terrain::GetBound() const
{
    return mBound;
}


void Terrain::LoadConfig(const TString128 & source)
{
    DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source.c_str());
    d_assert (stream.NotNull());

    xml_doc doc;
    doc.parse<0>((char *)stream->GetData());

    xml_node * root = doc.first_node("Terrain");

    xml_node * ndResGourp = root->first_node("group");
    xml_node * ndWidth = root->first_node("width");
    xml_node * ndHeight = root->first_node("height");
    xml_node * ndHight = root->first_node("hight");
    xml_node * ndHightmap = root->first_node("hightmap");
    xml_node * ndWeightmap = root->first_node("weightmap0");
    xml_node * ndWeightmap1 = root->first_node("weightmap1");
    xml_node * ndDecal = root->first_node("layers");
    xml_node * ndMorphEnable = root->first_node("morph");
    xml_node * ndMorphStart = root->first_node("start");

    d_assert (ndResGourp && ndWidth && ndHeight && ndHight && ndHightmap && ndWeightmap && ndWeightmap1 && ndDecal);

    mConfig.mResourceGroup = ndResGourp->first_attribute("value")->value();
    mConfig.width = (float)atof(ndWidth->first_attribute("value")->value());
    mConfig.height = (float)atof(ndHeight->first_attribute("value")->value());
    mConfig.hight = (float)atof(ndHight->first_attribute("value")->value());
    mConfig.hightmap = ndHightmap->first_attribute("value")->value();
    mConfig.weightmap0 = ndWeightmap->first_attribute("value")->value();
    mConfig.weightmap1 = ndWeightmap1->first_attribute("value")->value();

    xml_node * ndLayer = ndDecal->first_node("layer");
    int layerIndex = 0;

    while (ndLayer)
    {
        xml_attri * arTex = ndLayer->first_attribute("texture");
        xml_attri * arScale = ndLayer->first_attribute("scale");

        const char * tex = arTex->value();
        float scale = (float)atof(arScale->value());

        d_assert (layerIndex < TnConst::kMaxLayers);

        mConfig.layers[layerIndex].texture = tex;
        mConfig.layers[layerIndex].scale = scale;

        layerIndex++;
        ndLayer = ndLayer->next_sibling();
    }

    if (ndMorphEnable)
        mConfig.morphEnable = strcmp(ndMorphEnable->first_attribute("value")->value(), "true") == 0 ;

    if (ndMorphStart)
        mConfig.morphStart = (float)atof(ndMorphStart->first_attribute("value")->value());
}



void Terrain::OnCall(Event * sender, void * data)
{
    if (sender == &RenderEvent::OnPreVisibleCull)
        mVisibleSections.Clear();
}

void Terrain::Render()
{
    RenderSystem * render = RenderSystem::Instance();

    for (int i = 0; i < mVisibleSections.Size(); ++i)
    {
        mVisibleSections[i]->UpdateLod();
    }

    SamplerState state;
    render->SetTexture(0, state, mWeightmap[0].c_ptr());
    render->SetTexture(1, state, mWeightmap[1].c_ptr());
    render->SetTexture(2, state, mLayer[0].c_ptr());
    render->SetTexture(3, state, mLayer[1].c_ptr());
    render->SetTexture(4, state, mLayer[2].c_ptr());
    render->SetTexture(5, state, mLayer[3].c_ptr());
    render->SetTexture(6, state, mLayer[4].c_ptr());
    render->SetTexture(7, state, mLayer[5].c_ptr());
    render->SetTexture(8, state, mLayer[6].c_ptr());
    render->SetTexture(9, state, mLayer[7].c_ptr());

    mTech->GetVertexShaderParamTable()->GetParam("UVScale")->SetUnifom(
        mUVScale[0], mUVScale[1], mUVScale[2], mUVScale[3]);

    for (int i = 0; i < mVisibleSections.Size(); ++i)
    {
        TerrainSection * section = mVisibleSections[i];
        section->PreRender();

        render->Render(mTech, &section->mRender);
    }

}

void Terrain::RenderInMirror()
{
    RenderSystem * render = RenderSystem::Instance();

    for (int i = 0; i < mVisibleSections.Size(); ++i)
    {
        mVisibleSections[i]->UpdateLod();
    }

    SamplerState state;
    render->SetTexture(0, state, mWeightmap[0].c_ptr());
    render->SetTexture(1, state, mWeightmap[1].c_ptr());
    render->SetTexture(2, state, mLayer[0].c_ptr());
    render->SetTexture(3, state, mLayer[1].c_ptr());
    render->SetTexture(4, state, mLayer[2].c_ptr());
    render->SetTexture(5, state, mLayer[3].c_ptr());
    render->SetTexture(6, state, mLayer[4].c_ptr());
    render->SetTexture(7, state, mLayer[5].c_ptr());
    render->SetTexture(8, state, mLayer[6].c_ptr());
    render->SetTexture(9, state, mLayer[7].c_ptr());

    mTech->GetVertexShaderParamTable()->GetParam("UVScale")->SetUnifom(
        mUVScale[0], mUVScale[1], mUVScale[2], mUVScale[3]);

    for (int i = 0; i < mVisibleSections.Size(); ++i)
    {
        TerrainSection * section = mVisibleSections[i];
        section->PreRender();

        section->mRender.rState.cullMode = CULL_CW;
        render->Render(mTech, &section->mRender);
        section->mRender.rState.cullMode = CULL_CCW;
    }
}



}