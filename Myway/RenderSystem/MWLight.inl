
inline void Light::_SetName(const TString128 & name)
{
	mName = name;
}

inline void Light::SetType(LIGHT_TYPE type)
{
    mType = type;
}

inline LIGHT_TYPE Light::GetType() const
{
    return mType;
}


inline void Light::SetDiffuse(float r, float g, float b, float a)
{
    mDiffuse.r = r;
    mDiffuse.g = g;
    mDiffuse.b = b;
    mDiffuse.a = a;
}

inline void Light::SetDiffuse(const Color4 & c)
{
    mDiffuse = c;
}

inline void Light::SetSpecular(float r, float g, float b, float a)
{
    mSpecular.r = r;
    mSpecular.g = g;
    mSpecular.b = b;
    mSpecular.a = a;
}

inline void Light::SetSpecular(const Color4 & c)
{
    mSpecular = c;
}

inline void Light::SetAttenuation(float range, float atten0, float atten1, float atten2)
{
    mRange = range;
    mAtten0 = atten0;
    mAtten1 = atten1;
    mAtten2 = atten2;
}

inline void Light::SetRange(float range)
{
    mRange = range;
}

inline void Light::SetFalloff(float falloff)
{
    mFalloff = falloff;
}

inline void Light::SetAttenuation0(float atten)
{
    mAtten0 = atten;
}

inline void Light::SetAttenuation1(float atten)
{
    mAtten1 = atten;
}

inline void Light::SetAttenuation2(float atten)
{
    mAtten2 = atten;
}

inline void Light::SetSpotParams(float inner, float outer, float falloff)
{
    mInner = inner;
    mOuter = outer;
    mFalloff = falloff;
}

inline void Light::SetInner(float inner)
{
    mInner = inner;
}

inline void Light::SetOuter(float outer)
{
    mOuter = outer;
}

inline const Color4 & Light::GetDiffsue() const
{
    return mDiffuse;
}

inline const Color4 & Light::GetSpecular() const
{
    return mSpecular;
}


inline float Light::GetRange() const
{
    return mRange;
}

inline float Light::GetFalloff() const
{
    return mFalloff;
}

inline float Light::GetAttenuation0() const
{
    return mAtten0;
}

inline float Light::GetAttenuation1() const
{
    return mAtten1;
}

inline float Light::GetAttenuation2() const
{
    return mAtten2;
}

inline float Light::GetInner() const
{
    return mInner;
}

inline float Light::GetOuter() const
{
    return mOuter;
}