#pragma once

#ifndef DEV_MATH_H
#define DEV_MATH_H

#include "MWVector2.h"
#include "MWVector3.h"
#include "MWVector4.h"
#include "MWVectorX.h"
#include "MWMatrix3.h"
#include "MWMatrix4.h"
#include "MWMatrix3x4.h"
#include "MWColor.h"
#include "MWColor4.h"
#include "MWFrustum.h"
#include "MWAABB.h"
#include "MWSphere.h"
#include "MWRect.h"
#include "MWLine.h"
#include "MWBox.h"
#include "MWTriangle.h"
#include "MWRay.h"
#include "MWPlane.h"
#include "MWQuaternion.h"
#include "MWSkinInfl.h"
#include "MWCurve.h"


namespace Myway
{

/*宏定义
------------------------------------------------------------------------------

------------------------------------------------------------------------------
*/
#define IEEE_FLOAT_SIGN_BIT                 0x80000000
#define IEEE_FLOAT_EXPONENT_BIT             0x7F800000
#define IEEE_FLOAT_MANTISSA_BIT             0x007FFFFF
#define IEEE_FLOAT_SIGN_MARK                0x7FFFFFFF
#define IEEE_FLOAT_EXPONENT_MARK            0x807FFFFF
#define IEEE_FLOAT_MANTISSA_MASK            0xFF800000
#define IEEE_FLOAT_SIGN_POS                 31
#define IEEE_FLOAT_MANTISSA_POS             23
#define IEEE_FLOAT_STEP_BIT                 0x3F800000

#define IEEE_FLOAT_SIGN(x)                  (INT_BIT(x) >> 31)
#define IEEE_FLOAT_EXPONENT_SIGN(x)         (IEEE_FLOAT_EXPONENT(x) >> 31)
#define IEEE_FLOAT_EXPONENT(x)              (((INT_BIT(x) & IEEE_FLOAT_EXPONENT_BIT) >> 23) - 127)
#define IEEE_FLOAT_MANTISSA(x)              (INT_BIT(x) & IEEE_FLOAT_MANTISSA_BIT)

#define IEEE_FLOAT_NOTSTANDARD(x)           ((x & 0x7F800000) == 0 && (x & 0x007FFFFF) != 0))
#define IEEE_FLOAT_INFINTE(x)               ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) == 0))
#define IEEE_FLOAT_NEGATIVE_INFINTE(x)      ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) == 0) && (x & 0x80000000) == 0x80000000))
#define IEEE_FLOAT_POSITIVE_INFINTE(x)      ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) == 0) && (x & 0x80000000) == 0x00000000))
#define IEEE_FLOAT_NAN(x)                   ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) != 0))
#define IEEE_FLOAT_SNAN(x)                  ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) != 0) && (x & 0x00400000) == 0x00400000))
#define IEEE_FLOAT_QNAN(x)                  ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) != 0) && (x & 0x00400000) == 0x00000000))

#define INT_BIT(x)                          (*reinterpret_cast<int*>(&x))
#define FLOAT_BIT(x)                        (*reinterpret_cast<float*>(&x))


#define TRI_TABLE_SIZE                      2048
#define TRI_TABLE_MASK                      (TRI_TABLE_SIZE - 1)
#define TRI_TABLE_FACTORY                   ((float)PI_2 / TRI_TABLE_SIZE)
#define TRI_TABLE_ANGLE_MUL                 ((float)TRI_TABLE_SIZE / 360.0f)
#define TRI_TABLE_RADIAN_MUL                ((float)TRI_TABLE_SIZE / PI_2)

#define DEFAULT_EPSILON                     Math::EPSILON_E4

class MW_ENTRY Math
{
public:
                        static  void        Init();
                        static  void        DeInit();

    template<class T>   static  const T &   Maximum(const T & a, const T & b);
    template<class T>   static  const T &   Minimum(const T & a, const T & b);
    template<class T>   static  void        Swap(T & a, T & b);

                        static  int         HighestBitSet(int x);
                        static  int         LowestBitSet(int x);
    template<class T>   static  bool        IsPowerOfTwo(T x);
    template<class T>   static  T           RoundPowerOfTwo(T x);
    template<class T>   static  T           FloorPowerOfTwo(T x);
    template<class T>   static  T           CeilPowerOfTwo(T x);
    
    template<class T>   static  T           Abs(T x);
    template<class T>   static  T           Sign(T x);
    template<class T>   static  bool        Compare(const T & a, const T & b);
    template<class T>   static  T           Clamp(T x, T low, T hight);
    template<class T>   static  T           ClampPositive(T x);
    template<class T>   static  T           ClampNegative(T x);
    template<class T>   static  T           ClampUnit(T x);

    template<class T>   static  T           Lerp(const T & a, const T & b, float k);

                        static  float       Sqrt(float x);
                        static  float       Sin(float x);
                        static  float       Cos(float x);
                        static  void        SinCos(float x, float & s, float & c);
                        static  float       Tan(float x);
                        static  float       Cot(float x);
                        static  float       ASin(float x);
                        static  float       ACos(float x);
                        static  float       ATan(float x);
                        static  float       Sinh(float x);
                        static  float       Cosh(float x);
                        static  float       Tanh(float x);     

                        static  float       Floor(float x);
                        static  float       Ceil(float x);
                        static  float       Round(float x);
                        static  float       Fmod(float x, float y);

                        static  float       Exp(float v);
                        static  float       Log_(float v);
                        static  float       Pow(float base, float exponent);

                        static  void        SRandom(int seed);
                        static  int         Random();
                        static  float       RandomFloat();
    template<class T>   static  T           RandRange(T a, T b);
                        static  float       PerlinNoise2(float x, float y, float scale);

                        static  int         MaxSameSubmultiple(int m, int n);

                        static  float       ComputeVariance(float * array, int count);
                        static  float       ComputeGaussian(float x, float mid = 0.0f, float var = 1.0f);
                        static  float       ComputeGaussian(float x, float y, float mid_x = 0.0f, float mid_y = 0.0f, float var = 1.0f);

                        static  float       DegreeToRadian(float degree);
                        static  float       RadianToDegree(float rad);

                        static  float       DegreeNormalize(float x);
                        static  float       RadianNormalize(float x);


                        static  float       FastSinAngle(float ang);
                        static  float       FastCosAngle(float ang);
                        static  float       FastTanAnlge(float ang);
                        static  float       FastSin(float x);
                        static  float       FastCos(float x);
                        static  float       FastTan(float x);
                        static  float       FastSqrt(float x);
                        static  float       FastInvSqrt(float x);
                        static  float       FastInverse(float x);

                        static  float       FastClamp0_1(float x);
                        static  float       FastClamp(float x, float a, float b);
                        static  float       FastClampNegative(float x);
                        static  float       FastClampPositive(float x);
                        static  float       FastIntToFloat_23(int x);
                        static  int         FastFloatToInt_23(float x);

                        static  int         Log2le(int x);
                        static  int         Log2ge(int x);

                        static  int         FastFloatToInt(float x);

                        static  void        VecMinimum(Vec2 & vOut, const Vec2 & v1, const Vec2 & v2);
                        static  void        VecMinimum(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2);
                        static  void        VecMinimum(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2);

                        static  void        VecMaximum(Vec2 & vOut, const Vec2 & v1, const Vec2 & v2);
                        static  void        VecMaximum(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2);
                        static  void        VecMaximum(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2);

                        static  float       VecDot(const Vec2 & v1, const Vec2 & v2);
                        static  float       VecDot(const Vec3 & v1, const Vec3 & v2);
                        static  float       VecDot(const Vec4 & v1, const Vec4 & v2);

                        static  float       VecLength(const Vec2 & v);
                        static  float       VecLength(const Vec3 & v);
                        static  float       VecLength(const Vec4 & v);

                        static  float       VecInvLength(const Vec2 & v);
                        static  float       VecInvLength(const Vec3 & v);
                        static  float       VecInvLength(const Vec4 & v);

                        static  float       VecLengthSq(const Vec2 & v);
                        static  float       VecLengthSq(const Vec3 & v);
                        static  float       VecLengthSq(const Vec4 & v);

                        static  float       VecDistance(const Vec2 & v1, const Vec2 & v2);
                        static  float       VecDistance(const Vec3 & v1, const Vec3 & v2);
                        static  float       VecDistance(const Vec4 & v1, const Vec4 & v2);

                        static  float       VecDistanceSq(const Vec2 & v1, const Vec2 & v2);
                        static  float       VecDistanceSq(const Vec3 & v1, const Vec3 & v2);
                        static  float       VecDistanceSq(const Vec4 & v1, const Vec4 & v2);

                        static  void        VecReflect(Vec2 & v, const Vec2 & i, const Vec2 & n);
                        static  void        VecReflect(Vec3 & v, const Vec3 & i, const Vec3 & n);
                        static  void        VecReflect(Vec4 & v, const Vec4 & i, const Vec4 & n);

                        static  void        VecReflect(Vec3 & v, const Vec3 & i, const Plane & p);
                        static  void        VecReflect(Vec4 & v, const Vec4 & i, const Plane & p);

                        static  void        VecRefract(Vec2 & v, const Vec2 & i, const Vec2 & n, float eta);
                        static  void        VecRefract(Vec3 & v, const Vec3 & i, const Vec3 & n, float eta);
                        static  void        VecRefract(Vec4 & v, const Vec4 & i, const Vec4 & n, float eta);

                        static  void        VecCross(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2);
                        static  void        VecCross(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2);

                        static  void        VecCrossN(Vec3 & vOut, const Vec3 & v1, const Vec3 & v2);
                        static  void        VecCrossN(Vec4 & vOut, const Vec4 & v1, const Vec4 & v2);

                        static  void        VecNormalize(Vec2 & o, const Vec2 & v);
                        static  void        VecNormalize(Vec3 & o, const Vec3 & v);
                        static  void        VecNormalize(Vec4 & o, const Vec4 & v);

                        static  void        VecTransformN(Vec2 & o, const Vec2 & v, const Mat3 & mat);
                        static  void        VecTransformN(Vec3 & o, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransformN(Vec4 & o, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransformN(Vec4 & o, const Vec4 & v, const Mat4 & mat);

                        static  void        VecTransform(Vec2 & o, const Vec2 & v, const Mat3 & mat);
                        static  void        VecTransform(Vec3 & o, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransformX(float & x, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransformY(float & y, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransformZ(float & z, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransform(Vec4 & o, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransform(Vec4 & o, const Vec4 & v, const Mat4 & mat);

                        static  void	    VecTransformN(Vec3 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount);
                        static  void	    VecTransformN(Vec4 * pOut, const Vec4 * pIn, const Mat4 & mat, int iCount);
                        static  void	    VecTransform(Vec3 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount);
                        static  void   	    VecTransform(Vec4 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount);
                        static  void	    VecTransform(Vec4 * pOut, const Vec4 * pIn, const Mat4 & mat, int iCount);

                        static  void        VecTransformAffine(Vec3 & vOut, const Vec3 & v, const Mat4 & mat);
                        static  void        VecTransformAffine(Vec3 * pOut, const Vec3 * pIn, const Mat4 & mat, int iCount);

                        static  void        VecLerp(Vec2 & o, const Vec2 & v1, const Vec2 & v2, float t);
                        static  void        VecLerp(Vec3 & o, const Vec3 & v1, const Vec3 & v2, float t);
                        static  void        VecLerp(Vec4 & o, const Vec4 & v1, const Vec4 & v2, float t);

                        static  float       MatMinor(const Mat3 & m, int r, int c);
                        static  float       MatMinor(const Mat4 & m, int r, int c);
                        static  float       MatMinor(const Mat3 & m, int r0, int r1, int c0, int c1);
                        static  float       MatMinor(const Mat4 & m, int r0, int r1, int r2,
                                                                     int c0, int c1, int c2);
                        static  bool        MatIsAffine(const Mat3 & m);
                        static  bool        MatIsAffine(const Mat4 & m);

                        static  float       MatDet(const Mat3 & m);
                        static  float       MatDet(const Mat4 & m);
                        static  void        MatTranspose(Mat3 & mOut, const Mat3 & m);
                        static  void        MatTranspose(Mat4 & mOut, const Mat4 & m);
                        static  void        MatInverse(Mat3 & mOut, const Mat3 & m);
                        static  void        MatInverse(Mat4 & mOut, const Mat4 & m);
                        static  void        MatTranslate(Mat3 & mOut, float x, float y);
                        static  void        MatTranslate(Mat4 & mOut, float x, float y, float z);
                        static  void        MatRotation(Mat3 & mOut, float rads);
                        static  void        MatRotationX(Mat4 & mOut, float rads);
                        static  void        MatRotationY(Mat4 & mOut, float rads);
                        static  void        MatRotationZ(Mat4 & mOut, float rads);
                        static  void        MatRotationAxis(Mat4 & mOut, const Vec3 & vAxis, float rads);
                        static  void        MatRotationAxis(Mat4 & mOut, const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis);
                        static  void        MatRotationYawPitchRoll(Mat4 & mOut, float yaw, float pitch, float roll);
                        static  void        MatRotationQuaternion(Mat4 & mOut, float x, float y, float z, float w);
                        static  void        MatScale(Mat4 & mOut, float x, float y, float z);
                        static  void        MatOrtho(Mat4 & m);
                        
                        static  void        MatViewLH(Mat4 & mOut, const Vec3 & vEye, const Vec3 & vAt, const Vec3 & vUp);
                        static  void        MatViewLH(Mat4 & mOut, const Vec3 & vEye, const Quat & qOreint);
                        static  void        MatViewLHEX(Mat4 & mOut, const Vec3 & vEye, const Vec3 & vDir, const Vec3 & vUp);
                        static  void        MatOrthoLH(Mat4 & mOut, float w, float h, float zn, float zf);
                        static  void        MatOrthoCenterLH(Mat4 & mOut, float l, float r, float t, float b, float n, float f);
                        static  void        MatPerspectiveLH(Mat4 & mOut, float w, float h,float n, float f);
                        static  void        MatPerspectiveCenterLH(Mat4 & mOut, float l, float r, float t, float b, float n, float f);
                        static  void        MatPerspectiveFovLH(Mat4 & mOut, float fov, float aspect,float n, float f);
                        static  void        MatShadow(Mat4 & mOut, const Vec4 & l, const Plane & p);
                        static  void        MatReflect(Mat4 & mOut, const Plane & p);
                        static  void        MatMultiply(Mat4 & mOut, const Mat4 & m1, const Mat4 & m2);
                        static  void        MatMultiply(Mat4 & mOut, const Mat4 & m1, float scale);
                        static  void        MatTransform(Mat4 & mOut, const Vec3 & trans, const Quat & rotate, const Vec3 & scale);
                        static  void        MatLerp(Mat4 & mOut, const Mat4 & m1, const Mat4 & m2, float t);


                        static  void        PlaneNormalize(Plane & pOut, const Plane & p);
                        static  float       PlaneDistance(const Plane & p, const Vec3 & v);
                        static  Plane::Side PlaneSide(const Plane & p, const Vec3 & v);
                        static  Plane::Side PlaneSide(const Plane & p, const Aabb & box);
                        static  Plane::Side PlaneSide(const Plane & p, const Vec3 & v, const Vec3 & halfSize);
                        static  Plane::Side PlaneSide(const Plane & p, const Sphere & box);
                        static  void        PlaneTransform(Plane & pOut, const Plane & p, const Mat4 & m);

                        static  float       QuatDot(const Quat & q1, const Quat & q2);
                        static  float       QuatLength(const Quat & q);
                        static  void        QuatInverse(Quat & qOut, const Quat & q);
                        static  void        QuatNormalize(Quat & qOut, const Quat & q);
                        static  void        QuatConjugate(Quat & qOut, const Quat & q);
                        static  void        QuatFromAxis(Quat & qOut, const Vec3 & vAxis, float rads);
                        static  void        QuatFromAxis(Quat & qOut, const Vec3 & xAxis, const Vec3 & yAxis, const Vec3 & zAxis);
                        static  void        QuatFromDir(Quat & qOut, const Vec3 & dir1, const Vec3 & dir2, const Vec3 & fallbackAxis = Vec3::Zero, bool normalize = true);
                        static  void        QuatFromMatrix(Quat & qOut, const Mat4 & rot);
                        static  void        QuatFromPitchYawRoll(Quat & qOut, float pitch, float yaw, float roll);
                        static  void        QuatSlerp(Quat & qOut, const Quat & q1, const Quat & q2, float t);
                        static  void        QuatMultiply(Quat & qOut, const Quat & q1, const Quat & q2);
                        static  void        QuatToAxis(Vec3 & axis, float & rads, const Quat & q);
                        static  void        QuatToMatrix(Mat4 & matOut, const Quat & q);
                        static  void        QuatTranslate(Vec3 & vOut, const Quat & q, const Vec3 & v);
                        static  void        QuatRotation(Vec3 & vOut, const Quat & q, const Vec3 & v);
                        static  void        QuatAxisX(Vec3 & xAxis, const Quat & q);
                        static  void        QuatAxisY(Vec3 & yAxis, const Quat & q);
                        static  void        QuatAxisZ(Vec3 & zAxis, const Quat & q);
                        static  void        QuatAxisXYZ(Vec3 & xAxis, Vec3 & yAxis, Vec3 & zAxis, const Quat & q);

                        static  void        ColorNormalize(Color4 & cOut, const Color4 & cIn);
                        static  void        ColorSaturate(Color4 & cOut, const Color4 & cIn);
                        static  void        ColorLerp(Color4 & cOut, const Color4 & c1, const Color4 & c2, float t);

                        static  float       AABBVolume(const Aabb & aabb);
                        static  bool        AABBInside(const Aabb & dest, const Aabb & src);
                        static  void        AABBMerge(Aabb & aabbOut, const Aabb & aabb1, const Aabb & aabb2);
                        static  void        AABBMerge(Aabb & aabbOut, const Aabb & aabb1, const Vec3 & vPoint);
                        static  void        AABBTransform(Aabb & aabbOut, const Aabb & aabbIn, const Mat4 & mat);
                        static  int       AABBIntersection(Aabb & aabbOut, const Aabb & aabb1, const Aabb & aabb2);
                        static  float       AABBIntersectionVolume(const Aabb & aabb1, const Aabb & aabb2);
                        static  int       AABBIntersection(const Aabb & aabb1, const Aabb & aabb2);
                        static  int       AABBIntersection(const Aabb & aabb, const Vec3 & vPoint);
                        static  int       AABBIntersection(const Aabb & aabb, const Sphere & sph);

                        static  float       SphereVolume(const Sphere & sph);
                        static  void        SphereMerge(Sphere & sphOut, const Sphere & sph1, const Sphere & sph2);
                        static  void        SphereMerge(Sphere & sphOut, const Sphere & sphIn, const Vec3 & vPoint);
                        static  void        SphereTransform(Sphere & sphOut, const Sphere & sphIn, const Mat4 & mat);
                        static  int       SphereIntersection(const Sphere & sph1, const Sphere & sph2);
                        static  int       SphereIntersection(const Sphere & sph1, const Vec3 & vPoint);
                        static  int       SphereIntersection(const Sphere & sph, const Aabb & aabb);

                        static  void        FrustumFromMatrix(Frustum & frustum, const Mat4 & mat);
                        
                        static  void        RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Plane & plane);
                        static  void        RayIntersection(RayIntersectionInfo & info, const Vec3 & orig, const Vec3 & target, const Plane & plane);
                        static  void        RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Sphere & sph);
                        static  void        RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Aabb & aabb);
                        static  void        RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Tri3 & tri);
						static  void		RayIntersection(RayIntersectionInfo & info, const Ray & ray, const Vec3 * vert,
															const short * idx, int numTris, const Mat4 & form);
						static  void		RayIntersection(RayIntersectionInfo & info, const Ray & ray,
							                                const Vec3 & a, const Vec3 & b, const Vec3 & c);

                        static  bool        Intersection2Line(Vec2 & vOut, const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d);

                  
                        static  void        ComputeTBN(Vec3 & t, Vec3 & b, Vec3 & n,
                                                       const Vec3 & p1, const Vec2 & uv1,
                                                       const Vec3 & p2, const Vec2 & uv2,
                                                       const Vec3 & p3, const Vec3 & uv3);
                        static  void        ComputeTBNInverse(Vec3 & inv_t, Vec3 & inv_b, Vec3 & inv_n,
                                                              const Vec3 & p1, const Vec2 & uv1,
                                                              const Vec3 & p2, const Vec2 & uv2,
                                                              const Vec3 & p3, const Vec3 & uv3);

                        static  void        UpdateSkinned(Vec3 * pos, 
                                                          Vec3 * normals,
                                                          Vec3 * tangents,
                                                          const Vec3 * origin_pos,
                                                          const Vec3 * origin_normals,
                                                          const Vec3 * origin_tangents,
                                                          const SkinData * infl,
                                                          const Mat4 * mats,
                                                          int count);


                        static int          Crc32(const char * buffer, int size);

                               void         Test();

/* 全局变量声明
------------------------------------------------------------------------------

------------------------------------------------------------------------------
*/
   static const float		PI_1;  // 无奈之..
   static const float		PI_2; 
   static const float		PI_1_2;
   static const float		PI_1_3;
   static const float		PI_1_4;
   static const float		PI_1_6;
   static const float		PI_INV;

   static const float		DEG_90;
   static const float		DEG_180;
   static const float		DEG_360;
   static const float		DEG_90_INV;
   static const float		DEG_180_INV;
   static const float		DEG_360_INV;

   static const float		SQRT_2;
   static const float		SQRT_3;
   static const float		SQRT_1_OVER_2;
   static const float		SQRT_1_OVER_3;

   static const float       FLOAT_0;
   static const float       FLOAT_1;

   static const float		EPSILON_E4;
   static const float		EPSILON_E5;
   static const float		EPSILON_E6;

   static const int         RANDOM_MAX;
   static int               RAND_SEED;

protected:
    static float            SIN_TABLE[TRI_TABLE_SIZE + 1];
    static float            COS_TABLE[TRI_TABLE_SIZE + 1];
    static float            TAN_TABLE[TRI_TABLE_SIZE + 1];

    static int              SQRT_TABLE[0x10000];
    static int              INV_SQRT_TABLE[0x10000];

    union float_int
    {
        float f;
        int   i;
    };
};

#include "MWMath.inl"


} // end namespace Myway


#endif