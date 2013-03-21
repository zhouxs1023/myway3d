#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class MW_ENTRY RenderRegister
{
    DECLARE_SINGLETON (RenderRegister);

public:
                    RenderRegister();
                    ~RenderRegister();

    void            Reset();
    void            Begin();
    void            End();

    void            SetWorldMatrix(const Mat4 & m);
    void            SetViewMatrix(const Mat4 & m);
    void            SetProjMatrix(const Mat4 & m);
    void            SetTextureMatrix(int index, const Mat4 & m);
    void            SetBlendMatrix(const Mat4 * m, int count);

    void            SetTextureSize(int index, const Vec4 & size);
    const Vec4 &    GetTextureSize(int index);
    const Vec4 &    GetInvTextureSize(int index);

    void            SetMaterial(const Color4 & ambient, 
                                const Color4 & emissve,
                                const Color4 & diffuse,
                                const Color4 & specular,
                                float power);

    void            SetLightDiffuse(int index, const Color4 & c);
    void            SetLightSpecular(int index, const Color4 & c);

    void            SetLightPosition(int index, const Vec4 & pos);
    void            SetLightDirection(int index, const Vec4 & dir);
    void            SetLightAttenuation(int index, float range, float a0, float a1, float a2);
    void            SetLightSpotParams(int index, float inner, float outer, float falloff);


    const Mat4 &    GetWorldMatrix();
    const Mat4 &    GetViewMatrix();
    const Mat4 &    GetProjMatrix();
    const Mat4 &    GetWorldViewMatrix();
    const Mat4 &    GetViewProjMatrix();
    const Mat4 &    GetWorldViewProjMatrix();
    const Mat4 &	GetBlendMatrix(int index);
    const Mat4 *	GetBlendMatrices();
    int             GetNumBlendMatrices();

    void            SetFogParam(const Vec4 & param);
    void            SetFogColor(const Color4 & color);
    const Vec4 &    GetFogParam();
    const Color4 &  GetFogColor();

    const Color4 &  GetMaterialAmbient();
    const Color4 &  GetMaterialEmissive();
    const Color4 &  GetMaterialDiffuse();
    const Color4 &  GetMaterialSpecular();
    float           GetMaterialPower();

    const Color4 &  GetLightDiffuse(int index);
    const Color4 &  GetLightSpecular(int index);

    const Vec4 &    GetLightPosition(int index);
    const Vec4 &    GetLightDirection(int index);
    const Vec4 &    GetLightAttenuation(int index);
    const Vec4 &    GetLightSpotParams(int index);

    void            SetCameraPosition(const Vec4 & pos);
    const Vec4 &    GetCameraPosition();

    //Time
    void            SetTime(float time);
    const Vec4 &    GetTime();

    // clip plane
    void            SetClipPlane(float nearClip, float farClip);
    const Vec4 &    GetClipPlane();

    //user
    void            SetUserData(int offset, const void * data, int size);
    void            GetUserData(int offset, void * data, int size);
    const void *    GetUserData(int offset);

	// mirror
	void			SetMirrorColor(const Color4 & clr)  { mMirrorColor = clr; }
	const Color4 &  GetMirrorColor() const  { return mMirrorColor; }

protected:
    Mat4            mWorldMatrix;
    Mat4            mViewMatrix;
    Mat4            mProjMatrix;
    Mat4            mWorldViewMatrix;
    Mat4            mViewProjMatrix;
    Mat4            mWorldViewProjMatrix;

    bool            mNeedUpdateTransfrom;
    bool            mNeedUpdateLight;

    int             mNumBlendMatrices;
    Mat4			mBlendMatrices[MAX_BLEND_MATRIX];

    //texture
    Vec4            mTextureSize[MAX_SAMPLER_STATE];
    Vec4            mInvTextureSize[MAX_SAMPLER_STATE];
    Mat4            mTextureMatrix[MAX_SAMPLER_STATE];

    //fog
    Vec4            mFogParam;
    Color4          mFogColor;

    //material
    Color4          mMtrlAmbient;
    Color4          mMtrlEmissive;
    Color4          mMtrlDiffuse;
    Color4          mMtrlSpecular;

	// mirror
	Color4			mMirrorColor;

    //light
    Color4          mLightDiffuse[MAX_LIGHTS];
    Color4          mLightSpecular[MAX_LIGHTS];
    Vec4            mLightPosition[MAX_LIGHTS];
    Vec4            mLightDirection[MAX_LIGHTS];
    Vec4            mLightAttenuation[MAX_LIGHTS];
    Vec4            mLightSpotParams[MAX_LIGHTS];

    //camera
    Vec4            mCameraPosition;

    //time
    Vec4            mTime;

    //clip plane
    Vec4            mClipPlane;

    //user
    unsigned char   mUserBuffer[MAX_USER_BUFFER];

};

}