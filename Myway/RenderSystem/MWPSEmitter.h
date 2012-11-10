#pragma once

#include "MWParticle.h"

namespace Myway
{


class MW_ENTRY PS_Emitter : public AllocObj
{
    friend class PS_System;

public:
    PS_Emitter(const String & name, const String & type);
    virtual ~PS_Emitter();

    virtual void SetPosition(const Vec3 & pos);
           void SetPosition(float x, float y, float z);

    virtual void SetDirection(const Vec3 & dir);
           void SetDirection(float x, float y, float z);

    virtual void SetUp(const Vec3 & up);
           void SetUp(float x, float y, float z);

    virtual void SetMinColor(const Color4 & c);
           void SetMinColor(float r, float g, float b, float a = 1.0f);

    virtual void SetMaxColor(const Color4 & c);
    virtual void SetMaxColor(float r, float g, float b, float a = 1.0f);

    virtual void SetMinRadian(float radian);
    virtual void SetMaxRadian(float radian);

    virtual void SetMinLife(float time);
    virtual void SetMaxLife(float time);

    virtual void SetMinSpeed(float speed);
    virtual void SetMaxSpeed(float speed);

    virtual void SetMinSize(const Vec3 & size);
    virtual void SetMaxSize(const Vec3 & size);

    virtual void SetRate(float rate);

    virtual void SetDurationTime(float time);

    virtual void SetReplayTime(float time);

    virtual void SetEnable(bool enable);

    virtual void SetEmitted(bool bEmitted);

    const String & GetName() const;

    const String & GetType() const;

    const Vec3 & GetPosition() const;

    const Vec3 & GetDirection() const;

    const Vec3 & GetUp() const;

    const Color4 & GetMinColor() const;
    const Color4 & GetMaxColor() const;

    float GetMinRadian() const;
    float GetMaxRadian() const;

    float GetMinLife() const;
    float GetMaxLife() const;
    
    float GetMinSpeed() const;
    float GetMaxSpeed() const;

    const Vec3 & GetMinSize() const;
    const Vec3 & GetMaxSize() const;

    float GetRate() const;

    float GetDurationTime() const;

    float GetReplayTime() const;

    bool GetEnable() const;

    bool GetEmitted() const;

    int GetEmittionCount() const;

protected:
    virtual void _InitParticle(Particle * p) = 0;
    virtual void _Update(float fElapsedTime);

    Vec3 RandomDirection();
    float RandomLife();
    float RandomSpeed();
    Vec3 RandomSize();
    Color4 RandomColor();

protected:
    String                  m_name;
    String                  m_type;

    Vec3                    m_vPosition;
    Vec3                    m_vDirection;
    Vec3                    m_vUp;

    Color4                  m_cMinColor;
    Color4                  m_cMaxColor;
    float                   mMinRadian;
    float                   mMaxRadian;
    float                   mMinLife;
    float                   mMaxLife;
    float                   mMinSpeed;
    float                   mMaxSpeed;
    Vec3                    m_vMinSize;
    Vec3                    m_vMaxSize;

    float                   mRate;
    float                   mDurationTime;
    float                   mReplayTime;
    bool                    m_bEnable;

    float                   mInternalTime;
    float                   mLastEmitTime;

    int                  mEmittionCount;
    bool                    m_bEmitted;
};

class MW_ENTRY PS_EmitterFactory : public Factory<PS_Emitter>
{
public:
    PS_EmitterFactory() {}
    virtual ~PS_EmitterFactory() {}
};

}