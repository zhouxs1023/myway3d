#include "MWMath.h"

namespace Myway
{

void Math::FrustumFromMatrix(Frustum & frustum, const Mat4 & mat)
{
    frustum.Left.a = mat._14 + mat._11;
    frustum.Left.b = mat._24 + mat._21;
    frustum.Left.c = mat._34 + mat._31;
    frustum.Left.d = mat._44 + mat._41;

    frustum.Right.a = mat._14 - mat._11;
    frustum.Right.b = mat._24 - mat._21;
    frustum.Right.c = mat._34 - mat._31;
    frustum.Right.d = mat._44 - mat._41;

    frustum.Top.a = mat._14 - mat._12;
    frustum.Top.b = mat._24 - mat._22;
    frustum.Top.c = mat._34 - mat._32;
    frustum.Top.d = mat._44 - mat._42;

    frustum.Bottom.a = mat._14 + mat._12;
    frustum.Bottom.b = mat._24 + mat._22;
    frustum.Bottom.c = mat._34 + mat._32;
    frustum.Bottom.d = mat._44 + mat._42;

    frustum.Near.a = mat._13;
    frustum.Near.b = mat._23;
    frustum.Near.c = mat._33;
    frustum.Near.d = mat._43;

    frustum.Far.a = mat._14 - mat._13;
    frustum.Far.b = mat._24 - mat._23;
    frustum.Far.c = mat._34 - mat._33;
    frustum.Far.d = mat._44 - mat._43;

    PlaneNormalize(frustum.Left, frustum.Left);
    PlaneNormalize(frustum.Right, frustum.Right);
    PlaneNormalize(frustum.Top, frustum.Top);
    PlaneNormalize(frustum.Bottom, frustum.Bottom);
    PlaneNormalize(frustum.Near, frustum.Near);
    PlaneNormalize(frustum.Far, frustum.Far);
}

//bool Math::FrustumIntersection(const Frustum & frustum, const Vec3 & point)
//{   
//    if (PlaneDistance(frustum.Left, point) < 0.0f     ||
//        PlaneDistance(frustum.Right, point) < 0.0f    ||
//        PlaneDistance(frustum.Near, point) < 0.0f     ||
//        PlaneDistance(frustum.Far, point) < 0.0f      ||
//        PlaneDistance(frustum.Top, point) < 0.0f      ||
//        PlaneDistance(frustum.Bottom, point) < 0.0f)
//        return FALSE;
//
//    return TRUE;
//}
//
//bool Math::FrustumIntersection(const Frustum & frustum, const AxisAlignedRect3 & rect)
//{
//    const   Vec3 &  lt = rect.p1;
//            Vec3    rt = Vec3(rect.p2.x, rect.p1.y, rect.p1.z);
//            Vec3    lb = Vec3(rect.p1.x, rect.p2.y, rect.p2.z);
//    const   Vec3 &  rb = rect.p2;
//
//    if (PlaneDistance(frustum.Left, lt) < 0.0f  &&
//        PlaneDistance(frustum.Left, rt) < 0.0f  &&
//        PlaneDistance(frustum.Left, lb) < 0.0f  &&
//        PlaneDistance(frustum.Left, rb) < 0.0f)
//            return FALSE;
//
//    if (PlaneDistance(frustum.Right, lt) < 0.0f  &&
//        PlaneDistance(frustum.Right, rt) < 0.0f  &&
//        PlaneDistance(frustum.Right, lb) < 0.0f  &&
//        PlaneDistance(frustum.Right, rb) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Top, lt) < 0.0f  &&
//        PlaneDistance(frustum.Top, rt) < 0.0f  &&
//        PlaneDistance(frustum.Top, lb) < 0.0f  &&
//        PlaneDistance(frustum.Top, rb) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Bottom, lt) < 0.0f  &&
//        PlaneDistance(frustum.Bottom, rt) < 0.0f  &&
//        PlaneDistance(frustum.Bottom, lb) < 0.0f  &&
//        PlaneDistance(frustum.Bottom, rb) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Near, lt) < 0.0f  &&
//        PlaneDistance(frustum.Near, rt) < 0.0f  &&
//        PlaneDistance(frustum.Near, lb) < 0.0f  &&
//        PlaneDistance(frustum.Near, rb) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Far, lt) < 0.0f  &&
//        PlaneDistance(frustum.Far, rt) < 0.0f  &&
//        PlaneDistance(frustum.Far, lb) < 0.0f  &&
//        PlaneDistance(frustum.Far, rb) < 0.0f)
//        return FALSE;
//
//    return TRUE;
//}
//
//bool Math::FrustumIntersection(const Frustum & frustum, const Aabb & aabb)
//{
//    Vec3 LeftBottomFront = aabb.GetLeftBottomFrontPoint();
//    Vec3 LeftBottomBack = aabb.GetLeftBottomBackPoint();
//    Vec3 LeftTopFront = aabb.GetLeftTopFrontPoint();
//    Vec3 LeftTopBack = aabb.GetLeftTopBackPoint();
//    Vec3 RightBottomFront = aabb.GetRightBottomFrontPoint();
//    Vec3 RightBottomBack = aabb.GetRightBottomBackPoint();
//    Vec3 RightTopFront = aabb.GetRightTopFrontPoint();
//    Vec3 RightTopBack = aabb.GetRightTopBackPoint();
//
//    if (PlaneDistance(LeftBottomFront, frustum.Left) < 0.0f  &&
//        PlaneDistance(LeftBottomBack, frustum.Left) < 0.0f   &&
//        PlaneDistance(LeftTopFront, frustum.Left) < 0.0f     &&
//        PlaneDistance(LeftTopBack, frustum.Left) < 0.0f      &&
//        PlaneDistance(RightBottomFront, frustum.Left) < 0.0f &&
//        PlaneDistance(RightBottomBack, frustum.Left) < 0.0f  &&
//        PlaneDistance(RightTopFront, frustum.Left) < 0.0f    &&
//        PlaneDistance(RightTopBack, frustum.Left) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(LeftBottomFront, frustum.Right) < 0.0f  &&
//        PlaneDistance(LeftBottomBack, frustum.Right) < 0.0f   &&
//        PlaneDistance(LeftTopFront, frustum.Right) < 0.0f     &&
//        PlaneDistance(LeftTopBack, frustum.Right) < 0.0f      &&
//        PlaneDistance(RightBottomFront, frustum.Right) < 0.0f &&
//        PlaneDistance(RightBottomBack, frustum.Right) < 0.0f  &&
//        PlaneDistance(RightTopFront, frustum.Right) < 0.0f    &&
//        PlaneDistance(RightTopBack, frustum.Right) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(LeftBottomFront, frustum.Top) < 0.0f  &&
//        PlaneDistance(LeftBottomBack, frustum.Top) < 0.0f   &&
//        PlaneDistance(LeftTopFront, frustum.Top) < 0.0f     &&
//        PlaneDistance(LeftTopBack, frustum.Top) < 0.0f      &&
//        PlaneDistance(RightBottomFront, frustum.Top) < 0.0f &&
//        PlaneDistance(RightBottomBack, frustum.Top) < 0.0f  &&
//        PlaneDistance(RightTopFront, frustum.Top) < 0.0f    &&
//        PlaneDistance(RightTopBack, frustum.Top) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Bottom, LeftBottomFront) < 0.0f  &&
//        PlaneDistance(frustum.Bottom, LeftBottomBack) < 0.0f   &&
//        PlaneDistance(frustum.Bottom, LeftTopFront) < 0.0f     &&
//        PlaneDistance(frustum.Bottom, LeftTopBack) < 0.0f      &&
//        PlaneDistance(frustum.Bottom, RightBottomFront) < 0.0f &&
//        PlaneDistance(frustum.Bottom, RightBottomBack) < 0.0f  &&
//        PlaneDistance(frustum.Bottom, RightTopFront) < 0.0f    &&
//        PlaneDistance(frustum.Bottom, RightTopBack) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Near, LeftBottomFront) < 0.0f  &&
//        PlaneDistance(frustum.Near, LeftBottomBack) < 0.0f   &&
//        PlaneDistance(frustum.Near, LeftTopFront) < 0.0f     &&
//        PlaneDistance(frustum.Near, LeftTopBack) < 0.0f      &&
//        PlaneDistance(frustum.Near, RightBottomFront) < 0.0f &&
//        PlaneDistance(frustum.Near, RightBottomBack) < 0.0f  &&
//        PlaneDistance(frustum.Near, RightTopFront) < 0.0f    &&
//        PlaneDistance(frustum.Near, RightTopBack) < 0.0f)
//        return FALSE;
//
//    if (PlaneDistance(frustum.Far, LeftBottomFront) < 0.0f  &&
//        PlaneDistance(frustum.Far, LeftBottomBack) < 0.0f   &&
//        PlaneDistance(frustum.Far, LeftTopFront) < 0.0f     &&
//        PlaneDistance(frustum.Far, LeftTopBack) < 0.0f      &&
//        PlaneDistance(frustum.Far, RightBottomFront) < 0.0f &&
//        PlaneDistance(frustum.Far, RightBottomBack) < 0.0f  &&
//        PlaneDistance(frustum.Far, RightTopFront) < 0.0f    &&
//        PlaneDistance(frustum.Far, RightTopBack) < 0.0f)
//        return FALSE;
//
//    return TRUE;
//}
//
//bool Math::FrustumIntersection(const Frustum & frustum, const Sphere& sph)
//{
//    if ((PlaneDistance(frustum.Left, sph.center) + sph.radius < 0.0f)   ||
//        (PlaneDistance(frustum.Right, sph.center) + sph.radius < 0.0f)  ||
//        (PlaneDistance(frustum.Top, sph.center) + sph.radius < 0.0f)    ||
//        (PlaneDistance(frustum.Bottom, sph.center) + sph.radius < 0.0f) ||
//        (PlaneDistance(frustum.Near, sph.center) + sph.radius < 0.0f)   ||
//        (PlaneDistance(frustum.Far, sph.center) + sph.radius < 0.0f))
//        return FALSE;
//
//    return TRUE;
//}

}