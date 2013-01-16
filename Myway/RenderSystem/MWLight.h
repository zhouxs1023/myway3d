#pragma once

#include "MWMaterial.h"
#include "MWMover.h"

namespace Myway
{

enum ShadowTechnique
{
    SHADOW_NONE,
    SHADOW_TEXTURE,
    SHADOW_VOLUME,

    MW_ALIGN_ENUM(ShadowTechnique)
};



class MW_ENTRY Light : public Mover
{
public:
                    Light(const TString128 & sName);
    virtual         ~Light();

	void			_SetName(const TString128 & name);

    void            SetType(LIGHT_TYPE type);
    LIGHT_TYPE      GetType() const;

    void            SetDiffuse(float r, float g, float b, float a);
    void            SetDiffuse(const Color4 & c);

    void            SetSpecular(float r, float g, float b, float a);
    void            SetSpecular(const Color4 & c);

    void            SetAttenuation(float range, float atten0, float atten1, float atten2);
    void            SetRange(float range);
    void            SetAttenuation0(float atten);
    void            SetAttenuation1(float atten);
    void            SetAttenuation2(float atten);

    void            SetSpotParams(float inner, float outer, float falloff);
    void            SetInner(float inner);
    void            SetOuter(float outer);
    void            SetFalloff(float falloff);

    const Color4 &  GetDiffsue() const;
    const Color4 &  GetSpecular() const;
    float           GetRange() const;
    float           GetFalloff() const;
    float           GetAttenuation0() const;
    float           GetAttenuation1() const;
    float           GetAttenuation2() const;
    float           GetInner() const;
    float           GetOuter() const;

protected:
    LIGHT_TYPE      mType;
    Color4          mDiffuse;
    Color4          mSpecular;
    float           mRange;
    float           mFalloff;
    float           mAtten0;
    float           mAtten1;
    float           mAtten2;
    float           mInner;
    float           mOuter;
};

#include "MWLight.inl"

}