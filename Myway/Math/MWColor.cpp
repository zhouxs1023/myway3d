#include "MWColor.h"

namespace Myway
{

    const Color Color::White      = Color(255, 255, 255, 255);
    const Color Color::Red        = Color(255,   0,   0, 255);
    const Color Color::Green      = Color(0,   255,   0, 255);
    const Color Color::Blue       = Color(0,     0, 255, 255);
    const Color Color::Black      = Color(0,     0,   0, 255);

/* :) Color
---------------------------------------------------------------------------------------
@Remark:
    Color is 4 byte (clamp 0.0 - 1.0)
---------------------------------------------------------------------------------------
*/

inline Color::Color()
{
}

inline Color::Color(unsigned char c)
: r(c), g(c), b(c), a(c) 
{
}

inline Color::Color(unsigned char c1, unsigned char c2, unsigned char c3)
: r(c1), g(c2), b(c3), a(0xFF) 
{
}

inline Color::Color(unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4)
: r(c1), g(c2), b(c3), a(c4) 
{
}

inline Color::Color(const Color & c)
: r(c.r), g(c.g), b(c.b), a(c.a) 
{
}

inline Color::~Color()
{
}


inline Color& Color::operator =(const Color & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;

    return *this;
}


inline bool Color::operator ==(const Color & c) const
{
    return r == c.r && g == c.g && b == c.b && a == c.a;
}

inline bool Color::operator !=(const Color & c) const
{
    return r != c.r || g != c.g || b != c.b || a != c.a;
}

std::ostream & operator <<(std::ostream & os, const Color & c)
{
    os << c.r << " " << c.g << " " << c.b << " " << c.a;
    return os;
}

}