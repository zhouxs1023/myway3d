#pragma once

#include "MWVector3.h"

namespace Myway
{

class Mat4;
class Sphere;

class MW_ENTRY Aabb
{
    DECLARE_ALLOC();

public:
    static const Aabb Invalid;
    static const Aabb Infinite;
	static const Aabb Zero;
    static const Aabb Identiy;

public:
    Aabb();
    Aabb(const Vec3 & vMin, const Vec3 & vMax);
    Aabb(float fMinX, float fMinY, float fMinZ,
         float fMaxX, float fMaxY, float fMaxZ);
    ~Aabb();

	Vec3 GetSize() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;

    Vec3 GetHalfSize() const;
    Vec3 GetCenter() const;
    Vec3 GetLeftBottomFrontPoint() const;
    Vec3 GetLeftBottomBackPoint() const;
    Vec3 GetLeftTopFrontPoint() const;
    Vec3 GetLeftTopBackPoint() const;
    Vec3 GetRightBottomFrontPoint() const;
    Vec3 GetRightBottomBackPoint() const;
    Vec3 GetRightTopFrontPoint() const;
    Vec3 GetRightTopBackPoint() const;

    Aabb GetLeft() const;
    Aabb GetRight() const;
    Aabb GetTop() const;
    Aabb GetBottom() const;
    Aabb GetFront() const;
    Aabb GetBack() const;

    Aabb GetLeftTopFront() const;
    Aabb GetLeftTopBack() const;
    Aabb GetLeftBottomFront() const;
    Aabb GetLeftBottomBack() const;
    Aabb GetRightTopFront() const;
    Aabb GetRightTopBack() const;
    Aabb GetRightBottomFront() const;
    Aabb GetRightBottomBack() const;

	void GetPoints(Vec3 * points) const;

    Aabb & operator =(const Aabb & aabb);
    Aabb   operator *(float x) const;
    Aabb   operator /(float x) const;

    Aabb & operator *=(float x);
    Aabb & operator /=(float x);

    friend MW_ENTRY Aabb operator *(float x, const Aabb & aabb);
    friend MW_ENTRY Aabb operator /(float x, const Aabb & aabb);

public:
    float   Volume() const;
    bool    Inside(const Aabb & rk) const;
    Aabb    Merge(const Aabb & rk) const;
    Aabb    Merge(const Vec3 & rk) const;
    Aabb    Transform(const Mat4 & mat) const;
    Aabb    IntersectionEx(const Aabb & rk) const;
    int     Intersection(const Aabb & rk) const;
    int     Intersection(const Vec3 & rk) const;
    int     Intersection(const Sphere & rk) const;

public:
    Vec3 minimum;
    Vec3 maximum;
};


class MW_ENTRY Obb
{
	DECLARE_ALLOC();

public:
	static const Obb Zero;
	static const Obb Identiy;

public:
	Obb();
	Obb(const Vec3 * axis, const Vec3 & center, const Vec3 & extent);
	Obb(const Vec3 & vMin, const Vec3 & vMax);
	~Obb();
	
	bool Contain(const Vec3 & p) const;

	float Width() const { return extent.x * 2; }
	float Height() const { return extent.y * 2; }
	float Depth() const { return extent.z * 2; }
	float Volume() const { return Width() * Height() * Depth(); }
	Vec3 Size() const { return extent * 2; }

public:
	Vec3 axis[3];
	Vec3 center;
	Vec3 extent;
};

}