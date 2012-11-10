#include "MWMath.h"

namespace Myway
{

void Math::ColorSaturate(Color4 & cOut, const Color4 & cIn)
{
    cOut.a = Math::Clamp(cIn.a, 0.0f, 1.0f);
    cOut.r = Math::Clamp(cIn.r, 0.0f, 1.0f);
    cOut.g = Math::Clamp(cIn.g, 0.0f, 1.0f);
    cOut.b = Math::Clamp(cIn.b, 0.0f, 1.0f);
}

void Math::ColorLerp(Color4 & cOut, const Color4 & c1, const Color4 & c2, float t)
{
#ifdef WIN32
    __asm 
    {
        mov	    esi,    c1;
        mov	    edi,    c2;
        mov     edx,    cOut;

        movups  xmm0,   [esi];
        movups  xmm1,   [edi];
        movss   xmm2,   t;
        subps   xmm1,   xmm0;
        shufps  xmm2,   xmm2,   0;
        mulps   xmm1,   xmm2;
        addps   xmm0,   xmm1;

        movups [edx],	xmm0;
    }
#else
    cOut.r = c1.r + (c2.r - c1.r) * t;
    cOut.g = c1.g + (c2.g - c1.g) * t;
    cOut.b = c1.b + (c2.b - c1.b) * t;
	cOut.a = c1.a + (c2.a - c1.a) * t;
#endif
}

void Math::ColorNormalize(Color4 & cOut, const Color4 & cIn)
{
    float inv = 1.0f / (cIn.a + cIn.r + cIn.g + cIn.b);

    cOut.a = cIn.a * inv; 
    cOut.r = cIn.r * inv; 
    cOut.g = cIn.g * inv; 
    cOut.b = cIn.b * inv; 
}

}