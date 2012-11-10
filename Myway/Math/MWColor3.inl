

/* :) Color3
---------------------------------------------------------------------------------------
    @Remark:
        Color3 is 3 floats (clamp 0.0 - 1.0)
---------------------------------------------------------------------------------------
*/
inline Color3::Color3(float v) : r(v), g(v), b(v)
{
}

inline Color3::Color3(float _r, float _g, float _b) : r(_r), g(_g), b(_b) 
{
}

inline Color3::Color3(const Color3 & c) : r(c.r), g(c.g), b(c.b)
{
}

inline Color3& Color3::operator =(const Color3 & c)
{
	r = c.r, g = c.g, b = c.b;
	return *this;
}

inline Color3 Color3::operator +(const Color3 & c) const
{
	return Color3(r + c.r, g + c.g, b + c.b);
}

inline Color3 Color3::operator -(const Color3 & c) const
{
	return Color3(r - c.r, g - c.g, b - c.b);
}

inline Color3 Color3::operator *(const Color3 & c) const
{
	return Color3(r * c.r, g * c.g, b * c.b);
}

inline Color3 Color3::operator *(float v) const
{
	return Color3(r * v, g * v, b * v);
}

inline Color3& Color3::operator +=(const Color3 & c)
{
	r += c.r;
	g += c.g;
	b += c.b;

	return *this;
}

inline Color3& Color3::operator -=(const Color3 & c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;

	return *this;
}

inline Color3& Color3::operator *=(const Color3 & c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;

	return *this;
}

inline Color3& Color3::operator *=(float v)
{
	r *= v;
	g *= v;
	b *= v;

	return *this;
}

inline bool Color3::operator ==(const Color3 & c) const
{
    return r == c.r && g == c.g && b == c.b;
}

inline bool Color3::operator !=(const Color3 & c) const
{
    return r != c.r || g != c.g || b != c.b;
}

inline Color3 operator *(float v, const Color3 & c)
{
	return c * v;
}

inline std::ostream & operator <<(std::ostream & os, const Color3 & c)
{
	os << c.r << " " << c.g << " " << c.b;
	return os;
}
