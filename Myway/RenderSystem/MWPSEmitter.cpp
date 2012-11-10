#include "MWPSEmitter.h"

using namespace Myway;

PS_Emitter::PS_Emitter(const String & name, const String & type)
: m_name(name),
  m_type(type),
  m_vPosition(Vec3::Zero),
  m_vDirection(Vec3::UnitZ),
  m_vUp(Vec3::UnitY),
  m_cMinColor(Color4::White),
  m_cMaxColor(Color4::White),
  mMinRadian(0),
  mMaxRadian(Math::PI_1),
  mMinLife(1.0f),
  mMaxLife(1.0f),
  mMinSpeed(1.0f),
  mMaxSpeed(1.0f),
  m_vMinSize(1.0f),
  m_vMaxSize(1.0f),
  mRate(1.0f),
  mDurationTime(1.0f),
  mReplayTime(0.0f),
  m_bEnable(TRUE),
  mInternalTime(0.0f),
  mLastEmitTime(0.0f),
  m_bEmitted(true),
  mEmittionCount(0)
{
}

PS_Emitter::~PS_Emitter()
{
}

/*
void PS_Emitter::SetAttribute(int flag, const void * val)
{
    switch (flag)
    {
    case ATTRIBUTE_POSITION:
        Memcpy(&m_vPosition, val, sizeof(Vec3));
        break;

    case ATTRIBUTE_DIRECTION:
        Memcpy(&m_vDirection, val, sizeof(Vec3));
        break;

    case ATTRIBUTE_UP:
        Memcpy(&m_vUp, val, sizeof(Vec3));
        break;

    case ATTRIBUTE_ANGLE:
        mInitMaxRadian = *static_cast<const float *>(val);
        break;

    case ATTRIBUTE_LIFE:
        mInitLife = *static_cast<const float *>(val);
        break;

    case ATTRIBUTE_RATE:
        mRate = *static_cast<const float *>(val);
        break;

    case ATTRIBUTE_SPEED:
        mInitSpeed = *static_cast<const float *>(val);
        break;

    case ATTRIBUTE_SIZE:
        mInitSize = *static_cast<const float *>(val);
        break;

    case ATTRIBUTE_DURATION_TIME:
        mDurationTime = *static_cast<const float *>(val);
        break;

    case ATTRIBUTE_REPLAY_TIME:
        mReplayTime = *static_cast<const float *>(val);
        break;
    }
}

void PS_Emitter::GetAttribute(int flag, void * val)
{
    switch (flag)
    {
    case ATTRIBUTE_POSITION:
        Memcpy(val, &m_vPosition, sizeof(Vec3));
        break;

    case ATTRIBUTE_DIRECTION:
        Memcpy(val, &m_vDirection, sizeof(Vec3));
        break;

    case ATTRIBUTE_UP:
        Memcpy(val, &m_vUp, sizeof(Vec3));
        break;

    case ATTRIBUTE_ANGLE:
        Memcpy(val, &mInitMaxRadian, sizeof(float));
        break;

    case ATTRIBUTE_LIFE:
        Memcpy(val, &mInitLife, sizeof(float));
        break;

    case ATTRIBUTE_RATE:
        Memcpy(val, &mRate, sizeof(float));
        break;

    case ATTRIBUTE_SPEED:
        Memcpy(val, &mInitSpeed, sizeof(float));
        break;

    case ATTRIBUTE_SIZE:
        Memcpy(val, &mInitSize, sizeof(float));
        break;

    case ATTRIBUTE_DURATION_TIME:
        Memcpy(val, &mDurationTime, sizeof(float));
        break;
        
    case ATTRIBUTE_REPLAY_TIME:
        Memcpy(val, &mReplayTime, sizeof(float));
        break;
    }
}
*/

void PS_Emitter::SetPosition(const Vec3 & pos)
{
    m_vPosition = pos;
}

void PS_Emitter::SetPosition(float x, float y, float z)
{
    SetPosition(Vec3(x, y, z));
}

void PS_Emitter::SetDirection(const Vec3 & dir)
{
    m_vDirection = dir;
}

void PS_Emitter::SetDirection(float x, float y, float z)
{
    SetDirection(Vec3(x, y, z));
}

void PS_Emitter::SetUp(const Vec3 & up)
{
    m_vUp = up;
}

void PS_Emitter::SetUp(float x, float y, float z)
{
   SetDirection(Vec3(x, y, z));
}

void PS_Emitter::SetMinColor(const Color4 & c)
{
    m_cMinColor = c;
}

void PS_Emitter::SetMinColor(float r, float g, float b, float a)
{
    SetMinColor(Color4(r, g, b, a));
}

void PS_Emitter::SetMaxColor(const Color4 & c)
{
    m_cMaxColor = c;
}

void PS_Emitter::SetMaxColor(float r, float g, float b, float a)
{
    SetMaxColor(Color4(r, g, b, a));
}

void PS_Emitter::SetMinRadian(float ang)
{
    mMinRadian = ang;
}

void PS_Emitter::SetMaxRadian(float ang)
{
    mMaxRadian = ang;
}

void PS_Emitter::SetMinLife(float time)
{
    mMinLife = time;
}

void PS_Emitter::SetMaxLife(float time)
{
    mMaxLife = time;
}

void PS_Emitter::SetMinSpeed(float speed)
{
    mMinSpeed = speed;
}

void PS_Emitter::SetMaxSpeed(float speed)
{
    mMaxSpeed = speed;
}

void PS_Emitter::SetMinSize(const Vec3 & size)
{
    m_vMinSize = size;
}

void PS_Emitter::SetMaxSize(const Vec3 & size)
{
    m_vMaxSize = size;
}

void PS_Emitter::SetRate(float rate)
{
    mRate = rate;
}

void PS_Emitter::SetDurationTime(float time)
{
    mDurationTime = time;
}

void PS_Emitter::SetReplayTime(float time)
{
    mReplayTime = time;
}

void PS_Emitter::SetEnable(bool enable)
{
    m_bEnable = enable;
}

void PS_Emitter::SetEmitted(bool bEmitted)
{
    m_bEmitted = bEmitted;
}

const String & PS_Emitter::GetName() const
{
    return m_name;
}

const String & PS_Emitter::GetType() const
{
    return m_type;
}

const Vec3 & PS_Emitter::GetPosition() const
{
    return m_vPosition;
}

const Vec3 & PS_Emitter::GetDirection() const
{
    return m_vDirection;
}

const Vec3 & PS_Emitter::GetUp() const
{
    return m_vUp;
}

const Color4 & PS_Emitter::GetMinColor() const
{
    return m_cMinColor;
}

const Color4 & PS_Emitter::GetMaxColor() const
{
    return m_cMaxColor;
}

float PS_Emitter::GetMinRadian() const
{
    return mMinRadian;
}

float PS_Emitter::GetMaxRadian() const
{
    return mMaxRadian;
}

float PS_Emitter::GetMinLife() const
{
    return mMinLife;
}

float PS_Emitter::GetMaxLife() const
{
    return mMaxLife;
}

float PS_Emitter::GetMinSpeed() const
{
    return mMinSpeed;
}

float PS_Emitter::GetMaxSpeed() const
{
    return mMaxSpeed;
}

const Vec3 & PS_Emitter::GetMinSize() const
{
    return m_vMinSize;
}

const Vec3 & PS_Emitter::GetMaxSize() const
{
    return m_vMaxSize;
}

float PS_Emitter::GetRate() const
{
    return mRate;
}

float PS_Emitter::GetDurationTime() const
{
    return mDurationTime;
}

float PS_Emitter::GetReplayTime() const
{
    return mReplayTime;
}

bool PS_Emitter::GetEnable() const
{
    return m_bEnable;
}

bool PS_Emitter::GetEmitted() const
{
    return m_bEmitted;
}

int PS_Emitter::GetEmittionCount() const
{
    return mEmittionCount;
}

void PS_Emitter::_Update(float fElapsedTime)
{
    float emit_time = 1.0f / mRate;

    mInternalTime += fElapsedTime;
    mEmittionCount = 0;

    if (m_bEmitted)
    {
        while (mInternalTime > mDurationTime)
        {
            mInternalTime -= mDurationTime;
            m_bEmitted = false;
        }
    }
    
    if (!m_bEmitted)
    {
        if (mInternalTime > mReplayTime)
        {
            mInternalTime -= mReplayTime;
            m_bEmitted = true;
            mLastEmitTime = 0.0f;
        }
    }

    if (m_bEmitted)
    {
        float time = mInternalTime - mLastEmitTime;

        while (time > emit_time)
        {
            mLastEmitTime = mInternalTime;
            ++mEmittionCount;
            time -= emit_time;
        }
    }

}

Vec3 PS_Emitter::RandomDirection()
{
    float radian = Math::RandRange(mMinRadian, mMaxRadian);

    const Vec3 & dir = m_vDirection;
    const Vec3 & up = m_vUp;

    Quat q;
    Math::QuatFromAxis(q, up, radian);

    Vec3 newdir;
    Math::QuatRotation(newdir, q, dir);

    float rads = Math::RandRange(0.0f, Math::PI_2);
    Math::QuatFromAxis(q, dir, rads);
    Math::QuatRotation(newdir, q, newdir);

    return newdir;
}

float PS_Emitter::RandomLife()
{
    return Math::RandRange(mMinLife, mMaxLife);
}

float PS_Emitter::RandomSpeed()
{
    return Math::RandRange(mMinSpeed, mMaxSpeed);
}

Vec3 PS_Emitter::RandomSize()
{
    Vec3 size;

    size.x = Math::RandRange(m_vMinSize.x, m_vMaxSize.x);
    size.y = Math::RandRange(m_vMinSize.y, m_vMaxSize.y);
    size.z = Math::RandRange(m_vMinSize.z, m_vMaxSize.z);

    return size;
}

Color4 PS_Emitter::RandomColor()
{
    Color4 color;

    color.r = Math::RandRange(m_cMinColor.r, m_cMaxColor.r);
    color.g = Math::RandRange(m_cMinColor.g, m_cMaxColor.g);
    color.b = Math::RandRange(m_cMinColor.b, m_cMaxColor.b);
    color.a = Math::RandRange(m_cMinColor.a, m_cMaxColor.a);

    return color;
}