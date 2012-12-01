#include "MWTerrain.h"
#include "MWTerrainLod.h"
#include "MWTerrainSection.h"
#include "MWWorld.h"
#include "MWImage.h"
#include "MWResourceManager.h"
#include "MWEnvironment.h"
#include "MWRenderEvent.h"
#include "MWRenderHelper.h"

namespace Myway {

const int K_Terrain_Version = 0;
const int K_Terrain_Magic = 'TRN0';

Terrain::Terrain(const Config & config)
	: tOnPreVisibleCull(&RenderEvent::OnPreVisibleCull, this, &Terrain::OnPreVisibleCull)
{
	mInited = false;
	mLockedData = NULL;
	mLockedWeightMapData = NULL;

	mLod = new TerrainLod(kMaxDetailLevel);
	mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Terrain");

	_Create(config);
}

Terrain::Terrain(const char * source)
	: tOnPreVisibleCull(&RenderEvent::OnPreVisibleCull, this, &Terrain::OnPreVisibleCull)
{
	mInited = false;
	mLockedData = NULL;
	mLockedWeightMapData = NULL;

	mLod = new TerrainLod(kMaxDetailLevel);
	mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Terrain");

	_Load(source);
}


Terrain::~Terrain()
{
	d_assert(mLockedData == NULL);
	d_assert(mLockedWeightMapData == NULL);

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
	safe_delete_array(mWeights);
}

void Terrain::_init()
{
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

	_calcuNormals();

	// create sections
	mSections.Resize(mConfig.xSectionCount * mConfig.zSectionCount);
	mSceneNodes.Resize(mConfig.xSectionCount * mConfig.zSectionCount);

	int index = 0;
	for (int j = 0; j < mConfig.zSectionCount; ++j)
	{
		for (int i = 0; i < mConfig.xSectionCount; ++i)
		{
			mSections[index] = new TerrainSection(this, i, j);
			mSceneNodes[index] = World::Instance()->CreateSceneNode();

			mSceneNodes[index]->Attach(mSections[index]);

			++index;
		}
	}

	// create weight map.
	mWeightMaps.Resize(mConfig.iSectionCount);

	index = 0;
	
	for (int j = 0; j < mConfig.zSectionCount; ++j)
	{
		for (int i = 0; i < mConfig.xSectionCount; ++i)
		{

			TString128 texName = TString128("TWeightMap_") + i + "_" + j; 
			TexturePtr texture = VideoBufferManager::Instance()->CreateTexture(texName, kWeightMapSize, kWeightMapSize, 0, FMT_A8R8G8B8);

			LockedBox lb;
			texture->Lock(0, &lb, NULL, LOCK_NORMAL);

			int * clr = (int *)lb.pData;
			Color * weights = mWeights + j * kWeightMapSize * mConfig.xWeightMapSize + i * kWeightMapSize;

			for (int m = 0; m < kWeightMapSize; ++m)
			{
				for (int n = 0; n < kWeightMapSize; ++n)
				{
					*clr++ = RGBA(weights[n].r, weights[n].g, weights[n].b, weights[n].a);
				}

				weights += kWeightMapSize;
			}

			texture->Unlock(0);

			mWeightMaps[index++] = texture;
		}
	}

	// load default detail map
	mDefaultDetailMap = VideoBufferManager::Instance()->Load2DTexture("TerrainDefault.png", "TerrainDefault.png");
	mDefaultNormalMap = VideoBufferManager::Instance()->Load2DTexture("TerrainDefault_n.dds", "TerrainDefault_n.dds");

	for (int i = 0; i < kMaxLayers; ++i)
	{
		mDetailMaps[i] = mDefaultDetailMap;
		mNormalMaps[i] = mDefaultNormalMap;
		mSpecularMaps[i] = RenderHelper::Instance()->GetWhiteTexture();
	}

	for (int i = 0; i < kMaxLayers; ++i)
	{
		if (mLayer[i].detail != "")
			mDetailMaps[i] = VideoBufferManager::Instance()->Load2DTexture(mLayer[i].detail, mLayer[i].detail);

		if (mLayer[i].normal != "")
			mNormalMaps[i] = VideoBufferManager::Instance()->Load2DTexture(mLayer[i].normal, mLayer[i].normal);

		if (mLayer[i].specular != "")
			mSpecularMaps[i] = VideoBufferManager::Instance()->Load2DTexture(mLayer[i].specular, mLayer[i].specular);
	}
}

void Terrain::_calcuNormals()
{
	d_assert (mNormals == NULL);

	mNormals = new Vec3[mConfig.iVertexCount];

	for (int j = 0; j < mConfig.zVertexCount; ++j)
	{
		for (int i = 0; i < mConfig.xVertexCount; ++i)
		{
			Vec3 a = _getPosition(i - 1, j + 0);
			Vec3 b = _getPosition(i + 0, j - 1);
			Vec3 c = _getPosition(i + 1, j + 0);
			Vec3 d = _getPosition(i + 0, j + 1);
			Vec3 p = _getPosition(i + 0, j + 0);

			Vec3 L = a - p, T = b - p, R = c - p, B = d - p;

			Vec3 N = Vec3::Zero;
			float len_L = L.Length(), len_T = T.Length();
			float len_R = R.Length(), len_B = B.Length();

			if (len_L > 0.01f && len_T > 0.01f)
				N += L.CrossN(T);

			if (len_T > 0.01f && len_R > 0.01f)
				N += T.CrossN(R);

			if (len_R > 0.01f && len_B > 0.01f)
				N += R.CrossN(B);

			if (len_B > 0.01f && len_L > 0.01f)
				N += B.CrossN(L);

			mNormals[j * mConfig.xVertexCount + i] = N.Normalize();
		}
	}
}

void Terrain::_Create(const Config & config)
{
	d_assert (!mInited);

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

	// init default heights
	mHeights = new float[mConfig.iVertexCount];

	for (int i = 0; i < mConfig.iVertexCount; ++i)
		mHeights[i] = 0;

	// create weight maps
	mConfig.xWeightMapSize = Terrain::kWeightMapSize * mConfig.xSectionCount;
	mConfig.zWeightMapSize = Terrain::kWeightMapSize * mConfig.zSectionCount;
	mConfig.iWeightMapSize = mConfig.xWeightMapSize * mConfig.zWeightMapSize;

	mWeights = new Color[mConfig.xWeightMapSize * mConfig.zWeightMapSize];
	for (int i = 0; i < mConfig.xWeightMapSize * mConfig.zWeightMapSize; ++i)
	{
		mWeights[i] = Color(0, 0, 0, 255);
	}

	mBound.minimum = Vec3(0, 0, 0);
	mBound.maximum = Vec3(mConfig.xSize, 0, mConfig.zSize);

	_init();

	mInited = true;
}

void Terrain::_Load(const char * filename)
{
	d_assert (!mInited);

	DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);

	d_assert (stream != NULL);

	int magic, version;

	stream->Read(&magic, sizeof(int));

	d_assert (magic == K_Terrain_Magic);

	stream->Read(&version, sizeof(int));

	if (version == 0)
	{
		stream->Read(&mConfig, sizeof(Config));
		stream->Read(mLayer, sizeof(Layer) * kMaxLayers);

		mHeights = new float[mConfig.iVertexCount];

		stream->Read(mHeights, sizeof(float) * mConfig.iVertexCount);

		mWeights = new Color[mConfig.iWeightMapSize];

		stream->Read(mWeights, sizeof(Color) * mConfig.iWeightMapSize);

		stream->Read(&mBound, sizeof(Aabb));
	}

	_init();

	mInited = true;
}

void Terrain::Save(const char * filename)
{
	File file;

	file.Open(filename);

	file.Write(&K_Terrain_Magic, sizeof(int));
	file.Write(&K_Terrain_Version, sizeof(int));

	// write config
	file.Write(&mConfig, sizeof(Config));

	// write layers
	file.Write(mLayer, sizeof(Layer) * kMaxLayers);

	// write heights
	file.Write(mHeights, sizeof(float) * mConfig.iVertexCount);
	
	// write weights
	file.Write(mWeights, sizeof(Color) * mConfig.iWeightMapSize);

	// write bound
	file.Write(&mBound, sizeof(Aabb));

	file.Close();
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
			mSpecularMaps[i] = VideoBufferManager::Instance()->Load2DTexture(layer.specular, layer.specular);

			return i;
		}
	}

	return -1;
}

const Terrain::Layer * Terrain::GetLayer(int index)
{
	d_assert (index < kMaxLayers);

	return &mLayer[index];
}

void Terrain::SetLayer(int index, const Layer & layer)
{
	d_assert (index < kMaxLayers);

	Layer & oldLayer = mLayer[index];

	if (oldLayer.detail != layer.detail)
		mDetailMaps[index] = VideoBufferManager::Instance()->Load2DTexture(layer.detail, layer.detail);

	if (oldLayer.normal != layer.normal)
		mNormalMaps[index] = VideoBufferManager::Instance()->Load2DTexture(layer.normal, layer.normal);

	if (oldLayer.specular != layer.specular)
		mSpecularMaps[index] = VideoBufferManager::Instance()->Load2DTexture(layer.specular, layer.specular);

	oldLayer = layer;
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
	mSpecularMaps[layer] = RenderHelper::Instance()->GetWhiteTexture();
}

TerrainSection * Terrain::GetSection(int x, int z)
{
	d_assert (x < mConfig.xSectionCount && z < mConfig.zSectionCount);

	return mSections[z * mConfig.xSectionCount + x];
}

TexturePtr Terrain::GetWeightMap(int x, int z)
{
	d_assert (x < mConfig.xSectionCount && z < mConfig.zSectionCount);

	return mWeightMaps[z * mConfig.xSectionCount + x];
}

Vec3 Terrain::GetPosition(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	float fx = (float)x / (mConfig.xVertexCount - 1) * mConfig.xSize;
	float fz = (1 - (float)z / (mConfig.zVertexCount - 1)) * mConfig.zSize;
	float fy = GetHeight(x, z);

	return Vec3(fx, fy, fz);
}

float Terrain::GetHeight(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	return mHeights[z * mConfig.xVertexCount + x];
}

Vec3 Terrain::GetNormal(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	return mNormals[z * mConfig.xVertexCount + x];
}

Color Terrain::GetWeight(int x, int z)
{
	d_assert (x < mConfig.xWeightMapSize && z < mConfig.zWeightMapSize);

	return mWeights[z * mConfig.xWeightMapSize + x];
}

void Terrain::OnPreVisibleCull(void * param0, void * param1)
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

Vec3 Terrain::_getPosition(int x, int z)
{
	x = Math::Maximum(0, x);
	z = Math::Maximum(0, z);

	x = Math::Minimum(x, mConfig.xVertexCount - 1);
	z = Math::Minimum(z, mConfig.zVertexCount - 1);

	return GetPosition(x, z);
}

void Terrain::Render()l
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

		layer0 = Math::Maximum(0, layer0);
		layer1 = Math::Maximum(0, layer1);
		layer2 = Math::Maximum(0, layer2);
		layer3 = Math::Maximum(0, layer3);

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

float *	Terrain::LockHeight(const Rect & rc)
{
	d_assert (mLockedData == NULL);

	int w = rc.x2 - rc.x1 + 1;
	int h = rc.y2 - rc.y1 + 1;

	d_assert (w > 0 && h > 0);

	mLockedData = new float[w * h];

	int index = 0;
	for (int j = rc.y1; j <= rc.y2; ++j)
	{
		for (int i = rc.x1; i <= rc.x2; ++i)
		{
			mLockedData[index++] = GetHeight(i, j);
		}
	}

	mLockedRect = rc;

	return mLockedData;
}

void Terrain::UnlockHeight()
{
	d_assert (mLockedData != NULL);

	int index = 0;
	for (int j = mLockedRect.y1; j <= mLockedRect.y2; ++j)
	{
		for (int i = mLockedRect.x1; i <= mLockedRect.x2; ++i)
		{
			mHeights[j * mConfig.xVertexCount + i] = mLockedData[index++];
		}
	}

	// need re-calculate normals
	Rect rcNormal = mLockedRect;
	rcNormal.x1 -= 1;
	rcNormal.x2 += 1;
	rcNormal.y1 -= 1;
	rcNormal.y2 += 1;

	rcNormal.x1 = Math::Maximum(0, rcNormal.x1);
	rcNormal.y1 = Math::Maximum(0, rcNormal.y1);
	rcNormal.x2 = Math::Minimum(mConfig.xVertexCount - 1, rcNormal.x2);
	rcNormal.y2 = Math::Minimum(mConfig.zVertexCount - 1, rcNormal.y2);

	for (int j = rcNormal.y1; j < rcNormal.y2; ++j)
	{
		for (int i = rcNormal.x1; i < rcNormal.x2; ++i)
		{
			Vec3 a = _getPosition(i - 1, j + 0);
			Vec3 b = _getPosition(i + 0, j - 1);
			Vec3 c = _getPosition(i + 1, j + 0);
			Vec3 d = _getPosition(i + 0, j + 1);
			Vec3 p = _getPosition(i + 0, j + 0);

			Vec3 L = a - p, T = b - p, R = c - p, B = d - p;

			Vec3 N = Vec3::Zero;
			float len_L = L.Length(), len_T = T.Length();
			float len_R = R.Length(), len_B = B.Length();

			if (len_L > 0.01f && len_T > 0.01f)
				N += L.CrossN(T);

			if (len_T > 0.01f && len_R > 0.01f)
				N += T.CrossN(R);

			if (len_R > 0.01f && len_B > 0.01f)
				N += R.CrossN(B);

			if (len_B > 0.01f && len_L > 0.01f)
				N += B.CrossN(L);

			mNormals[j * mConfig.xVertexCount + i] = N.Normalize();
		}
	}

	// update sections
	for (int i = 0; i < mSections.Size(); ++i)
	{
		mSections[i]->NotifyUnlockHeight();
	}

	// update bound
	index = 0;
	for (int j = mLockedRect.y1; j <= mLockedRect.y2; ++j)
	{
		for (int i = mLockedRect.x1; i <= mLockedRect.x2; ++i)
		{
			float h =mLockedData[index++];
			mBound.minimum.y = Math::Minimum(mBound.minimum.y, h);
			mBound.maximum.y = Math::Maximum(mBound.maximum.y, h);
		}
	}

	safe_delete_array(mLockedData);
}

float * Terrain::LockWeightMap(const Rect & rc)
{
	d_assert (!IsLockedWeightMap());

	int w = rc.x2 - rc.x1 + 1;
	int h = rc.y2 - rc.y1 + 1;

	d_assert (w > 0 && h > 0);

	mLockedWeightMapData = new float[w * h];

	int index = 0;
	for (int j = rc.y1; j <= rc.y2; ++j)
	{
		for (int i = rc.x1; i <= rc.x2; ++i)
		{
			mLockedWeightMapData[index++] = 0;
		}
	}

	mLockedWeightMapRect = rc;

	return mLockedWeightMapData;
}

void Terrain::UnlockWeightMap(int layer)
{
	d_assert (IsLockedWeightMap());

	int index = 0;
	for (int j = mLockedWeightMapRect.y1; j <= mLockedWeightMapRect.y2; ++j)
	{
		for (int i = mLockedWeightMapRect.x1; i <= mLockedWeightMapRect.x2; ++i)
		{
			float weight = mLockedWeightMapData[index++];
			int xSection = i / kWeightMapSize;
			int zSection = j / kWeightMapSize;

			TerrainSection * section = GetSection(xSection, zSection);

			int layer0 = section->GetLayer(0);
			int layer1 = section->GetLayer(1);
			int layer2 = section->GetLayer(2);
			int layer3 = section->GetLayer(3);

			Color c = mWeights[j * mConfig.xWeightMapSize + i];
			Color4 c4;

			c4.a = c.a / 255.0f;
			c4.r = c.r / 255.0f;
			c4.g = c.g / 255.0f;
			c4.b = c.b / 255.0f;

			if (layer == layer0)
				c4.a += weight;
			else if (layer == layer1)
				c4.r += weight;
			else if (layer == layer2)
				c4.g += weight;
			else if (layer == layer3)
				c4.b += weight;
			else
			{
				if (layer0 == -1)
				{
					c4.a += weight;
					section->SetLayer(0, layer);
				}
				else if (layer1 == -1)
				{
					c4.r += weight;
					section->SetLayer(1, layer);
				}
				else if (layer2 == -1)
				{
					c4.g += weight;
					section->SetLayer(2, layer);
				}
				else if (layer3 == -1)
				{
					c4.b += weight;
					section->SetLayer(3, layer);
				}
			}

			c4 = c4.Normalize();

			c.r = unsigned char(c4.r * 255);
			c.g = unsigned char(c4.g * 255);
			c.b = unsigned char(c4.b * 255);
			c.a = unsigned char(c4.a * 255);

			mWeights[j * mConfig.xWeightMapSize + i] = c;
		}
	}

	// update weight map
	for (int i = 0; i < mSections.Size(); ++i)
	{
		TerrainSection * section = mSections[i];

		int xtile = Terrain::kWeightMapSize;
		int ztile = Terrain::kWeightMapSize;
		int x = section->GetSectionX();
		int z = section->GetSectionZ();

		Rect myRect;

		myRect.x1 = x * xtile;
		myRect.y1 = z * ztile;
		myRect.x2 = x * xtile + xtile;
		myRect.y2 = z * ztile + ztile;

		if (mLockedWeightMapRect.x1 > myRect.x2 || mLockedWeightMapRect.x2 < myRect.x1 ||
			mLockedWeightMapRect.y1 > myRect.y2 || mLockedWeightMapRect.y2 < myRect.y1)
			continue ;

		TexturePtr weightMap = GetWeightMap(x, z);

		LockedBox lb;
		weightMap->Lock(0, &lb, NULL, LOCK_DISCARD);

		Color * data = mWeights + z * kWeightMapSize * mConfig.xWeightMapSize + x * kWeightMapSize;
		int * dest = (int *)lb.pData;
		for (int k = 0; k < kWeightMapSize; ++k)
		{
			for (int p = 0; p < kWeightMapSize; ++p)
				dest[p] = RGBA(data[p].r, data[p].g, data[p].b, data[p].a);

			dest += kWeightMapSize;
			data += mConfig.xWeightMapSize;
		}

		weightMap->Unlock(0);
	}

	safe_delete_array(mLockedWeightMapData);
}

}