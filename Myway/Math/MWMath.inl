

/* 函数: FastCast
------------------------------------------------------------------------------
    @Remark:
        快速类型转换.
------------------------------------------------------------------------------
*/
inline int FastFloatToInt(float x)
{
#ifdef MW_PLATFORM_WIN32

    int y;

    __asm
    {
        fld     x;
        fistp   y;
    }

    return y;

#else if 0

    int shift = 23 - IEEE_FLOAT_EXPONENT(x);
    int y = (IEEE_FLOAT_MANTISSA(x) | 0x00800000) >> shift;
    int sign = IEEE_FLOAT_SIGN(x);
    y = (y ^ sign) - sign;
    sign = IEEE_FLOAT_EXPONENT_SIGN(x);
    y &= ~sign;
    return y;

else

    return (int)x;

#endif
}



/* 函数: Maximum
------------------------------------------------------------------------------
    @Remark:
        取两者之间的最大值.
------------------------------------------------------------------------------
*/
template<class T>
inline const T & Math::Maximum(const T & a, const T & b)
{
    return a > b ? a : b;
}


/* 函数: Minimum
------------------------------------------------------------------------------
    @Remark:
        取两者之间的最小值.
------------------------------------------------------------------------------
*/
template<class T>
inline const T & Math::Minimum(const T & a, const T & b)
{
    return a < b ? a : b;
}


/* 函数: Swap
------------------------------------------------------------------------------
    @Remark:
        交换两者.
------------------------------------------------------------------------------
*/
template<class T>
inline void Math::Swap(T & a, T & b)
{
    T temp = a;
    a = b;
    b = temp;
}


/* 函数: HighestBitSet
------------------------------------------------------------------------------
    @Remark:
        取得最高位为1的位置.
------------------------------------------------------------------------------
*/
inline int Math::HighestBitSet(int x)
{
#ifdef MW_PLATFORM_WIN32 

    int y;

    __asm
    {
        bsr     eax,    x;
        mov     y,      eax;
    }

    return y;

#else

    int tmp = 0x80000000;
    int num = 31;
    while (num > 0 && !(T & tmp))
    {
        --num;
        tmp >>= 1;
    }

    return 0;

#endif

}

/* 函数: LowestBitSet
------------------------------------------------------------------------------
    @Remark:
        取得最低位为1的位置.
------------------------------------------------------------------------------
*/
inline int Math::LowestBitSet(int x)
{
#ifdef MW_PLATFORM_WIN32

    int y;

    __asm
    {
        bsf     eax,    x;
        mov     y,      eax;
    }

    return y;

#else

    int tmp = 0x00000001;
    int num = 0;

    while (num < 32 && !(x & tmp))
    {
        tmp <<= 1;
        ++num;
    }

    return num;

#endif
}

/* 函数: IsPowerOfTwo
------------------------------------------------------------------------------
    @Remark:
        是否2的整数次方.
------------------------------------------------------------------------------
*/
template<class T>
inline bool Math::IsPowerOfTwo(T x)
{
    d_assert (0);
    return !(x & (x - 1)) && x > 0;
}

template<>
inline bool Math::IsPowerOfTwo(int x)
{
    return !(x & (x - 1)) && x > 0;
}

template<>
inline bool Math::IsPowerOfTwo(float x)
{
    return !(IEEE_FLOAT_MANTISSA(x));
}


/* 函数: RoundPowerOfTwo
------------------------------------------------------------------------------
    @Remark:
        返回最接近2的整数次方(四舍五入)
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::RoundPowerOfTwo(T x)
{
    if (x < 1) 
        return 1;

    int iHighestSet = HighestBitSet(x);
    int iNextBit = x & (1 << (iHighestSet - 1));

    if (iNextBit)
        ++iHighestSet;

    return 1 << iHighestSet;
}

template<>
inline float Math::RoundPowerOfTwo(float x)
{
    int n = INT_BIT(x);

    if ((n & 0xC0000000) != 0x40000000)
        return 1.0f;

    n += (n & 0x00400000) << 1;
    n &= IEEE_FLOAT_EXPONENT_BIT;

    return FLOAT_BIT(n);
}


/* 函数: FloorPowerOfTwo
------------------------------------------------------------------------------
    @Remark:
        返回最接近2的整数次方(向下去取).
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::FloorPowerOfTwo(T x)
{
    if (x < 1) 
        return 1;

    int iHighestSet = HighestBitSet(x);

    return 1 << iHighestSet;
}

template<>
inline float Math::FloorPowerOfTwo(float x)
{
    int n = INT_BIT(x);

    if ((n & 0xC0000000) != 0x40000000)
        return 1.0f;

    n &= IEEE_FLOAT_EXPONENT_BIT;

    return FLOAT_BIT(n);
}



/* 函数: CeilPowerOfTwo
------------------------------------------------------------------------------
    @Remark:
        返回最接近2的整数次方(向上去取).
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::CeilPowerOfTwo(T x)
{
/*
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;

    return x;
*/

    if (x < 1) 
        return 1;

    int iHighestSet = HighestBitSet(x);
    int iMask = x & ((1 << iHighestSet) - 1);

    if (iMask)
        ++iHighestSet;

    return 1 << iHighestSet;
}

template<>
inline float Math::CeilPowerOfTwo(float x)
{
    int n = INT_BIT(x);

    if ((n & 0xC0000000) != 0x40000000)
        return 1.0f;

    n += IEEE_FLOAT_MANTISSA_BIT;
    n &= IEEE_FLOAT_EXPONENT_BIT;

    return FLOAT_BIT(n);
}


/* 函数: Abs
------------------------------------------------------------------------------
    @Remark:
        取绝对值.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Abs(T x)
{
    return ::abs(x);
}

template<>
inline int Math::Abs(int x)
{
    int Sign = x >> 31;
    return (x ^ Sign) - Sign;
}

template<>
inline float Math::Abs(float x)
{
    int y = INT_BIT(x);
    y &= IEEE_FLOAT_SIGN_MARK;
    return  FLOAT_BIT(y);
}



/* 函数: Sign
------------------------------------------------------------------------------
    @Remark:
        取符号, 小于零返回-1, 否则返回1.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Sign(T x)
{
    if (x < 0)
        return (T)-1;
    else
        return (T)1;
}



/* 函数: Compare
------------------------------------------------------------------------------
    @Remark:
        比较2个值, 等于返回true, 否则返回false.
------------------------------------------------------------------------------
*/
template<class T>
inline bool Math::Compare(const T & a, const T & b)
{
    return a == b;
}

template<>
inline bool Math::Compare(const float & a, const float & b)
{
    float x = a - b;
    int y = INT_BIT(x);
    y &= IEEE_FLOAT_SIGN_MARK;
    return FLOAT_BIT(y) < DEFAULT_EPSILON;
}


/* 函数: Clamp
------------------------------------------------------------------------------
    @Remark:
        夹取, 不得大于最大值, 不得小于最大值.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Clamp(T x, T low, T hight)
{
    if (x < low)
        return low;

    if (x > hight)
        return hight;

    return x;
}


/* 函数: ClampPositive
------------------------------------------------------------------------------
    @Remark:
        向正夹取, 不得小于0.
------------------------------------------------------------------------------
*/
template<class T> 
inline T Math::ClampPositive(T x)
{
    return x > 0 ? x : 0;
}

template<>
inline float Math::ClampPositive(float x)
{
    int n = INT_BIT(x);
    int sign_mask = ~IEEE_FLOAT_SIGN(x);
    n &= sign_mask;
    return FLOAT_BIT(n);
}


/* 函数: ClampNegative
------------------------------------------------------------------------------
    @Remark:
        向负夹取, 不得大于0.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::ClampNegative(T x)
{
    return x < 0 ? x : 0;
}

template<>
inline float Math::ClampNegative(float x)
{
    int n = INT_BIT(x);
    int sign_mask = IEEE_FLOAT_SIGN(x);
    n &= sign_mask;
    return FLOAT_BIT(n);
}


/* 函数: ClampUnit
------------------------------------------------------------------------------
    @Remark:
        单位夹取, 不得大于-1, 不得大于1.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::ClampUnit(T x)
{
    if (x < -1)
        return -1;
    
    if (x > 1)
        return 1;

    return x;
}

template<>
inline float Math::ClampUnit(float x)
{
    int n = INT_BIT(x);
    int exp_sign = IEEE_FLOAT_EXPONENT_SIGN(x);
    int one = (127 << 23) & (~exp_sign);
    n = (n & exp_sign) + one;
    return FLOAT_BIT(n);
}


/* 函数: Sqrt
------------------------------------------------------------------------------
    @Remark:
        开平方.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Lerp(const T & a, const T & b, float k)
{
	if (k < 0)
		return a;
	else if (k > 1)
		return b;
	else
		return a + (b - a) * k;
}



/* 函数: Sqrt
------------------------------------------------------------------------------
    @Remark:
        开平方.
------------------------------------------------------------------------------
*/
inline float Math::Sqrt(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        fsqrt;
        fstp    x;
    }

    return x;

#else
    return ::sqrtf(x);

#endif
}

/* 函数: Sin
------------------------------------------------------------------------------
    @Remark:
        取正弦值.
------------------------------------------------------------------------------
*/
inline float Math::Sin(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        fsin;
        fstp    x;
    }

    return x;

#else

    return ::sin(x);

#endif
}


/* 函数: Cos
------------------------------------------------------------------------------
    @Remark:
        取余弦值.
------------------------------------------------------------------------------
*/
inline float Math::Cos(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        fcos;
        fstp    x;
    }

    return x;

#else 

    return ::cos(x);

#endif 

}


/* 函数: SinCos
------------------------------------------------------------------------------
    @Remark:
        取正弦值和余弦值.
------------------------------------------------------------------------------
*/
inline void Math::SinCos(float x, float & s, float & c)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        mov     ecx, c;
        mov     edx, s;
        fld     x;
        fsincos;
        fstp    [ecx];
        fstp    [edx];
    }

#else

    s = ::sinf(x);
    c = ::cosf(x);

#endif
}



/* 函数: Tan
------------------------------------------------------------------------------
    @Remark:
        取正切值.
------------------------------------------------------------------------------
*/
inline float Math::Tan(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        fsincos;
        fdiv;
        fstp    x;
    }

    return x;

#else

    ::tanf(x);

#endif
}



/* 函数: Tan
------------------------------------------------------------------------------
    @Remark:
        取正切值.
------------------------------------------------------------------------------
*/
inline float Math::Cot(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        fsincos;
        fxch;
        fdiv;
        fstp    x;
    }

    return x;

#else

    return 1.0f / ::tanf(x);

#endif
}


/* 函数: ASin
------------------------------------------------------------------------------
    @Remark:
        取反正弦值.
------------------------------------------------------------------------------
*/
inline float Math::ASin(float x)
{
    return ::asinf(x);
}


/* 函数: ACos
------------------------------------------------------------------------------
    @Remark:
        取反余弦值.
------------------------------------------------------------------------------
*/
inline float Math::ACos(float x)
{
    return ::acosf(x);
}


/* 函数: ATan
------------------------------------------------------------------------------
    @Remark:
        取反正切值.
------------------------------------------------------------------------------
*/
inline float Math::ATan(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        fpatan;
        fstp    x;
    }

    return x;

#else

    return ::atanf(x);

#endif
}


/* 函数: Sinh
------------------------------------------------------------------------------
    @Remark:
        正弦双曲线函数.
------------------------------------------------------------------------------
*/
inline float Math::Sinh(float x)
{
    return ::sinhf(x);
}


/* 函数: Cosh
------------------------------------------------------------------------------
    @Remark:
        余弦双曲线函数.
------------------------------------------------------------------------------
*/
inline float Math::Cosh(float x)
{
    return ::coshf(x);
}


/* 函数: Tanh
------------------------------------------------------------------------------
    @Remark:
        正切双曲线函数.
------------------------------------------------------------------------------
*/
inline float Math::Tanh(float x)
{
    return ::tanhf(x);
}


/* 函数: Round
------------------------------------------------------------------------------
    @Remark:
        四舍五入.
------------------------------------------------------------------------------
*/
inline float Math::Round(float x)
{
#ifdef MW_PLATFORM_WIN32

    __asm
    {
        fld     x;
        frndint;
    }

#else

    int n = INT_BIT(x);
    int sign = n & IEEE_FLOAT_EXPONENT_SIGN;
    n ~= sign;
    x = FLOAT_BIT(n);
    x = ::floorf(x + 0.4999999f);
    n = INT_BIT(x) | sign;

    return FLOAT_BIT(n);

#endif 
}

/* 函数: Fmod
------------------------------------------------------------------------------
    @Remark:
        取余数.
------------------------------------------------------------------------------
*/
inline float Math::Fmod(float x, float y)
{
    return ::fmod(x, y);
}


/* 函数: Floor
------------------------------------------------------------------------------
    @Remark:
        向下取整.
------------------------------------------------------------------------------
*/
inline float Math::Floor(float x)
{
#if 0

    int n = IEEE_FLOAT_MANTISSA(x);

    int shift = 23 - IEEE_FLOAT_EXPONENT(x);
    int y = (IEEE_FLOAT_MANTISSA(x) | 0x00800000) >> shift;
    int sign = IEEE_FLOAT_SIGN(x);
    y = (y ^ sign) - sign;
    sign = IEEE_FLOAT_EXPONENT_SIGN(x);
    y &= ~sign;
    y -= IEEE_FLOAT_SIGN(x) && (IEEE_FLOAT_EXPONENT_SIGN(x) || (IEEE_FLOAT_MANTISSA(x) & ((1 << shift) - 1)));
    return (float)y;

#else

    return (float)::floorf(x);

#endif
}

/* 函数: Ceil
------------------------------------------------------------------------------
    @Remark:
        向上取整.
------------------------------------------------------------------------------
*/
inline float Math::Ceil(float x)
{
#if 0

    int shift = 23 - IEEE_FLOAT_EXPONENT(x);
    int y = (IEEE_FLOAT_MANTISSA(x) | 0x00800000) >> shift;
    int sign = IEEE_FLOAT_SIGN(x);
    y = (y ^ sign) - sign;
    sign = IEEE_FLOAT_EXPONENT_SIGN(x);
    y &= ~sign;
    y += !IEEE_FLOAT_SIGN(x) && (IEEE_FLOAT_EXPONENT_SIGN(x) || (IEEE_FLOAT_MANTISSA(x) & ((1 << shift) - 1)));
    return (float)y;

#else

    return ::ceilf(x);

#endif
}


/* 函数: Exp
------------------------------------------------------------------------------
    @Remark:
        e的x次方.
------------------------------------------------------------------------------
*/
inline float Math::Exp(float x)
{
    return ::expf(x);
}


/* 函数: Log
------------------------------------------------------------------------------
    @Remark:
        x的2的对数.
------------------------------------------------------------------------------
*/
inline float Math::Log(float x)
{
    return ::logf(x);
}


/* 函数: Pow
------------------------------------------------------------------------------
    @Remark:
        x的y次方.
------------------------------------------------------------------------------
*/
inline float Math::Pow(float x, float y)
{
    return ::powf(x, y);
}


/* 函数: SRandom
------------------------------------------------------------------------------
    @Remark:
        设置随机数种子.
------------------------------------------------------------------------------
*/
inline void Math::SRandom(int seed)
{
    RAND_SEED = seed;
}


/* 函数: Random
------------------------------------------------------------------------------
    @Remark:
        生成随机数.
------------------------------------------------------------------------------
*/
inline int Math::Random()
{
    RAND_SEED = (RAND_SEED * 123 + 59) & RANDOM_MAX; 
    return RAND_SEED;
}


inline float Math::RandomFloat()
{
    return  (float)Random() / (float)RANDOM_MAX;
}


/* 函数: RandRange
------------------------------------------------------------------------------
    @Remark:
        生成随机数.
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::RandRange(T a, T b)
{
    return a + (T)(Random() % (b - a + 1)); 
}

template<>
inline float Math::RandRange(float a, float b)
{
    return a + (b - a) * RandomFloat();
}


/* 函数: DegreeToRadian
------------------------------------------------------------------------------
    @Remark:
        角度转弧度.
------------------------------------------------------------------------------
*/
inline float Math::DegreeToRadian(float degree)
{
    static const float DEG_TO_RAD_FACTORY = PI_1 / 180.0f;
    return degree * DEG_TO_RAD_FACTORY;
}


/* 函数: RadianToDegree
------------------------------------------------------------------------------
    @Remark:
        弧度转角度.
------------------------------------------------------------------------------
*/
inline float Math::RadianToDegree(float rad)
{
    static const float RAD_TO_DEG_FACTORY = 180.0f / PI_1;
    return rad * RAD_TO_DEG_FACTORY;
}


/* 函数: DegreeNormalize
------------------------------------------------------------------------------
    @Remark:
        normalize degree in [0, 360).
------------------------------------------------------------------------------
*/
inline float Math::DegreeNormalize(float x)
{
    if (x >= 360.0f || x < 0)
    {
        x -= 360.0f * Floor(x / 360.0f);
    }

    return x;
}


/* 函数: RadianNormalize
------------------------------------------------------------------------------
    @Remark:
        normalize radian in [0, 2pi).
------------------------------------------------------------------------------
*/
inline float Math::RadianNormalize(float x)
{
    if (x >= PI_2 || x < 0)
    {
        x -= PI_2 * Floor(x / PI_2);
    }

    return x;
}


/* :) ComputeVariance
------------------------------------------------------------------------------
    @Remark:
        求标准正态差.
                        n
        var = sqrt ( (  E(xi - avg)^2 / (n - 1)  ) )     
                        1
------------------------------------------------------------------------------
*/
inline float Math:: ComputeVariance(float * Array, int iCount)
{
    float sum = 0.0f, avg = 0.0f, var = 0.0f;
    for (int i = 0; i < iCount; ++i)
    {
        sum += Array[i];
    }

    avg = sum / (float)iCount;

    for (int i = 0; i < iCount; ++i)
    {
        var += (Array[i] - avg) * (Array[i] - avg);
    }

    var /= iCount - 1;
    var = Sqrt(var);

    return var;
}

/* :) ComputeGaussian
------------------------------------------------------------------------------
    @Remark:
        高斯分布.
                                             -(x - miu)^2 / (2 * rou^2)
        G(x) = (1 / (sqrt(2PI) * rou)) * e ^ 
                                            -((x - miu)^2 + (y - miu)^2)) / (2 * rou^2)
        G(x, y) = (1 / (2PI * rou^2)) * e ^ 
------------------------------------------------------------------------------
*/
inline float Math::ComputeGaussian(float x, float mid, float var)
{
    static const float InvSqrt2PI = 1.0f / Sqrt(PI_2);
    float dx = x - mid;

    var = 1.0f / var;
    float exp_param = -dx * dx * 2.0f * var * var;

    return InvSqrt2PI * var * Exp(exp_param);
}

inline float Math::ComputeGaussian(float x, float y, float mid_x, float mid_y, float var)
{
    static const float Inv2PI = PI_INV * 0.5f;
    float dx = x - mid_x;
    float dy = y - mid_y;

    var = 1.0f / var;
    var *= var;

    float exp_param = -(dx * dx + dy * dy) * 2.0f * var;

    return Inv2PI * var * Exp(exp_param);
}




/* 函数: FastInverse
------------------------------------------------------------------------------
    @Remark:
        快速求浮点倒数(也快不了多少).
------------------------------------------------------------------------------
*/
inline float Math::FastInverse(float x)
{
    debug_assert(!Compare(x, 0.0f), "0 can't inverse");
    //求近似值x0
    int n = 2 * 0x03F800000 - INT_BIT(x);
    float x0 = FLOAT_BIT(n);

    //牛顿迭代法 f(x) = 1 - ax = 0
    x0 = x0 * (2.0f - x * x0);
    return x0;
}


/* : FastClamp0_1
------------------------------------------------------------------------------
        Fast Clamp 0 - 1
------------------------------------------------------------------------------
*/
inline float Math::FastClamp0_1(float x)
{
    float_int fi;
    int sign;

    fi.f = x;
    sign = fi.i >> 31;
    fi.i &= ~sign;

    fi.f = 1.0f - fi.f;
    sign = fi.i >> 31;
    fi.i &= ~sign;
    fi.f = 1.0f - fi.f;

    return fi.f;
}

inline float Math::FastClamp(float x, float a, float b)
{
    float_int fi;
    int sign;

    fi.f = x - a;
    sign = fi.i >> 31;
    fi.i &= ~sign;
    fi.f = fi.f + a;

    fi.f = b - fi.f;
    sign = fi.i >> 31;
    fi.i &= ~sign;
    fi.f = b - fi.f;

    return fi.f;
}

inline float Math::FastClampNegative(float x)
{
    float_int fi;
    int sign;

    fi.f = x;
    sign = fi.i >> 31;
    fi.i &= sign;

    return fi.f;
}

inline float Math::FastClampPositive(float x)
{
    float_int fi;
    int sign;

    fi.f = x;
    sign = fi.i >> 31;
    fi.i &= ~sign;

    return fi.f;
}

inline float Math::FastIntToFloat_23(int x)
{
    float_int y1, y2;

    y1.i = x;
    y2.i = ((23 + 127) << 23) + (1 << 22);
    y1.i += y2.i;
    y1.f -= y2.f;

    return y1.f;
}

inline int Math::FastFloatToInt_23(float x)
{
    float_int fi;

    fi.f = x;
    fi.f += (1 << 23) + (1 << 22);
    fi.i &= (1 << 23) - 1;
    fi.i -= 1 << 22;

    return fi.i;
}


/* 函数: FastSinAnlge
------------------------------------------------------------------------------
    @Remark:
        快速正弦函数(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastSinAngle(float x)
{
    int Sign = INT_BIT(x) & IEEE_FLOAT_SIGN_BIT;
    x = Math::Abs(x * TRI_TABLE_ANGLE_MUL);
    int index = (int)x;
    x = x - index;
    index = index & TRI_TABLE_MASK;
    x = SIN_TABLE[index] + x * (SIN_TABLE[index + 1] - SIN_TABLE[index]);
    index = INT_BIT(x) ^ Sign;
    return FLOAT_BIT(index);
}

/* 函数: FastCosAnlge
------------------------------------------------------------------------------
    @Remark:
        快速余弦函数(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastCosAngle(float x)
{
    x = Math::Abs(x * TRI_TABLE_ANGLE_MUL);
    int index = (int)x;
    x -= index;
    index = index & TRI_TABLE_MASK;
    return COS_TABLE[index] + x * (COS_TABLE[index + 1] - COS_TABLE[index]);
}

/* 函数: FastTanAnlge
------------------------------------------------------------------------------
    @Remark:
        快速正切函数(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastTanAnlge(float x)
{
    int Sign = INT_BIT(x) & IEEE_FLOAT_SIGN_BIT;
    x = Math::Abs(x * TRI_TABLE_ANGLE_MUL);
    int index = (int)x;
    x = x - index;
    index = index & TRI_TABLE_MASK;
    x = TAN_TABLE[index] + x * (TAN_TABLE[index + 1] - TAN_TABLE[index]);
    index = INT_BIT(x) ^ Sign;
    return FLOAT_BIT(index);
}


/* 函数: FastSin
------------------------------------------------------------------------------
    @Remark:
        快速正弦函数(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastSin(float x)
{
    int Sign = INT_BIT(x) & IEEE_FLOAT_SIGN_BIT;
    x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);
    int index = (int)x;
    x = x - index;
    index = index & TRI_TABLE_MASK;
    x = SIN_TABLE[index] + x * (SIN_TABLE[index + 1] - SIN_TABLE[index]);
    index = INT_BIT(x) ^ Sign;
    return FLOAT_BIT(index);
}


/* 函数: FastCos
------------------------------------------------------------------------------
    @Remark:
        快速余弦弦函数(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastCos(float x)
{
    x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);
    int index = (int)x;
    x -= index;
    index = index & TRI_TABLE_MASK;
    return COS_TABLE[index] + x * (COS_TABLE[index + 1] - COS_TABLE[index]);
}


/* 函数: FastSin
------------------------------------------------------------------------------
    @Remark:
        快速正弦函数(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastTan(float x)
{
    int Sign = INT_BIT(x) & IEEE_FLOAT_SIGN_BIT;
    x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);
    int index = (int)x;
    x = x - index;
    index = index & TRI_TABLE_MASK;
    x = TAN_TABLE[index] + x * (TAN_TABLE[index + 1] - TAN_TABLE[index]);
    index = INT_BIT(x) ^ Sign;
    return FLOAT_BIT(index);
}


/* 函数: FastSqrt
------------------------------------------------------------------------------
    @Remark:
        快速求平方根(查表法).
------------------------------------------------------------------------------
*/
inline float Math::FastSqrt(float x)
{
    debug_assert(x >= 0.0f, "x is negative!");

    int n = INT_BIT(x);

    n = (((n + IEEE_FLOAT_STEP_BIT) >> 1) & IEEE_FLOAT_EXPONENT_BIT) | SQRT_TABLE[(n >> 8) & 0xFFFF];
    n *= (x != 0);

    return FLOAT_BIT(n);
}


/* 函数: FastInvSqrt
------------------------------------------------------------------------------
    @Remark:
        快速求平方根倒数(查表法).
------------------------------------------------------------------------------
*/
#pragma warning(push)
#pragma warning(disable : 4307)

inline float Math::FastInvSqrt(float x)
{
    debug_assert(x >= DEFAULT_EPSILON, "x is negative or zero!");

    int n = INT_BIT(x);

    n = ((( (IEEE_FLOAT_STEP_BIT * 3) - (n & IEEE_FLOAT_EXPONENT_BIT) ) >> 1) 
           & IEEE_FLOAT_EXPONENT_BIT) | INV_SQRT_TABLE[(n >> 8) & 0xFFFF];

    float r = FLOAT_BIT(n);
    r = r * (1.5f - 0.5f * x * r * r);

    return r;
}

#pragma warning(pop)

/* :) Log2le 
------------------------------------------------------------------------------
    @Remark:
        lower equal log2.
------------------------------------------------------------------------------
*/
inline int Math::Log2le(int x)
{
    int log2x = 0;
    int t = 0x10000;

    //t = 0x10000
    if (x >= t)
    {
        log2x += 16;
        t <<= 8;
    }
    else
    {
        t >>= 9;
    }

    //t = 0x1000000 or 0x100
    if (x >= t)
    {
        log2x += 8;
        t <<= 4;
    }
    else
    {
        t >>= 4;
    }

    //t = 0x10000000 or 0x10
    if (x >= t)
    {
        log2x += 4;
        t <<= 2;
    }
    else
    {
        t >>= 2;
    }

    //t = 0x80000000 or 0x04
    if (x >= t)
    {
        log2x += 2;
        t <<= 1;
    }
    else
    {
        t >>= 1;
    }

    // t = 0x00 or 0x02
    if (x > t)
    {
        log2x += 1;
    }

    return log2x;
}


/* :) Log2le 
------------------------------------------------------------------------------
    @Remark:
        greater equal log2.
------------------------------------------------------------------------------
*/
inline int Math::Log2ge(int x)
{
    int log2x = 0;
    int t = 0x8000;

    if (x > 0x80000000)
    {
        return 32;
    }

    if (x > t)
    {
        log2x += 16;
        t <<= 8;
    }
    else
    {
        t >>= 8;
    }

    if (x > t)
    {
        log2x += 8;
        t <<= 4;
    }
    else
    {
        t >>= 4;
    }

    if (x >= t)
    {
        log2x += 4;
        t <<= 2;
    }
    else
    {
        t >>= 2;
    }

    if (x >= t)
    {
        log2x += 2;
        t <<= 1;
    }
    else
    {
        t >>= 1;
    }

    if (x > t)
    {
        log2x += 1;
    }

    return log2x;
}























