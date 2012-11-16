#include "MWAABB.h"
#include "MWMath.h"

namespace Myway
{

/* :) Aabb
---------------------------------------------------------------------------------------
        Axis Algined Bounding box
---------------------------------------------------------------------------------------
*/

const Aabb Aabb::Invalid   = Aabb(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT, MIN_FLOAT, MIN_FLOAT, MIN_FLOAT);
const Aabb Aabb::Infinite  = Aabb(MIN_FLOAT, MIN_FLOAT, MIN_FLOAT, MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
const Aabb Aabb::Zero      = Aabb(0, 0, 0, 0, 0, 0);
const Aabb Aabb::Identiy   = Aabb(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f);

inline Aabb::Aabb()
{
}

inline Aabb::Aabb(const Vec3 & vMin, const Vec3 & vMax)
: minimum(vMin.x, vMin.y, vMin.z),
  maximum(vMax.x, vMax.y, vMax.z)
{
}

inline Aabb::Aabb(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ)
: minimum(fMinX, fMinY, fMinZ),
  maximum(fMaxX, fMaxY, fMaxZ)
{
}

inline Aabb::~Aabb()
{
}

inline Vec3 Aabb::GetSize() const
{
	return Vec3(GetWidth(), GetHeight(), GetDepth());
}

inline float Aabb::GetWidth() const
{
    return maximum.x - minimum.x;
}

inline float Aabb::GetHeight() const
{
    return maximum.y - minimum.y;
}

inline float Aabb::GetDepth() const
{
    return maximum.z - minimum.z;
}

inline Vec3 Aabb::GetHalfSize() const
{
    return Vec3((maximum.x - minimum.x) * 0.5f, (maximum.y - minimum.y) * 0.5f, (maximum.z - minimum.z) * 0.5f);
}

inline Vec3 Aabb::GetCenter() const
{
    return (maximum + minimum) * 0.5f; 
}

inline Vec3 Aabb::GetLeftBottomFrontPoint() const
{
    return minimum;
}

inline Vec3 Aabb::GetLeftBottomBackPoint() const
{
    return Vec3(minimum.x, minimum.y, maximum.z);
}

inline Vec3 Aabb::GetLeftTopFrontPoint() const
{
    return Vec3(minimum.x, maximum.y, minimum.z);
}

inline Vec3 Aabb::GetLeftTopBackPoint() const
{
    return Vec3(minimum.x, maximum.y, maximum.z);
}

inline Vec3 Aabb::GetRightBottomFrontPoint() const
{
    return Vec3(maximum.x, minimum.y, minimum.z);
}

inline Vec3 Aabb::GetRightBottomBackPoint() const
{
    return Vec3(maximum.x, minimum.y, maximum.z);
}

inline Vec3 Aabb::GetRightTopFrontPoint() const
{
    return Vec3(maximum.x, maximum.y, minimum.z);
}

inline Vec3 Aabb::GetRightTopBackPoint() const
{
    return  maximum;
}

inline Aabb Aabb::GetLeft() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    return Aabb(minimum.x, minimum.y, minimum.z, maximum.x - fHalfWidth, maximum.y, maximum.z);
}

inline Aabb Aabb::GetRight() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    return Aabb(minimum.x + fHalfWidth, minimum.y, minimum.z, maximum.x, maximum.y, maximum.z);
}

inline Aabb Aabb::GetTop() const
{
    float fHalfHeight = GetHeight() / 2.0f;
    return Aabb(minimum.x, minimum.y + fHalfHeight, minimum.z, maximum.x, maximum.y, maximum.z);
}

inline Aabb Aabb::GetBottom() const
{
    float fHalfHeight = GetHeight() / 2.0f;
    return Aabb(minimum.x, minimum.y, minimum.z, maximum.x, maximum.y - fHalfHeight, maximum.z);
}

inline Aabb Aabb::GetFront() const
{
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x, minimum.y, minimum.z, maximum.x, maximum.y, maximum.z - fHalfDepth);
}

inline Aabb Aabb::GetBack() const
{
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x, minimum.y, minimum.z - fHalfDepth, maximum.x, maximum.y, maximum.z); 
}

inline Aabb Aabb::GetLeftTopFront() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x, minimum.y + fHalfHeight, minimum.z, maximum.x - fHalfWidth, maximum.y, maximum.z - fHalfDepth);
}

inline Aabb Aabb::GetLeftTopBack() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x, minimum.y + fHalfHeight, minimum.z + fHalfDepth, maximum.x - fHalfWidth, maximum.y, maximum.z);
}

inline Aabb Aabb::GetLeftBottomFront() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x, minimum.y, minimum.z, maximum.x - fHalfWidth, maximum.y - fHalfHeight, maximum.z - fHalfDepth);
}

inline Aabb Aabb::GetLeftBottomBack() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x, minimum.y, minimum.z + fHalfDepth, maximum.x - fHalfWidth, maximum.y - fHalfHeight, maximum.z);
}

inline Aabb Aabb::GetRightTopFront() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x + fHalfWidth, minimum.y + fHalfHeight, minimum.z, maximum.x, maximum.y, maximum.z - fHalfDepth);
}

inline Aabb Aabb::GetRightTopBack() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x + fHalfWidth, minimum.y + fHalfHeight, minimum.z + fHalfDepth, maximum.x, maximum.y, maximum.z);
}

inline Aabb Aabb::GetRightBottomFront() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x + fHalfWidth, minimum.y, minimum.z, maximum.x, maximum.y - fHalfHeight, maximum.z - fHalfDepth);
}

inline Aabb Aabb::GetRightBottomBack() const
{
    float fHalfWidth = GetWidth() / 2.0f;
    float fHalfHeight = GetHeight() / 2.0f;
    float fHalfDepth = GetDepth() / 2.0f;
    return Aabb(minimum.x + fHalfWidth, minimum.y, minimum.z + fHalfDepth, maximum.x, maximum.y - fHalfHeight, maximum.z);
}

inline Aabb & Aabb::operator =(const Aabb & aabb)
{
    minimum = aabb.minimum;
    maximum = aabb.maximum;
    return *this;
}

inline Aabb Aabb::operator *(float x) const
{
    return Aabb(minimum * x, maximum * x);
}

inline Aabb Aabb::operator /(float x) const
{
    x = 1.0f / x;
    return Aabb(minimum * x, maximum * x);
}

inline Aabb & Aabb::operator *=(float x)
{
    minimum *= x;
    maximum *= x;
    return *this;
}

inline Aabb & Aabb::operator /=(float x)
{
    x = 1.0f / x;
    minimum *= x;
    maximum *= x;
    return *this;
}



float Aabb::Volume() const
{
    return Math::AABBVolume(*this);
}

bool Aabb::Inside(const Aabb & rk) const
{
    return Math::AABBInside(*this, rk);
}

Aabb Aabb::Merge(const Aabb & rk) const
{
    Aabb aabb;
    Math::AABBMerge(aabb, *this, rk);
    return aabb;
}

Aabb Aabb::Merge(const Vec3 & rk) const
{
    Aabb aabb;
    Math::AABBMerge(aabb, *this, rk);
    return aabb;
}

Aabb Aabb::Transform(const Mat4 & maximumt) const
{
    Aabb aabb;
    Math::AABBTransform(aabb, *this, maximumt);
    return aabb;
}

Aabb Aabb::IntersectionEx(const Aabb & rk) const
{
    Aabb aabb;
    Math::AABBIntersection(aabb, *this, rk);
    return aabb;
}

int Aabb::Intersection(const Aabb & rk) const
{
    return Math::AABBIntersection(*this, rk);
}

int Aabb::Intersection(const Vec3 & rk) const
{
    return Math::AABBIntersection(*this, rk);
}

int Aabb::Intersection(const Sphere & rk) const
{
    return Math::AABBIntersection(*this, rk);
}





const Obb Obb::Zero = Obb(0, 0);
const Obb Obb::Identiy = Obb(-0.5f, 0.5f);

Obb::Obb()
{
}

Obb::Obb(const Vec3 * a, const Vec3 & c, const Vec3 & e)
{
	axis[0] = a[0];
	axis[1] = a[1];
	axis[2] = a[2];

	center = c;
	extent = e;
}

Obb::Obb(const Vec3 & vMin, const Vec3 & vMax)
{
	axis[0] = Vec3::UnitX;
	axis[1] = Vec3::UnitY;
	axis[2] = Vec3::UnitZ;

	center = (vMax + vMin) * 0.5f;
	extent = (vMax - vMin) * 0.5f;
}

Obb::~Obb()
{
}


bool Obb::Contain(const Vec3 & p) const
{
	Vec3 local_p = p - center;
	float proj = local_p.Dot(axis[0]);
	
	if (proj > extent[0] || proj < -extent[0])
		return false;

	proj = local_p.Dot(axis[1]);
	if (proj > extent[1] || proj < -extent[1])
		return false;

	proj = local_p.Dot(axis[2]);
	if (proj > extent[2] || proj < -extent[2])
		return false;

	return true;
}


}