#include "MWColor4.h"
#include "MWMath.h"

namespace Myway
{

    const Color4 Color4::White              = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    const Color4 Color4::Red                = Color4(1.0f, 0.0f, 0.0f, 1.0f);
    const Color4 Color4::Green              = Color4(0.0f, 1.0f, 0.0f, 1.0f);
    const Color4 Color4::Blue               = Color4(0.0f, 0.0f, 1.0f, 1.0f);
    const Color4 Color4::Black              = Color4(0.0f, 0.0f, 0.0f, 1.0f);
    const Color4 Color4::Gray               = Color4(0.5f, 0.5f, 0.5f, 1.0f);


/* :) Color4
---------------------------------------------------------------------------------------
    @Remark:
        Color4 is 4 floats (clamp 0.0 - 1.0)
---------------------------------------------------------------------------------------
*/
inline Color4::Color4(float v) : r(v), g(v), b(v), a(v) 
{
}

inline Color4::Color4(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) 
{
}

inline Color4::Color4(const Color4 & c) : r(c.r), g(c.g), b(c.b), a(c.a)
{
}

inline Color4& Color4::operator =(const Color4 & c)
{
	r = c.r, g = c.g, b = c.b, a = c.a;
	return *this;
}

inline Color4 Color4::operator +(const Color4 & c) const
{
	return Color4(r + c.r, g + c.g, b + c.b, a + c.a);
}

inline Color4 Color4::operator -(const Color4 & c) const
{
	return Color4(r - c.r, g - c.g, b - c.b, a - c.a);
}

inline Color4 Color4::operator *(const Color4 & c) const
{
	return Color4(r * c.r, g * c.g, b * c.b, a * c.a);
}

inline Color4 Color4::operator *(float v) const
{
	return Color4(r * v, g * v, b * v, a * v);
}

inline Color4 Color4::operator /(float v) const
{
    Color4 c;

    v = 1.0f / v;
    c.a = a * v;
    c.r = r * v;
    c.g = g * v;
    c.b = b * v;

    return c;
}

inline Color4& Color4::operator +=(const Color4 & c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;

	return *this;
}

inline Color4& Color4::operator -=(const Color4 & c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;

	return *this;
}

inline Color4& Color4::operator *=(const Color4 & c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;

	return *this;
}

inline Color4& Color4::operator *=(float v)
{
	r *= v;
	g *= v;
	b *= v;
	a *= v;

	return *this;
}

inline Color4& Color4::operator /=(float v)
{
    v = 1.0f / v;

    r *= v;
    g *= v;
    b *= v;
    a *= v;

    return *this;
}

inline bool Color4::operator ==(const Color4 & c) const
{
    return a == c.a && r == c.r && g == c.g && b == c.b;
}

inline bool Color4::operator !=(const Color4 & c) const
{
    return a != c.a || r != c.r || g != c.g || b != c.b;
}

inline Color4 Myway::operator *(float v, const Color4 & c)
{
	return c * v;
}

inline std::ostream & operator <<(std::ostream & os, const Color4 & c)
{
	os << c.r << " " << c.g << " " << c.b << " " << c.a;
	return os;
}

Color4 Color4::Normalize() const
{
    Color4 col;
    Math::ColorNormalize(col, *this);
    return col;
}

Color4 Color4::Saturate() const
{
    Color4 col;
    Math::ColorSaturate(col, *this);
    return col;
}

Color4 Color4::Lerp(const Color4 & rk, float t) const
{
    Color4 col;
    Math::ColorLerp(col, *this, rk, t);
    return col;
}

}