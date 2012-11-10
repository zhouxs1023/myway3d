#include "MWMath.h"
#include "MWMathSSE.h"

using namespace Myway;

const   float       Math::PI_1                      = 3.141592654f;
const   float       Math::PI_2                      = 3.141592654f * 2.0f;
const   float       Math::PI_1_2                    = 3.141592654f / 2.0f;
const   float       Math::PI_1_3                    = 3.141592654f / 3.0f;
const   float       Math::PI_1_4                    = 3.141592654f / 4.0f;
const   float       Math::PI_1_6                    = 3.141592654f / 6.0f;
const   float       Math::PI_INV                    = 1.0f / 3.141592654f;

const   float       Math::DEG_90                    = 90.0f;
const   float       Math::DEG_180                   = 180.0f;
const   float       Math::DEG_360                   = 360.0f;
const   float       Math::DEG_90_INV                = 1.0f / 9.0f;
const   float       Math::DEG_180_INV               = 1.0f / 180.0f;
const   float       Math::DEG_360_INV               = 1.0f / 360.0f;

const   float       Math::SQRT_2                    = 1.414213562f;
const   float       Math::SQRT_3                    = 1.732050808f;
const   float       Math::SQRT_1_OVER_2             = 0.707106781f;
const   float       Math::SQRT_1_OVER_3             = 0.577350269f;

const   float       Math::FLOAT_1                   = 1.0f;
const   float       Math::FLOAT_0                   = 0.0f;

const   float       Math::EPSILON_E4                = float(1E-4);
const   float       Math::EPSILON_E5                = float(1E-5);
const   float       Math::EPSILON_E6                = float(1E-6);

const   int         Math::RANDOM_MAX                = 0x7FFF;

        int         Math::RAND_SEED                 = 0;


float Math::SIN_TABLE[TRI_TABLE_SIZE + 1];
float Math::COS_TABLE[TRI_TABLE_SIZE + 1];
float Math::TAN_TABLE[TRI_TABLE_SIZE + 1];

int Math::SQRT_TABLE[0x10000];
int Math::INV_SQRT_TABLE[0x10000];


static Vec2 NOISE_VECTOR2_TABLE[256];

unsigned char NOISE_TABLE[256] =
{
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
    7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
    203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
    20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
    166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
    220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
    63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
    200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
    186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 
    126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
    189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
    70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 
    98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 
    97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 
    241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
    199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 
    254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 
    195, 78, 66, 215, 61, 156, 180
};


int CRC32_TABLE[256] = 
{
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL
};


void Math::Init()
{
    //initialize sin , cos, tan table
    float rads = 0.0f;
    for (int i = 0; i < TRI_TABLE_SIZE + 1; i++)
    {
        SIN_TABLE[i] = ::sinf(rads);
        COS_TABLE[i] = ::cosf(rads);
        TAN_TABLE[i] = ::tanf(rads);
        rads += TRI_TABLE_FACTORY;
    }

    //initialize sqrt, inv sqrt table
    float_int ifu;
    for (int i = 0; i < 0x7FFF; i++)
    {
        ifu.i = (i << 8) | (0x7F << 23);
        ifu.f = ::sqrtf(FLOAT_BIT(ifu.f));
        SQRT_TABLE[i + 0x8000] = ifu.i & IEEE_FLOAT_MANTISSA_BIT;

        ifu.f = 1.0f / ifu.f;
        INV_SQRT_TABLE[i + 0x8000] = ifu.i & IEEE_FLOAT_MANTISSA_BIT;

        ifu.i = (i << 8) | (0x80 << 23);
        ifu.f = ::sqrtf(FLOAT_BIT(ifu.f));
        SQRT_TABLE[i] = ifu.i & IEEE_FLOAT_MANTISSA_BIT;

        ifu.f = 1.0f / ifu.f;
        INV_SQRT_TABLE[i] = ifu.i & IEEE_FLOAT_MANTISSA_BIT;
    }

    //initialize perlin noise vector2 table using sin cos
    rads = 0.0f;
    for (int i = 0; i < 256; i++)
    {
        NOISE_VECTOR2_TABLE[i].x = ::sinf(rads);
        NOISE_VECTOR2_TABLE[i].y = ::cosf(rads);
        rads += PI_2 / 256.0f;
    }
}

void Math::DeInit()
{
}


int Math::Crc32(const char * buffer, int size)
{
    long crc = 0;  
    long temp1, temp2;  
    long tmpCRC;  
    for (int i = 0; i < size; ++i)
    {  
        temp1 = (crc >> 8) & 0x00FFFFFFL;  
        tmpCRC = ((byte)crc ^ buffer[i]);  

        temp2 = CRC32_TABLE[(int)(tmpCRC & 0x000000FFL)];  
        crc = temp1 ^ temp2;  
    }

    return crc;

    //    int icrc32 = 0;
    //
    //    __asm
    //    {
    //        mov		eax, -1;
    //        mov		ebx, buffer;
    //        or		ebx, ebx;
    //        jz		_done;
    //        mov		ecx, isize;
    //        or		ecx, ecx;
    //        jz		_done;
    //
    //_next:
    //        mov		dl,	[ebx];      
    //        xor		dl, al;         
    //        movzx     edx, dl;           
    //        shr		eax, 8;         
    //        xor		eax, [CRC32_TABLE + edx * 4];  
    //
    //        inc		ebx;
    //        loop    _next;
    //
    //_done:
    //        mov     icrc32, eax;
    //    }
    //
    //    return icrc32;
}



#include <iostream>
void Math::Test()
{
    using namespace std;
    const int iTestCount = 10000;

    //maximum
    cout << "/*:   Maximum:                                " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        int a = Math::RandRange(0, 10000);
        int b = Math::RandRange(0, 10000);

        int c = Math::Maximum(a, b);
        int d = max(a, b);
        assert(c == d);
    }
    cout << endl;
    cout << endl;
    cout << endl;

    //minimum
    cout << "/*:   Minimum:                                " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        int a = Math::RandRange(0, 10000);
        int b = Math::RandRange(0, 10000);

        int c = Math::Minimum(a, b);
        int d = min(a, b);
        assert(c == d);
    }
    cout << endl;
    cout << endl;
    cout << endl;


    //swap
    cout << "/*:   Swap:                                   " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        int a = Math::RandRange(0, 10000);
        int b = Math::RandRange(0, 10000);
        int c = a;
        int d = b;

        Math::Swap(a, b);
        swap(c, d);
        assert(a == c && b == d);
    }
    cout << endl;
    cout << endl;
    cout << endl;


    //IsPowerOfTwo
    cout << "/*:   IsPowerOfTwo:                           " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    assert(IsPowerOfTwo(2.0f));
    assert(IsPowerOfTwo(2));
    cout << endl;
    cout << endl;
    cout << endl;


    //Abs
    cout << "/*:   Abs:                                    " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        int a = Math::RandRange(-10000, 10000);
        int b, c;

        b = Math::Abs(a);
        c = abs(a);
        assert(b == c);
    }
    cout << endl;
    cout << endl;
    cout << endl;


    //Round
    cout << "/*:   Round:                                  " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(0.0f, 10000.0f);
        float b, c;

        b = Math::Round(a);
        c = (float)int(a + 0.5f);
        assert(b == c);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //Floor
    cout << "/*:   Floor:                                  " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(-10000.0f, 10000.0f);
        float b, c;

        b = Math::Floor(a);
        c = floor(a);
        assert(b == c);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //Ceil
    cout << "/*:   Ceil:                                  " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(-10000.0f, 10000.0f);
        float b, c;

        b = Math::Ceil(a);
        c = ceilf(a);
        assert(b == c);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //FastSin
    cout << "/*:   FastSin:                                " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(-10000.0f, 10000.0f);
        float b, c;

        b = Math::FastSin(a);
        c = sinf(a);
        assert(fabs(b - c) <= 0.001f);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //FastCos
    cout << "/*:   FastCos:                                " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(-10000.0f, 10000.0f);
        float b, c;

        b = Math::FastCos(a);
        c = cosf(a);
        assert(fabs(b - c) <= 0.001);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //FastTan
    cout << "/*:   FastTan:                                " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(-10000.0f, 10000.0f);
        float b, c;

        b = Math::FastTan(a);
        c = tanf(a);
        assert(fabs(b - c) <= 0.001f);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //FastSqrt
    cout << "/*:   FastSqrt:                               " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(0.0f, 10000.0f);
        float b, c;

        b = Math::FastSqrt(a);
        c = sqrtf(a);
        assert(fabs(b - c) <= 0.01f);
    }
    cout << endl;
    cout << endl;
    cout << endl;



    //FastInvSqrt
    cout << "/*:   FastInvSqrt:                             " << endl
        << "-----------------------------------------------" << endl
        << "                                               " << endl
        << "-----------------------------------------------" << endl
        << "*/" << endl;
    for (int i = 0; i < iTestCount; i++)
    {
        float a = Math::RandRange(0.0f, 10000.0f);
        float b, c;

        b = Math::FastInvSqrt(a);
        c = 1.0f / sqrtf(a);
        assert(fabs(b - c) <= 0.01f);
    }
    cout << endl;
    cout << endl;
    cout << endl;
    cin.get();

}

/* 函数: PerlinNoise2
------------------------------------------------------------------------------
@Remark:
柏林噪声.
------------------------------------------------------------------------------  
*/
#define GetNoiseVec2(v, x, y)                                   \
{                                                               \
    unsigned char a = NOISE_TABLE[x & 255];                     \
    unsigned char b = NOISE_TABLE[y & 255];                     \
    unsigned char val = NOISE_TABLE[(a + b) & 255];             \
    v = NOISE_VECTOR2_TABLE[val];                               \
}

float Math::PerlinNoise2(float x, float y, float scale)
{
    Vec2 v(x * scale, y * scale);

    float x0 = (float)::floor(v.x);
    float y0 = (float)::floor(v.y);
    float x1 = x0 + 1.0f;
    float y1 = y0 + 1.0f;

    Vec2 v1, v2, v3, v4;
    GetNoiseVec2(v1, (int)x0, (int)y0);
    GetNoiseVec2(v2, (int)x0, (int)y1);
    GetNoiseVec2(v3, (int)x1, (int)y0);
    GetNoiseVec2(v4, (int)x1, (int)y1);

    Vec2 d1(v.x - x0, v.y - y0);
    Vec2 d2(v.x - x0, v.y - y1);
    Vec2 d3(v.x - x1, v.y - y0);
    Vec2 d4(v.x - x1, v.y - y1);

    float l1, l2, l3, l4;
    l1 = d1.x * v1.x + d1.y * v1.y;
    l2 = d2.x * v2.x + d2.y * v2.y;
    l3 = d3.x * v3.x + d3.y * v3.y;
    l4 = d4.x * v4.x + d4.y * v4.y;

    /*
    Perlin Function 1 (1985):
    w = 3t^2 - 2t^3        @: hermit interpolate
    */

    /*
    Perlin Function 2 (2002):
    w = 6t^5 - 15t^4 + 10t^3    @: I am using this
    */
    float sx, sy;

    float x3 = d1.x * d1.x * d1.x;
    float y3 = d1.y * d1.y * d1.y;

    sx = 6.0f * x3 * d1.x * d1.x - 15.0f * x3 * d1.x + 10.0f * x3;
    sy = 6.0f * y3 * d1.y * d1.y - 15.0f * y3 * d1.y + 10.0f * y3;

    float avg0x = l1 + (sx * (l3 - l1));
    float avg1x = l2 + (sx * (l4 - l2));
    float result = avg0x + (sy * (avg1x - avg0x));

    return result;
}

/* 函数: MaxSameSubmultiple
------------------------------------------------------------------------------
@Remark:
求最大公因数.

m = max(m, n)
n = min(m, n)
r = m % n
if (r == 0)
return 0;
else
m = n;
n = r;
loop ->
------------------------------------------------------------------------------  
*/
int Math::MaxSameSubmultiple(int m, int n)
{
    int r = m;

    do
    {
        m = r;

        if (m < n)
        {
            r = m;
            m = n;
            n = r;
        }

        r = m % n;

    } while (r);

    return n;
}


bool Math::Intersection2Line(Vec2 & vOut, const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d)
{
    float r, s, denominator = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);

		// If the denominator in above is zero, AB & CD are colinear
		if (denominator == 0)
		{
			return false;
		}

		float numeratorR = (a.y - c.y) * (d.x - c.x) - (a.x - c.x) * (d.y - c.y);
		//  If the numerator above is also zero, AB & CD are collinear.
		//  If they are collinear, then the segments may be projected to the x- 
		//  or y-axis, and overlap of the projected intervals checked.

		r = numeratorR / denominator;

		float numeratorS = (a.y - c.y) * (b.x - a.x) - (a.x - c.x) * (b.y - a.y);

		s = numeratorS / denominator;

		//  If 0<=r<=1 & 0<=s<=1, intersection exists
		//  r<0 or r>1 or s<0 or s>1 line segments do not intersect
		if (r < 0 || r > 1 || s < 0 || s > 1)
		{
			return false;
		}

		///*
		//    Note:
		//    If the intersection point of the 2 lines are needed (lines in this
		//    context mean infinite lines) regardless whether the two line
		//    segments intersect, then
		//
		//        If r>1, P is located on extension of AB
		//        If r<0, P is located on extension of BA
		//        If s>1, P is located on extension of CD
		//        If s<0, P is located on extension of DC
		//*/

		// Find intersection point
		vOut = Vec2((a.x + (r * (b.x - a.x))), (a.y + (r * (b.y - a.y))));
        return true;
}


/* : ComputeTangent
------------------------------------------------------------
@Remark:
计算切线.
------------------------------------------------------------
*/
void Math::ComputeTBN(Vec3 & T,
                         Vec3 & B,
                         Vec3 & N,
                         const Vec3 & P1, const Vec2 & UV1,
                         const Vec3 & P2, const Vec2 & UV2,
                         const Vec3 & P3, const Vec3 & UV3)
{
    /*
    @P2P1 = @U2U1 * T + @V2V1 * B
    @P3P1 = @U3U1 * T + @V3V1 * B

    | @P2P1 |   | @U2U1 @V2V1 |   | T |
    |       | = |             | * |   |
    | @P3P1 |   | @U3U1 @V3V1 |   | B |

    N = T X B
    */
    Vec3 P2P1 = P2 - P1;
    Vec3 P3P1 = P3 - P1;
    float u2u1 = UV2.x - UV1.x;
    float u3u1 = UV2.y - UV1.y;
    float v2v1 = UV3.x - UV1.x;
    float v3v1 = UV3.y - UV1.y;

    float fDet = (u2u1 * v3v1 - u3u1 * v2v1);
    if (fDet == 0.0f)
    {
        T = Vec3::UnitX;
        B = Vec3::UnitY;
        N = Vec3::UnitZ;
    }
    else
    {
        fDet = 1.0f / fDet;
        T.x = fDet * (v3v1 * P2P1.x - v2v1 * P3P1.x);
        T.y = fDet * (v3v1 * P2P1.y - v2v1 * P3P1.y);
        T.z = fDet * (v3v1 * P2P1.z - v2v1 * P3P1.z);
        B.x = fDet * (-u3u1 * P2P1.x + u2u1 * P3P1.x);
        B.y = fDet * (-u3u1 * P2P1.y + u2u1 * P3P1.y);
        B.z = fDet * (-u3u1 * P2P1.z + u2u1 * P3P1.z);
        VecCross(N, T, B);
        VecNormalize(T, T);
        VecNormalize(B, B);
        VecNormalize(N, N);
    }
}

void Math::ComputeTBNInverse(Vec3 & InvT, Vec3 & InvB, Vec3 & InvN,
                                const Vec3 & P1, const Vec2 & UV1,
                                const Vec3 & P2, const Vec2 & UV2,
                                const Vec3 & P3, const Vec3 & UV3)
{
    /*
    @P2P1 = @U2U1 * T + @V2V1 * B
    @P3P1 = @U3U1 * T + @V3V1 * B

    | @P2P1 |   | @U2U1 @V2V1 |   | T |
    |       | = |             | * |   |
    | @P3P1 |   | @U3U1 @V3V1 |   | B |

    N = T X B
    */
    Vec3 T, B, N;
    Vec3 P2P1 = P2 - P1;
    Vec3 P3P1 = P3 - P1;
    float u2u1 = UV2.x - UV1.x;
    float u3u1 = UV2.y - UV1.y;
    float v2v1 = UV3.x - UV1.x;
    float v3v1 = UV3.y - UV1.y;

    float fDet = (u2u1 * v3v1 - u3u1 * v2v1);
    if (fDet == 0.0f)
    {
        T = Vec3::UnitX;
        B = Vec3::UnitY;
        N = Vec3::UnitZ;
    }
    else
    {
        fDet = 1.0f / fDet;
        T.x = fDet * (v3v1 * P2P1.x - v2v1 * P3P1.x);
        T.y = fDet * (v3v1 * P2P1.y - v2v1 * P3P1.y);
        T.z = fDet * (v3v1 * P2P1.z - v2v1 * P3P1.z);
        B.x = fDet * (-u3u1 * P2P1.x + u2u1 * P3P1.x);
        B.y = fDet * (-u3u1 * P2P1.y + u2u1 * P3P1.y);
        B.z = fDet * (-u3u1 * P2P1.z + u2u1 * P3P1.z);
        VecCross(N, T, B);

        fDet = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + 
            (B.x * N.y * T.z - B.z * N.y * T.x) +
            (N.x * T.y * B.z - N.z * T.y * B.x));

        Vec3 BXN, NXT, TXB;
        VecCross(BXN, B, N);
        VecCross(NXT, N, T);
        VecCross(TXB, T, B);

        InvT.x =  BXN.x * fDet;
        InvT.y = -NXT.x * fDet;
        InvT.z =  TXB.x * fDet;

        InvB.x = -BXN.y * fDet;
        InvB.y =  NXT.y * fDet;
        InvB.z = -TXB.y * fDet;

        InvN.x =  BXN.z * fDet;
        InvN.y = -NXT.z * fDet;
        InvN.z =  TXB.z * fDet;

        VecNormalize(InvT, InvT);
        VecNormalize(InvB, InvB);
        VecNormalize(InvN, InvN);
    }
}

void Math::UpdateSkinned(Vec3 * pos, Vec3 * normals, Vec3 * tangents,
                         const Vec3 * origin_pos,
                         const Vec3 * origin_normals,
                         const Vec3 * origin_tangents,
                         const SkinData * infl,
                         const Mat4 * mats,
                         int count)
{
#ifdef WIN32
    Mat4  *m1, *m2, *m3, *m4;

    __asm
    {
        mov     esi,    infl;
        mov     edi,    mats;
        mov     ecx,    count;

__again:
        mov     ebx,    64;

        //indices[0]
        mov     eax,    0;
        mov     al,     [esi];
        mul     ebx;
        add     eax,    edi;
        mov     m1,     eax;

        //indices[1]
        mov     eax,    0;
        mov     al,     1[esi];
        mul     ebx;
        add     eax,    edi;
        mov     m2,     eax;

        //indices[2]
        mov     eax,    0;
        mov     al,     2[esi];
        mul     ebx;
        add     eax,    edi;
        mov     m3,     eax;

        //indices[3]
        mov     eax,    0;
        mov     al,     3[esi];
        mul     ebx;
        add     eax,    edi;
        mov     m4,     eax;

        //m1
        movss   xmm4,   4[esi];     //weight1;
        shufps  xmm4,   xmm4,   SHUFFLE_PS(0, 0, 0, 0);

        mov     edx,    m1;
        movups  xmm0,   [edx];
        movups  xmm1,   16[edx];
        movups  xmm2,   32[edx];
        movups  xmm3,   48[edx];
        mulps   xmm0,   xmm4;
        mulps   xmm1,   xmm4;
        mulps   xmm2,   xmm4;
        mulps   xmm3,   xmm4;

        //m2
        cmp     dword ptr 8[esi], 0;
        jz      __weight3;
        movss   xmm4,   8[esi];     //weight2;
        shufps  xmm4,   xmm4,   SHUFFLE_PS(0, 0, 0, 0);

        mov     edx,    m2;
        movups  xmm5,   [edx];
        movups  xmm6,   16[edx];
        mulps   xmm5,   xmm4;
        mulps   xmm6,   xmm4;
        addps   xmm0,   xmm5;
        addps   xmm1,   xmm6;

        movups  xmm5,   32[edx];
        movups  xmm6,   48[edx];
        mulps   xmm5,   xmm4;
        mulps   xmm6,   xmm4;
        addps   xmm2,   xmm5;
        addps   xmm3,   xmm6;

__weight3:
        //m3
        cmp     dword ptr 12[esi], 0;
        jz      __weight4;
        movss   xmm4,   12[esi];     //weight3;
        shufps  xmm4,   xmm4,   SHUFFLE_PS(0, 0, 0, 0);

        mov     edx,    m3;
        movups  xmm5,   [edx];
        movups  xmm6,   16[edx];
        mulps   xmm5,   xmm4;
        mulps   xmm6,   xmm4;
        addps   xmm0,   xmm5;
        addps   xmm1,   xmm6;

        movups  xmm5,   32[edx];
        movups  xmm6,   48[edx];
        mulps   xmm5,   xmm4;
        mulps   xmm6,   xmm4;
        addps   xmm2,   xmm5;
        addps   xmm3,   xmm6;

__weight4:
        //m4
        cmp     dword ptr 12[esi], 0;
        jz      __position;
        movss   xmm4,   16[esi];     //weight4;
        shufps  xmm4,   xmm4,   SHUFFLE_PS(0, 0, 0, 0);

        mov     edx,    m4;
        movups  xmm5,   [edx];
        movups  xmm6,   16[edx];
        mulps   xmm5,   xmm4;
        mulps   xmm6,   xmm4;
        addps   xmm0,   xmm5;
        addps   xmm1,   xmm6;

        movups  xmm5,   32[edx];
        movups  xmm6,   48[edx];
        mulps   xmm5,   xmm4;
        mulps   xmm6,   xmm4;
        addps   xmm2,   xmm5;
        addps   xmm3,   xmm6;

__position:
        //transform position
        mov     edx,    pos;
        cmp     edx,    0;
        jz      __normal;

        mov     eax,    origin_pos;

        movss	xmm4,	[eax];
        movss	xmm5,	4[eax];
        movss	xmm6,	8[eax];

        shufps	xmm4,	xmm4,	0;
        shufps	xmm5,	xmm5,	0;
        shufps	xmm6,	xmm6,	0;

        mulps	xmm4,	xmm0;
        mulps	xmm5,	xmm1;
        mulps	xmm6,	xmm2;

        addps	xmm4,	xmm5;
        addps	xmm6,	xmm3;
        addps	xmm4,	xmm6;

        movlps	[edx],	xmm4;
        shufps	xmm4,	xmm4,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edx + 8], xmm4;

        add     pos,    12;
        add     origin_pos, 12;

__normal:
        mov     edx,    normals;
        cmp     edx,    0;
        jz      __tangent;

        mov     eax,    origin_normals;

        movss	xmm4,	[eax];
        movss	xmm5,	4[eax];
        movss	xmm6,	8[eax];

        shufps	xmm4,	xmm4,	0;
        shufps	xmm5,	xmm5,	0;
        shufps	xmm6,	xmm6,	0;

        mulps	xmm4,	xmm0;
        mulps	xmm5,	xmm1;
        mulps	xmm6,	xmm2;

        addps	xmm4,	xmm5;
        addps	xmm4,	xmm6;

        movlps	[edx],	xmm4;
        shufps	xmm4,	xmm4,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edx + 8], xmm4;

        add     normals,    12;
        add     origin_normals, 12;  

__tangent:
        mov     edx,    tangents;
        cmp     edx,    0;
        jz      __next;

        mov     eax,    origin_tangents;

        movss	xmm4,	[eax];
        movss	xmm5,	4[eax];
        movss	xmm6,	8[eax];

        shufps	xmm4,	xmm4,	0;
        shufps	xmm5,	xmm5,	0;
        shufps	xmm6,	xmm6,	0;

        mulps	xmm4,	xmm0;
        mulps	xmm5,	xmm1;
        mulps	xmm6,	xmm2;

        addps	xmm4,	xmm5;
        addps	xmm4,	xmm6;

        movlps	[edx],	xmm4;
        shufps	xmm4,	xmm4,	SHUFFLE_PS(2, 2, 2, 2);
        movss	[edx + 8], xmm4;

        add     tangents,    12;
        add     origin_tangents, 12;

__next:
        add     esi,    20;
        sub     ecx,    1;
        jnz     __again;
    }
#else
    Mat4 blend;
    Vec3 p1, p2, p3, p4;
    char i1, i2, i3, i4;
    float w1, w2, w3, w4;

    for (int i = 0; i < count; ++i)
    {
        i1 = infl[i].indices[0];
        i2 = infl[i].indices[1];
        i3 = infl[i].indices[2];
        i4 = infl[i].indices[3];

        w1 = infl[i].weights[0];
        w2 = infl[i].weights[1];
        w3 = infl[i].weights[2];
        w4 = infl[i].weights[3];

        blend = mats[i1] * w1 + mats[i2] * w2 +
            mats[i3] * w3 + mats[i4] * w4;

        if (pos)
        {
            VecTransform(pos[i], origin_pos[i], blend);
        }

        if (normals)
        {
            VecTransformN(normals[i], origin_normals[i], blend);
        }

        if (tangents)
        {
            VecTransformN(tangents[i], tangents[i], blend);
        }
    }
#endif
}