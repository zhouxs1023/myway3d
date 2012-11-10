#include "MWPlane.h"
#include "MWMath.h"

namespace Myway
{

const Plane Plane::Zero = Plane(0, 0, 0, 0);

/* Plane
--------------------------------------------------------------------------
    @Remark:
        3d plane.
--------------------------------------------------------------------------
*/

inline Plane::Plane()
{
}

inline Plane::~Plane()
{
}

inline Plane::Plane(const Plane & p) : a(p.a), b(p.b), c(p.c), d(p.d)
{
}

inline Plane::Plane(const Vec3 & _n, float _d) : n(_n.x, _n.y, _n.z), d(_d) 
{
}

inline Plane::Plane(const Vec3 & _n, const Vec3 & _p) : n(_n)
{
    d = - Math::VecDot(_n, _p);
}

inline Plane::Plane(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) 
{
}

inline Plane::Plane(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3)
{
	//d_assert(Math::VecDot((p2 - p1), (p3 - p1)) > Math::EPSILON_E4);

	Vec3 n;
	Math::VecCrossN(n, (p2 - p1), (p3 - p1));

	a = n.x;
	b = n.y;
	c = n.z;
	d = -Math::VecDot(n, p1);
}

inline Plane& Plane::operator =(const Plane & p)
{
	a = p.a, b = p.b, c = p.c, d = p.d;
	return *this;
}

inline Plane Plane::operator -() const
{
	return Plane(-a, -b, -c, -d);
}

inline bool Plane::operator ==(const Plane & p) const
{
    return a == p.a && b == p.b && c == p.c && d == p.d;
}

inline bool Plane::operator !=(const Plane & p) const
{
    return a != p.a || b != p.b || c != p.c || d != p.d;
}

inline std::ostream & operator << (std::ostream  & os, const Plane & p)
{
	os << p.a << "  " << p.b << "  " << p.c << "  " << p.d;
	return os;
}



Plane Plane::Normalize() const
{
    Plane plane;
    Math::PlaneNormalize(plane, *this);
    return plane;
}

float Plane::Distance(const Vec3 & v) const
{
    return Math::PlaneDistance(*this, v);
}

Plane::Side Plane::AtSide(const Vec3 & v) const
{
    return Math::PlaneSide(*this, v);
}

Plane::Side Plane::AtSide(const Aabb & box) const
{
    return Math::PlaneSide(*this, box);
}

Plane::Side Plane::AtSide(const Vec3 & v, const Vec3 & halfSize) const
{
    return Math::PlaneSide(*this, halfSize);
}

Plane::Side Plane::AtSide(const Sphere & box) const
{
    return Math::PlaneSide(*this, box);
}

Plane Plane::Transform(const Mat4 & m)
{
    Plane plane;
    Math::PlaneTransform(plane, *this, m);
    return plane;
}

}
