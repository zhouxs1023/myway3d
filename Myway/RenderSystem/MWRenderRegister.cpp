#include "MWRenderRegister.h"

using namespace Myway;

IMP_SLN(RenderRegister);

RenderRegister::RenderRegister()
{
    INIT_SLN;

    mWorldMatrix = Mat4::Identity;

    mViewMatrix = Mat4::Identity;
    mProjMatrix = Mat4::Identity;
    mWorldViewMatrix = Mat4::Identity;
    mViewProjMatrix = Mat4::Identity;
    mWorldViewProjMatrix = Mat4::Identity;
   
    mNumBlendMatrices = 0;
    for (int i = 0; i < MAX_BLEND_MATRIX; ++i)
        mBlendMatrices[i] = Mat4::Identity;

    //texture
    for (int i = 0; i < MAX_SAMPLER_STATE; ++i)
    {
        mTextureSize[i] = Vec4::Zero;
        mInvTextureSize[i] = Vec4::Zero;
        mTextureMatrix[i] = Mat4::Identity;
    }

    //fog
    mFogParam = Vec4(10.0f, 300.0f, 1.0f, 0.0f);
    mFogColor = Color4::White;

    //material
    mMtrlAmbient = Color4::White;
    mMtrlEmissive = Color4::Black;
    mMtrlDiffuse = Color4::White;
    mMtrlSpecular = Color4::Black;

    //light
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        mLightDiffuse[i] = Color4::Black;
        mLightSpecular[i] = Color4::Black;
        mLightPosition[i] = Vec4::Zero;
        mLightDirection[i] = Vec4::UnitZ;
        mLightAttenuation[i] = Vec4(-2, 1, 0, 0);
        mLightSpotParams[i] = Vec4(1, 0, 0, 1);
    }
    
    //camera
    mCameraPosition = Vec4::Zero;

    //time
    mTime = Vec4::Zero;

    //clip plane
    mClipPlane = Vec4(1, 10000, 1, 0.0001f);

    //control
    mNeedUpdateTransfrom = FALSE;
    mNeedUpdateLight = FALSE;
}

RenderRegister::~RenderRegister()
{
    SHUT_SLN;
}

void RenderRegister::Reset()
{
    // transform
    mWorldMatrix = Mat4::Identity;

    for (int i = 0; i < mNumBlendMatrices; ++i)
    {
        mBlendMatrices[i] = Mat4::Identity;
    }

    mNeedUpdateTransfrom = TRUE;
}

void RenderRegister::Begin()
{
    if (mNeedUpdateTransfrom)
    {
        Math::MatMultiply(mWorldViewMatrix, mWorldMatrix, mViewMatrix);
        Math::MatMultiply(mViewProjMatrix, mViewMatrix, mProjMatrix);
        Math::MatMultiply(mWorldViewProjMatrix, mWorldViewMatrix, mProjMatrix);
    }

	mNeedUpdateTransfrom = FALSE;
}

void RenderRegister::End()
{
    //clear light
    if (mNeedUpdateLight)
    {
        for (int i = 0; i < MAX_LIGHTS; ++i)
        {
            mLightDiffuse[i] = Color4::Black;
            mLightSpecular[i] = Color4::Black;
            mLightPosition[i] = Vec4::Zero;
            mLightDirection[i] = Vec4::UnitZ;
            mLightAttenuation[i] = Vec4(-2, 1, 0, 0);
            mLightSpotParams[i] = Vec4(1, 0, 0, 1);
        }

        mNeedUpdateLight = FALSE;
    }

    Memzero(mUserBuffer, sizeof(MAX_USER_BUFFER));
}

void RenderRegister::SetWorldMatrix(const Mat4 & m)
{
    mWorldMatrix = m;
    mNeedUpdateTransfrom = TRUE;
}

void RenderRegister::SetViewMatrix(const Mat4 & m)
{
    mViewMatrix = m;
    mNeedUpdateTransfrom = TRUE;
}

void RenderRegister::SetProjMatrix(const Mat4 & m)
{
    mProjMatrix = m;
    mNeedUpdateTransfrom = TRUE;
}

void RenderRegister::SetTextureMatrix(int index, const Mat4 & m)
{
    d_assert(index < MAX_SAMPLER_STATE);
    mTextureMatrix[index] = m;
}

void RenderRegister::SetBlendMatrix(const Mat4 * m, int count)
{
    d_assert(count < MAX_BLEND_MATRIX);
    Memcpy(mBlendMatrices, m, count * sizeof(Mat4));
    mNumBlendMatrices = count;
}

void RenderRegister::SetTextureSize(int index, const Vec4 & size)
{
    mTextureSize[index] = size;
    mInvTextureSize[index] = 1.0f / size;
}

const Vec4 & RenderRegister::GetTextureSize(int index)
{
    return mTextureSize[index];
}

const Vec4 & RenderRegister::GetInvTextureSize(int index)
{
    return mInvTextureSize[index];
}

void RenderRegister::SetMaterial(const Color4 & ambient, 
                                 const Color4 & emissve,
                                 const Color4 & diffuse,
                                 const Color4 & specular,
                                 float power)
{
    mMtrlAmbient = ambient;
    mMtrlEmissive = emissve;
    mMtrlDiffuse = diffuse;
    mMtrlSpecular = specular;
    mMtrlSpecular.a = power;
}

void RenderRegister::SetLightDiffuse(int index, const Color4 & c)
{
    d_assert(index < MAX_LIGHTS);

    mLightDiffuse[index] = c;
    mNeedUpdateLight = TRUE;
}

void RenderRegister::SetLightSpecular(int index, const Color4 & c)
{
    d_assert(index < MAX_LIGHTS);

    mLightSpecular[index] = c;
    mNeedUpdateLight = TRUE;
}

void RenderRegister::SetLightPosition(int index, const Vec4 & pos)
{
    d_assert(index < MAX_LIGHTS);

    mLightPosition[index] = pos;
    mNeedUpdateLight = TRUE;
}

void RenderRegister::SetLightDirection(int index, const Vec4 & dir)
{
    d_assert(index < MAX_LIGHTS);

    mLightDirection[index] = dir;
    mNeedUpdateLight = TRUE;
}

void RenderRegister::SetLightAttenuation(int index, float range, float a0, float a1, float a2)
{
    d_assert(index < MAX_LIGHTS);

    mLightAttenuation[index].x = range;
    mLightAttenuation[index].y = a0;
    mLightAttenuation[index].z = a1;
    mLightAttenuation[index].w = a2;
    mNeedUpdateLight = TRUE;
}

void RenderRegister::SetLightSpotParams(int index, float inner, float outer, float falloff)
{
    d_assert(index < MAX_LIGHTS);

    mLightSpotParams[index].x = inner;
    mLightSpotParams[index].y = outer;
    mLightSpotParams[index].z = falloff;
    mLightSpotParams[index].w = 1.0f;
    mNeedUpdateLight = TRUE;
}

const Mat4 & RenderRegister::GetWorldMatrix()
{
    return mWorldMatrix;
}

const Mat4 & RenderRegister::GetViewMatrix()
{
    return mViewMatrix;
}

const Mat4 & RenderRegister::GetProjMatrix()
{
    return mProjMatrix;
}

const Mat4 & RenderRegister::GetWorldViewMatrix()
{
    return mWorldViewMatrix;
}

const Mat4 & RenderRegister::GetViewProjMatrix()
{
    return mViewProjMatrix;
}

const Mat4 & RenderRegister::GetWorldViewProjMatrix()
{
    return mWorldViewProjMatrix;
}

const Mat4 & RenderRegister::GetBlendMatrix(int index)
{
    d_assert(index < 256);
    return mBlendMatrices[index];
}

const Mat4 * RenderRegister::GetBlendMatrices()
{
    return mBlendMatrices;
}

int RenderRegister::GetNumBlendMatrices()
{
    return mNumBlendMatrices;
}


void RenderRegister::SetFogParam(const Vec4 & param)
{
    mFogParam = param;
}

void RenderRegister::SetFogColor(const Color4 & color)
{
    mFogColor = color;
}

const Vec4 & RenderRegister::GetFogParam()
{
    return mFogParam;
}

const Color4 & RenderRegister::GetFogColor()
{
    return mFogColor;
}


const Color4 & RenderRegister::GetMaterialAmbient()
{
    return mMtrlAmbient;
}

const Color4 & RenderRegister::GetMaterialEmissive()
{
    return mMtrlEmissive;
}

const Color4 & RenderRegister::GetMaterialDiffuse()
{
    return mMtrlDiffuse;
}

const Color4 & RenderRegister::GetMaterialSpecular()
{
    return mMtrlSpecular;
}

const Color4 & RenderRegister::GetLightDiffuse(int index)
{
    d_assert(index < MAX_LIGHTS);

    return mLightDiffuse[index];
}

const Color4 & RenderRegister::GetLightSpecular(int index)
{
    d_assert(index < MAX_LIGHTS);

    return mLightSpecular[index];
}

const Vec4 & RenderRegister::GetLightPosition(int index)
{
    d_assert(index < MAX_LIGHTS);

    return mLightPosition[index];
}

const Vec4 & RenderRegister::GetLightDirection(int index)
{
    d_assert(index < MAX_LIGHTS);

    return mLightDirection[index];
}

const Vec4 & RenderRegister::GetLightAttenuation(int index)
{
    d_assert(index < MAX_LIGHTS);

    return mLightAttenuation[index];
}

const Vec4 & RenderRegister::GetLightSpotParams(int index)
{
    d_assert(index < MAX_LIGHTS);

    return mLightSpotParams[index];
}

void RenderRegister::SetCameraPosition(const Vec4 & pos)
{
    mCameraPosition = pos;
}


const Vec4 & RenderRegister::GetCameraPosition()
{
    return mCameraPosition;
}


//Time
void RenderRegister::SetTime(float time)
{
    mTime.x = time;
    Math::SinCos(time, mTime.y, mTime.z);
    mTime.w = 1;
}

const Vec4 & RenderRegister::GetTime()
{
    return mTime;
}

void RenderRegister::SetClipPlane(float nearClip, float farClip)
{
    mClipPlane.x = nearClip; mClipPlane.y = farClip;
    mClipPlane.z = 1 / nearClip; mClipPlane.w = 1 / farClip;
}

const Vec4 & RenderRegister::GetClipPlane()
{
    return mClipPlane;
}

void RenderRegister::SetUserData(int offset, const void * data, int size)
{
    d_assert (offset + size < MAX_USER_BUFFER);

    Memcpy(&mUserBuffer[offset], data, size);
}

void RenderRegister::GetUserData(int offset, void * data, int size)
{
    d_assert (offset + size < MAX_USER_BUFFER && data);
    Memcpy(data, &mUserBuffer[offset], size);
}

const void * RenderRegister::GetUserData(int offset)
{
    d_assert (offset < MAX_USER_BUFFER);
    return &mUserBuffer[offset];
}