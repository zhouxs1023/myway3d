#pragma once

#include "MWCore.h"

namespace Myway
{

class MW_ENTRY Color
{
    DECLARE_ALLOC();

public:
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Black;

public:
    Color();
    Color(unsigned char v);
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color(const Color & c);
    ~Color();

    Color& operator =(const Color & c);

    bool operator ==(const Color & c) const;
    bool operator !=(const Color & c) const;

    friend MW_ENTRY std::ostream & operator <<(std::ostream & os, const Color & c);

public:
    union
    {
        struct
        {
            unsigned char r, g, b, a;
        };

        int value;
    };
};

}