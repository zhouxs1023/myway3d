#pragma once

#define SHUFFLE_PS(x, y, z, w)		((w & 3) << 6 | (z & 3) << 4 | (y & 3) << 2 | (x & 3))

/*
代码    助记符      描述 
0       eq          相等 
1       lt          小于 
2       le          小于后等于 
3       unord       无序 
4       neq         不等 
5       nlt         不小于 
6       nle         不小于不等于 
7       ord         有序 
*/
#define SSE_EQ          0
#define SSE_LT          1
#define SSE_LE          2
#define SSE_UNORD       3
#define SSE_NE          4
#define SSE_GE          5
#define SSE_GT          6
#define SSE_ORD         7
#define SSE_CMP_OK      0xFFFFFFFF
#define SSE_CMP_FAIL    0