#include "MWShaderParam.h"
#include "MWRenderRegister.h"

using namespace Myway;

int ShaderParam::GetTypeSize(SHADER_PARAM_TYPE type)
{
    // always packed.(16 bytes aligned)
    switch (type)
    {
    case SPT_INT4:
    case SPT_FLOAT4:
        return 16;

    case SPT_MATRIX3x4:
        return 48;

    case SPT_MATRIX4x4:
        return 64;

    default:
        return 0;
    }
}

ShaderParam::ShaderParam()
: mParent(NULL)
, mType(SPT_FLOAT4)
, mBType(SPBT_UNKNOWN)
, mBIndex(0)
, mCount(0)
, mPosition(0)
, mRegister(0)
, mSize(0)
{
}

ShaderParam::~ShaderParam()
{
}

void ShaderParam::SetUnifom(float x, float y, float z, float w)
{
    SetVector(Vec4(x, y, z, w));
}

void ShaderParam::SetFloat(float x)
{
    d_assert (mType == SPT_FLOAT4);
    mParent->SetData(mPosition, &x, 4);
}

void ShaderParam::SetVector(const Vec4 & v)
{
    d_assert (mType == SPT_FLOAT4);

    mParent->SetData(mPosition, &v, 16);
}

void ShaderParam::SetMatrix(const Mat4 & m)
{
    d_assert (mType == SPT_MATRIX4x4);

    mParent->SetData(mPosition, &m, 64);
}

void ShaderParam::SetColor(const Color4 & c)
{
    mParent->SetData(mPosition, &c, 16);
}


void ShaderParam::SetVector(const Vec4 * v, int count)
{
    d_assert (mType == SPT_FLOAT4 && count <= mCount);

    mParent->SetData(mPosition, v, 16 * count);
}

void ShaderParam::SetMatrix(const Mat4 * m, int count)
{
    d_assert (mType == SPT_MATRIX4x4 && count <= mCount);

    mParent->SetData(mPosition, m, 64 * count);
}

void ShaderParam::SetMatrix(const Mat3x4 * m, int count)
{
	d_assert (mType == SPT_MATRIX3x4);

	mParent->SetData(mPosition, m, 48 * count);
}


void ShaderParam::SetColor(const Color4 * c, int count)
{
    d_assert (mType == SPT_FLOAT4 && count <= mCount);

    mParent->SetData(mPosition, c, 16 * count);
}

void ShaderParam::Set(const void * data, int size)
{
    d_assert (size <= mSize);

    mParent->SetData(mPosition, data, size);
}

void ShaderParam::Get(void * data, int size) const
{
    d_assert (size <= mSize);

    mParent->GetData(mPosition, data, size);
}

const void * ShaderParam::GetData() const
{
    return mParent->GetData(mPosition);
}


void ShaderParam::SetBindType(SHADER_PARAM_BIND_TYPE type)
{
    mBType = type;
}

void ShaderParam::SetBindIndex(int index)
{
    mBIndex = index;
}

SHADER_PARAM_TYPE ShaderParam::GetType() const
{
    return mType;
}

SHADER_PARAM_BIND_TYPE ShaderParam::GetBindType() const
{
    return mBType;
}

int ShaderParam::GetBindIndex() const
{
    return mBIndex;
}

const TString32 & ShaderParam::GetName() const
{
    return mName;
}

int ShaderParam::GetCount() const
{
    return mCount;
}

int ShaderParam::GetSize() const
{
    return mSize;
}

int ShaderParam::GetPosition() const
{
    return mPosition;
}

int ShaderParam::GetRegister() const
{
    return mRegister;
}

bool ShaderParam::IsInt() const
{
    switch (mType)
    {
    case SPT_INT4:
        return true;

    default:
        return false;
    }
}

bool ShaderParam::IsAuto() const
{
    return mBType != SPBT_UNKNOWN;
}

int ShaderParam::GetVec4Count() const
{
    return mSize / 16;
}




ShaderParamTable::ShaderParamTable()
{
    Clear();
}

ShaderParamTable::~ShaderParamTable()
{
    Clear();
}

ShaderParam * ShaderParamTable::AddParam(const TString32 & name,
                                         SHADER_PARAM_TYPE type,
                                         int count,
                                         int reg,
                                         SHADER_PARAM_BIND_TYPE autotype,
                                         int autoindex)
{
    d_assert (!GetParam(name));

    ShaderParam * param = mParams.Increase();

    param->mParent = this;
    param->mName = name;
    param->mType = type;
    param->mBType = autotype;
    param->mBIndex = autoindex;
    param->mCount = count;
    param->mSize = ShaderParam::GetTypeSize(type) * count;;
    param->mPosition = mSize;
    param->mRegister = reg;

    mSize += param->mSize;

    d_assert (mSize <= BUFFER_SIZE);

    return param;
}

void ShaderParamTable::Clear()
{
    memset(mBuffer, 0, BUFFER_SIZE);
    mParams.Clear();
    mSize = 0;
}

int ShaderParamTable::GetNumParam() const
{
    return mParams.Size();
}

ShaderParam * ShaderParamTable::GetParam(int index)
{
    d_assert (index < mParams.Size());
    return &mParams[index];
}

ShaderParam * ShaderParamTable::GetParam(const TString32 & name)
{
    for (int i = 0; i < mParams.Size(); ++i)
    {
        if (mParams[i].GetName() == name)
            return &mParams[i];
    }

    return NULL;
}

void ShaderParamTable::SetData(int pos, const void * data, int size)
{
    d_assert (mBuffer && pos + size <= mSize);

    Memcpy(mBuffer + pos, data, size);
}

void ShaderParamTable::GetData(int pos, void * data, int size)
{
    d_assert (mBuffer && pos + size <= mSize);

    Memcpy(data, mBuffer + pos, size);
}

const void * ShaderParamTable::GetData(int pos)
{
    return mBuffer + pos;
}

void ShaderParamTable::Update()
{
    RenderRegister * reg = RenderRegister::Instance();

    for (int i = 0; i < mParams.Size(); ++i)
    {
        ShaderParam * param = &mParams[i];

        if (param->IsAuto())
        {
            int index = param->GetBindIndex();
            //...
            switch (param->GetBindType())
            {
            case SPBT_WORLD_MATRIX:
                param->SetMatrix(reg->GetWorldMatrix());
                break;

            case SPBT_VIEW_MATRIX:
                param->SetMatrix(reg->GetViewMatrix());
                break;

            case SPBT_PROJ_MATRIX:
                param->SetMatrix(reg->GetProjMatrix());
                break;

            case SPBT_WORLD_VIEW_MATRIX:
                param->SetMatrix(reg->GetWorldViewMatrix());
                break;

            case SPBT_VIEW_PROJ_MATRIX:
                param->SetMatrix(reg->GetViewProjMatrix());
                break;

            case SPBT_WORLD_VIEW_PROJ_MATRIX:
                param->SetMatrix(reg->GetWorldViewProjMatrix());
                break;

            case SPBT_BLEND_MATRIX:
                param->SetMatrix(reg->GetBlendMatrices(), param->GetCount());
                break;

            case SPBT_FOG_PARAM:
                param->SetVector(reg->GetFogParam());
                break;

            case SPBT_FOG_COLOR:
                param->SetColor(reg->GetFogColor());
                break;

            case SPBT_MATERIAL_AMBIENT:
                param->SetColor(reg->GetMaterialAmbient());
                break;

            case SPBT_MATERIAL_EMISSIVE:
                param->SetColor(reg->GetMaterialEmissive());
                break;

            case SPBT_MATERIAL_DIFFUSE:
                param->SetColor(reg->GetMaterialDiffuse());
                break;

            case SPBT_MATERIAL_SPECULAR:
                param->SetColor(reg->GetMaterialSpecular());
                break;

            case SPBT_LIGHT_POSITION:
                param->SetVector(reg->GetLightPosition(index));
                break;

            case SPBT_LIGHT_DIRECTION:
                param->SetVector(reg->GetLightDirection(index));
                break;

            case SPBT_LIGHT_ATTENUATION:
                param->SetVector(reg->GetLightAttenuation(index));
                break;

            case SPBT_LIGHT_SPOT_PARAM:
                param->SetVector(reg->GetLightSpotParams(index));
                break;

            case SPBT_TEXTURE_SIZE:
                param->SetVector(reg->GetTextureSize(index));
                break;

            case SPBT_INV_TEXTURE_SIZE:
                param->SetVector(reg->GetInvTextureSize(index));
                break;

            case SPBT_TIME:
                param->SetVector(reg->GetTime());
                break;

            case SPBT_CLIP_PANE:
                param->SetVector(reg->GetClipPlane());
                break;

            case SPBT_USER:
                param->Set(reg->GetUserData(param->GetBindIndex()), param->GetSize());
                break;
            }
        }
    }
}