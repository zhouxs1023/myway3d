#include "MWTerrain.h"
#include "MWTerrainLod.h"
#include "MWTerrainSection.h"
#include "MWWorld.h"
#include "MWImage.h"
#include "MWResourceManager.h"
#include "MWEnvironment.h"
#include "MWRenderEvent.h"

namespace Myway {


Terrain::Terrain(const Config & config)
: tOnPreVisibleCull(RenderEvent::OnPreVisibleCull, this, &Terrain::OnPreVisibleCull)
{
    mLod = new TerrainLod(kMaxDetailLevel);
    mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Terrain");

	Create(config);
}

Terrain::Terrain(const char * source)
: tOnPreVisibleCull(RenderEvent::OnPreVisibleCull, this, &Terrain::OnPreVisibleCull)
{
	mLod = new TerrainLod(kMaxDetailLevel);
	mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Terrain");

	Load(source);
}

Terrain::~Terrain()
{
    safe_delete (mLod);

    for (int i = 0; i < mSections.Size(); ++i)
    {
        delete mSections[i];
        World::Instance()->DestroySceneNode(mSceneNodes[i]);
    }

    mSceneNodes.Clear();
    mSections.Clear();

	safe_delete_array(mHeights);
	safe_delete_array(mNormals);
}

void Terrain::Create(const Config & config)
{
	mConfig = config;

	int x = config.xVertexCount - 1;
	int z = config.zVertexCount - 1;
	int k = kSectionVertexSize - 1;

	if ((x % k) != 0)
		mConfig.xVertexCount = (x / k + 1) * k + 1;

	if ((z % k) != 0)
		mConfig.zVertexCount = (z / k + 1) * k + 1;

	mConfig.iVertexCount = mConfig.xVertexCount * mConfig.zVertexCount;

	mConfig.xSectionCount = mConfig.xVertexCount / (kSectionVertexSize - 1);
	mConfig.zSectionCount = mConfig.zVertexCount / (kSectionVertexSize - 1);

	mConfig.iSectionCount = mConfig.xSectionCount * mConfig.zSectionCount;

	mConfig.xSectionSize = mConfig.xSize / mConfig.xSectionCount;
	mConfig.zSectionSize = mConfig.zSize / mConfig.zSectionCount;

	// create shared x & y stream
	mXYStream = VideoBufferManager::Instance()->CreateVertexBuffer(8 * kSectionVertexSize * kSectionVertexSize);
	
	float * vert = (float *)mXYStream->Lock(0, 0, LOCK_NORMAL);
	{
		float w = mConfig.xSize / mConfig.xSectionCount;
		float h = mConfig.zSize / mConfig.zSectionCount;

		for (int j = 0; j < kSectionVertexSize; ++j)
		{
			for (int i = 0; i < kSectionVertexSize; ++i)
			{
				*vert++ = i / (float)(kSectionVertexSize - 1) * w;
				*vert++ = (1 - j / (float)(kSectionVertexSize - 1)) * h;
			}
		}
	}
	mXYStream->Unlock();

	// init default heights and normals
	mHeights = new float[mConfig.iVertexCount];
	mNormals = new Vec3[mConfig.iVertexCount];

	for (int i = 0; i < mConfig.iVertexCount; ++i)
		mHeights[i] = 0;

	for (int i = 0; i < mConfig.iVertexCount; ++i)
		mNormals[i] = Vec3(0, 1, 0);

	// create sections
	mSections.Resize(mConfig.xSectionCount * mConfig.zSectionCount);
	mSceneNodes.Resize(mConfig.xSectionCount * mConfig.zSectionCount);

	int index = 0;
	for (int i = 0; i < mConfig.xSectionCount; ++i)
	{
		for (int j = 0; j < mConfig.zSectionCount; ++j)
		{
			mSections[index] = new TerrainSection(this, i, j);
			mSceneNodes[index] = World::Instance()->CreateSceneNode();

			mSceneNodes[index]->Attach(mSections[index]);

			++index;
		}
	}

	// load default detail map
	mDefaultDetailMap = VideoBufferManager::Instance()->Load2DTexture("TerrainDefault.png", "TerrainDefault.png");
	mDefaultNormalMap = VideoBufferManager::Instance()->Load2DTexture("TerrainDefault_n.dds", "TerrainDefault_n.dds");

	for (int i = 0; i < kMaxLayers; ++i)
	{
		mDetailMaps[i] = mDefaultDetailMap;
		mNormalMaps[i] = mDefaultNormalMap;
	}

	// create weight maps
	mWeightMaps.Resize(mConfig.iSectionCount);

	index = 0;
	for (int i = 0; i < mConfig.zSectionCount; ++i)
	{
		for (int j = 0; j < mConfig.zSectionCount; ++j)
		{
			TString128 texName = TString128("TWeightMap_") + i + "_" + j; 
			TexturePtr texture = VideoBufferManager::Instance()->CreateTexture(texName, kWeightMapSize, kWeightMapSize, 5);

			LockedBox lb;
			texture->Lock(0, &lb, NULL, LOCK_NORMAL);
			Color * clr = (Color *)lb.pData;

			for (int m = 0; m < kWeightMapSize; ++m)
				for (int n = 0; n < kWeightMapSize; ++n)
					*clr++ = Color(0, 0, 0, 255);

			texture->Unlock(0);

			mWeightMaps[index++] = texture;
		}
	}

	mBound.minimum = Vec3(0, 0, 0);
	mBound.maximum = Vec3(mConfig.xSize, 0, mConfig.zSize);
}

void Terrain::Load(const char * source)
{
}

int	Terrain::AddLayer(const Layer & layer)
{
	for (int i = 0; i < kMaxLayers; ++i)
	{
		if (mLayer[i].detail == "")
		{
			mLayer[i] = layer;

			mDetailMaps[i] = VideoBufferManager::Instance()->Load2DTexture(layer.detail, layer.detail);
			mNormalMaps[i] = VideoBufferManager::Instance()->Load2DTexture(layer.normal, layer.normal);

			return i;
		}
	}

	return -1;
}

void Terrain::RemoveLayer(int layer)
{
	d_assert (layer < kMaxLayers);

	mLayer[layer].detail = "";
	mLayer[layer].normal = "";
	mLayer[layer].specular = "";
	mLayer[layer].material = -1;
	mLayer[layer].scale = 1;

	mDetailMaps[layer] = mDefaultDetailMap;
	mNormalMaps[layer] = mDefaultNormalMap;
}

TerrainSection * Terrain::GetSection(int x, int z)
{
	d_assert (x < mConfig.xSectionCount && z < mConfig.zSectionCount);

	return mSections[z * mConfig.zSectionCount + x];
}

TexturePtr Terrain::GetWeightMap(int x, int z)
{
	d_assert (x < mConfig.xSectionCount && z < mConfig.zSectionCount);

	return mWeightMaps[z * mConfig.zSectionCount + x];
}

float Terrain::GetHeight(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	return mHeights[z * mConfig.zVertexCount + x];
}

Vec3 Terrain::GetNormal(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	return mNormals[z * mConfig.zVertexCount + x];
}

void Terrain::OnPreVisibleCull(void * data)
{
	mVisibleSections.Clear();
}

TexturePtr Terrain::_getDetailMap(int layer)
{
	if (layer >= 0 && layer < kMaxLayers)
		return mDetailMaps[layer];
	else
		return mDefaultDetailMap;
}

TexturePtr Terrain::_getNormalMap(int layer)
{
	if (layer >= 0 && layer < kMaxLayers)
		return mNormalMaps[layer];
	else
		return mDefaultNormalMap;
}

TexturePtr Terrain::_getSpecularMap(int layer)
{
	if (layer >= 0 && layer < kMaxLayers)
		return mSpecularMaps[layer];
	else
		return mDefaultSpecularMap;
}

void Terrain::Render()
{
    RenderSystem * render = RenderSystem::Instance();

    for (int i = 0; i < mVisibleSections.Size(); ++i)
    {
        mVisibleSections[i]->UpdateLod();
    }

	ShaderParam * uTransform = mTech->GetVertexShaderParamTable()->GetParam("gTransform");
	ShaderParam * uUVParam = mTech->GetVertexShaderParamTable()->GetParam("gUVParam");
	ShaderParam * uUVScale = mTech->GetVertexShaderParamTable()->GetParam("gUVScale");
	ShaderParam * uMorph = mTech->GetVertexShaderParamTable()->GetParam("gMorph");

	float xInvSectionSize = 1 / mConfig.xSectionSize;
	float zInvSectionSize = 1 / mConfig.zSectionSize;
	float xInvSize = 1 / mConfig.xSize;
	float zInvSize = 1 / mConfig.zSize;

	for (int i = 0; i < mVisibleSections.Size(); ++i)
	{
		TerrainSection * section = mVisibleSections[i];
		int x = section->GetSectionX();
		int z = section->GetSectionZ();
		int layer0 = section->GetLayer(0);
		int layer1 = section->GetLayer(1);
		int layer2 = section->GetLayer(2);
		int layer3 = section->GetLayer(3);
		float xOff = section->GetOffX();
		float zOff = section->GetOffZ();

		TexturePtr weightMap = GetWeightMap(x, z);
		TexturePtr detailMap0 = _getDetailMap(layer0);
		TexturePtr detailMap1 = _getDetailMap(layer1);
		TexturePtr detailMap2 = _getDetailMap(layer2);
		TexturePtr detailMap3 = _getDetailMap(layer3);

		float uvScale0 = mLayer[layer0].scale;
		float uvScale1 = mLayer[layer1].scale;
		float uvScale2 = mLayer[layer2].scale;
		float uvScale3 = mLayer[layer3].scale;

		SamplerState state;
		state.Address = TEXA_CLAMP;
		render->SetTexture(0, state, weightMap.c_ptr());

		state.Address = TEXA_WRAP;
		render->SetTexture(1, state, detailMap0.c_ptr());
		render->SetTexture(2, state, detailMap1.c_ptr());
		render->SetTexture(3, state, detailMap2.c_ptr());
		render->SetTexture(4, state, detailMap3.c_ptr());

		uTransform->SetUnifom(xOff, 0, zOff, 0);
		uUVParam->SetUnifom(xInvSectionSize, zInvSectionSize, xInvSize, zInvSize);
		uUVScale->SetUnifom(uvScale0, uvScale1, uvScale2, uvScale3);

		section->PreRender();

		render->Render(mTech, &section->mRender);
	}
}

void Terrain::RenderInMirror()
{
}


Vec3 Terrain::GetPosition(const Ray & ray)
{
	Vec3 result(0, 0, 0);
	const int iMaxCount = 1000;

	int i = 0;
	Vec3 pos = ray.origin;
	float y = 0;

	if ((ray.origin.x < mBound.minimum.x || ray.origin.x > mBound.maximum.x) ||
		(ray.origin.y < mBound.minimum.y || ray.origin.y > mBound.maximum.y) ||
		(ray.origin.z < mBound.minimum.z || ray.origin.z > mBound.maximum.z))
	{
		RayIntersectionInfo info = ray.Intersection(mBound);

		if (!info.iterscetion)
			return result;

		pos = ray.origin + (info.distance + 0.1f) * ray.direction;
	}

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

	return result;
}

float Terrain::GetHeight(float x, float z)
{
	float sx = 0, sz = mConfig.zSize;
	float ex = mConfig.zSize, ez = 0;

	float fx = (x - sx) / (ex - sx) * (mConfig.xVertexCount - 1);
	float fz = (z - sz) / (ez - sz) * (mConfig.zVertexCount - 1);

	int ix = (int) fx;
	int iz = (int) fz;

	d_assert(ix >= 0 && ix <= mConfig.xVertexCount - 1 &&
			 iz >= 0 && iz <= mConfig.zVertexCount - 1);

	float dx = fx - ix;
	float dz = fz - iz;

	int ix1 = ix + 1;
	int iz1 = iz + 1;

	ix1 = Math::Minimum(ix1, mConfig.xVertexCount);
	iz1 = Math::Minimum(iz1, mConfig.zVertexCount);

	float a = GetHeight(ix,  iz);
	float b = GetHeight(ix1, iz);
	float c = GetHeight(ix,  iz1);
	float d = GetHeight(ix1, iz1);
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

}