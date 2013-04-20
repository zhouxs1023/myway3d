#pragma once 

#include "MWMath.h"
#include "MWProperty.h"

namespace Myway
{
/* LockedRect
----------------------------------------------------------
    @Remark:
        锁定的矩形(锁纹理用的).
        某些纹理一行的实际大小可能和定义时不同.
----------------------------------------------------------
*/
struct MW_ENTRY LockedRect
{
    int  Pitch;
    void * pData;
};


/* LockedBox
----------------------------------------------------------
    @Remark:
        锁定的长方体(锁纹理用的).
        某些纹理一行的实际大小可能和定义时不同.       
----------------------------------------------------------
*/
struct MW_ENTRY LockedBox
{
    int  RowPitch;
    int  SlicePitch;
    void * pData;
};



/* USAGE
----------------------------------------------------------
    @Remark:
        用法(硬件缓存使用)      
----------------------------------------------------------
*/
enum USAGE
{
    USAGE_STATIC,
    USAGE_DYNAMIC,

    MW_ALIGN_ENUM(USAGE)
};


/* enum: LOCK
----------------------------------------------------------
    @Remark:
        锁定方式.        
----------------------------------------------------------
*/
enum LOCK
{
    LOCK_NORMAL         = 0,
    LOCK_DISCARD        = 1,
    LOCK_NOOVERWRITE    = 2,
    LOCK_READONLY       = 4,

    MW_ALIGN_ENUM(LOCK)
};


/* enum: FORMAT
----------------------------------------------------------
    @Remark:
        格式类型(像素格式, 索引格式).        
----------------------------------------------------------
*/

enum FORMAT
{
    FMT_UNKNOWN,

    FMT_R8G8B8,
    FMT_A8R8G8B8,
    FMT_X8R8G8B8,
    FMT_R5G6B5,
    FMT_X1R5G5B5,
    FMT_A1R5G5B5,
    FMT_A4R4G4B4,
    FMT_R3G3B2,
    FMT_A8,
    FMT_A8R3G3B2,
    FMT_X4R4G4B4,
    FMT_A2B10G10R10,
    FMT_A8B8G8R8,
    FMT_X8B8G8R8,
    FMT_G16R16,
    FMT_A2R10G10B10,
    FMT_A16B16G16R16,

    FMT_A8P8,
    FMT_P8,

    FMT_L8,
    FMT_A8L8,
    FMT_A4L4,

    FMT_V8U8,
    FMT_L6V5U5,
    FMT_X8L8V8U8,
    FMT_Q8W8V8U8,
    FMT_V16U16,
    FMT_A2W10V10U10,

    FMT_UYVY,
    FMT_R8G8_B8G8,
    FMT_YUY2,
    FMT_G8R8_G8B8,
    FMT_DXT1,
    FMT_DXT2,
    FMT_DXT3,
    FMT_DXT4,
    FMT_DXT5,

    FMT_D16_LOCKABLE,
    FMT_D32,
    FMT_D15S1,
    FMT_D24S8,
    FMT_D24X8,
    FMT_D24X4S4,
    FMT_D16,

    FMT_D32F_LOCKABLE,
    FMT_D24FS8,

    FMT_L16,

    FMT_VERTEXDATA,

    FMT_INDEX16,
    FMT_INDEX32,

    FMT_Q16W16V16U16,

    FMT_MULTI2_ARGB8,

    // Floating point surface formats(s10e5 formats <16-bits per channel>)
    FMT_R16F,
    FMT_G16R16F,
    FMT_A16B16G16R16F,

    // IEEE s23e8 formats (32-bits per channel)
    FMT_R32F,
    FMT_G32R32F,
    FMT_A32B32G32R32F,

    //D3D9 Ex
    FMT_D32_LOCKABLE,
    FMT_S8_LOCKABLE,
    FMT_CxV8U8,
    FMT_A1,
    FMT_BINARYBUFFER,

    MW_ALIGN_ENUM(FORMAT)
};


/* enum: TEXTURE_TYPE
----------------------------------------------------------
    @Remark:
        纹理类型.        
----------------------------------------------------------
*/
enum TEXTURE_TYPE
{
    TEXTYPE_2D,                 // normal texture
    TEXTYPE_3D,                 // volume texture
    TEXTYPE_CUBE,               // cube texture
    TEXTYPE_RENDERTARGET,       // render target

    MW_ALIGN_ENUM(TEXTURE_TYPE)
};

/* enum: DECL_TYPE
----------------------------------------------------------
    @Remark:
        定点元素类型.        
----------------------------------------------------------
*/
enum DECL_TYPE
{
    DT_FLOAT1         = 0,
    DT_FLOAT2         = 1,
    DT_FLOAT3         = 2,
    DT_FLOAT4         = 3,
    DT_COLOR          = 4,
    DT_UBYTE4         = 5,
    DT_SHORT2         = 6,
    DT_SHORT4         = 7,

    DT_UNKNOWN        = 255
};


/* enum: DECL_USAGE
----------------------------------------------------------
    @Remark:
        定点元素语义.        
----------------------------------------------------------
*/
enum DECL_USAGE
{
    DU_POSITION      = 0,
    DU_BLENDWEIGHT   = 1,
    DU_BLENDINDICES  = 2,
    DU_NORMAL        = 3,
    DU_TEXCOORD      = 5,
    DU_TANGENT       = 6,
    DU_BINORMAL      = 7,
    DU_COLOR         = 10,
    
    DU_UNKNOWN       = 255
};


/* struct: VertexElement
--------------------------------------------------------------------------------------
    @Remark:
        顶点元素声明.
--------------------------------------------------------------------------------------
*/
#define MAX_ELEMENT             16

#define DECL_UNUSED             255

struct MW_ENTRY VertexElement
{
    short           Stream;
    short           Offset;
    DECL_TYPE       Type;
    unsigned char   unused;
    DECL_USAGE      Usage;
    unsigned char   UsageIndex;

    VertexElement()
    : Stream(DECL_UNUSED),
      Offset(0),
      Type(DT_UNKNOWN),
      unused(0),
      Usage(DU_UNKNOWN),
      UsageIndex(0)
    {
    }
};


/* enum: SHADER_PROGRAM_LANGUAGE
----------------------------------------------------------
    @Remark:
        着色程序语言.        
----------------------------------------------------------
*/
enum SHADER_LANGUAGE
{
    SL_UNKNOWN,

    SL_HLSL,
    SL_CG,
    SL_ASM,
    SL_PRECOMPILED,

    MW_ALIGN_ENUM(SHADER_LANGUAGE)
};


/* enum: SHADER_PARAM_TYPE
----------------------------------------------------------
    @Remark:
        着色程序参数类型.        
----------------------------------------------------------
*/
enum SHADER_PARAM_TYPE
{
    SPT_UNKNOWN,

    SPT_INT4,
    SPT_FLOAT4,

    SPT_MATRIX3x4,
    SPT_MATRIX4x4,

    MW_ALIGN_ENUM(SHADER_PARAM_TYPE)
};

enum SHADER_PARAM_BIND_TYPE
{
    SPBT_UNKNOWN,

    SPBT_WORLD_MATRIX,
    SPBT_VIEW_MATRIX,
    SPBT_PROJ_MATRIX,
    SPBT_WORLD_VIEW_MATRIX,
    SPBT_VIEW_PROJ_MATRIX,
    SPBT_WORLD_VIEW_PROJ_MATRIX,
    SPBT_BLEND_MATRIX,

    SPBT_FOG_PARAM,
    SPBT_FOG_COLOR,

    SPBT_MATERIAL_AMBIENT,
    SPBT_MATERIAL_EMISSIVE,
    SPBT_MATERIAL_DIFFUSE,
	SPBT_MATERIAL_SPECULAR,

    SPBT_MIRROR_COLOR,

    SPBT_LIGHT_POSITION,
    SPBT_LIGHT_DIRECTION,
    SPBT_LIGHT_ATTENUATION,
    SPBT_LIGHT_SPOT_PARAM,

    SPBT_TEXTURE_SIZE,
    SPBT_INV_TEXTURE_SIZE,

    SPBT_TIME, // x - time, y - sin(t), z - cos(t), w - 1

    SPBT_CLIP_PANE, // x: near, y: far, z: 1 / near, w: 1 / far

    SPBT_USER,

    MW_ALIGN_ENUM(SHADER_PARAM_BIND_TYPE)
};


/* enum: SHADER_PROGRAM_PROFILE
----------------------------------------------------------
    @Remark:
        着色程序版本.        
----------------------------------------------------------
*/
enum SHADER_PROFILE
{
    SP_UNKNOWN      = 0,

    SP_VS_2_0       = 200,
    SP_VS_3_0       = 300,

    SP_PS_2_0       = 2000,
    SP_PS_3_0       = 3000,

    MW_ALIGN_ENUM(SHADER_PROFILE)
};

enum SAMPLER_STATE_TYPE
{
    SAMP_ADDRESSU = 1,
    SAMP_ADDRESSV = 2,
    SAMP_ADDRESSW = 3,
    SAMP_BORDERCOLOR = 4,
    SAMP_MAGFILTER = 5,
    SAMP_MINFILTER = 6,
    SAMP_MIPFILTER = 7,
    SAMP_MIPMAPLODBIAS = 8,
    SAMP_MAXMIPLEVEL = 9,
    SAMP_MAXANISOTROPY = 10,
    SAMP_SRGBTEXTURE = 11,
    SAMP_ELEMENTINDEX = 12,
    SAMP_DMAPOFFSET = 13,

    MW_ALIGN_ENUM(SAMPLER_STATE_TYPE)
};

enum TEXTURE_ADDRESS_MODE
{
    TEXA_WRAP = 1,
    TEXA_MIRROR = 2,
    TEXA_CLAMP = 3,
    TEXA_BORDER = 4,

    MW_ALIGN_ENUM(TEXTURE_ADDRESS_MODE)
};

enum TEXTURE_FILTER_TYPE
{
    TEXF_DEFAULT = 0, 
    TEXF_POINT = 1,
    TEXF_LINEAR = 2,
    TEXF_ANISOTROPIC = 3,

    MW_ALIGN_ENUM(TEXTURE_FILTER_TYPE)
};

enum SamplerBindType
{
    SPBT_Unknown,

    SBT_Emissive,
    SBT_Diffuse,
    SBT_Normal,
    SBT_Specular,
    SBT_Shadow,
    SBT_Scene,
    SBT_Extern0,
    SBT_Extern1,
    SBT_Extern2,
    SBT_Extern3,
    SBT_Extern4,
    SBT_Extern5,
    SBT_Extern6,
    SBT_Extern7,
};

struct MW_ENTRY SamplerState
{
    SamplerBindType      BindType;
    TEXTURE_FILTER_TYPE  Filter; 
    TEXTURE_ADDRESS_MODE Address;
    Color                BorderColor;
    int                  MaxMipLevel;

    SamplerState() : BindType(SPBT_Unknown),
                     Filter(TEXF_DEFAULT),
                     Address(TEXA_WRAP),
                     BorderColor(0, 0, 0),
                     MaxMipLevel(0) {}
};

/* enum: MULTI_SAMPLE
----------------------------------------------------------
    @Remark:
        多重采样类型.        
----------------------------------------------------------
*/
enum MULTI_SAMPLE
{
    MSAA_NONE	= 0,
    MSAA_2x		= 2,
    MSAA_4x		= 4,
    MSAA_8x		= 8,
    MSAA_16x		= 16,

    MW_ALIGN_ENUM(MULTI_SAMPLE)
};

/* enum: SMAA_TYPE
----------------------------------------------------------
----------------------------------------------------------
*/
struct MW_ENTRY eSmaaType : public IEnumObj
{
	enum enum_t {
		SMAA_NONE,
		SMAA_LOW,
		SMAA_MEDIUM,
		SMAA_HIGH,
		SMAA_ULTRA
	};

	DECLARE_ENUM(eSmaaType)
};

/* enum: PRIMITIVE_TYPE
----------------------------------------------------------
    @Remark:
        图元类型.        
----------------------------------------------------------
*/
enum PRIMITIVE_TYPE
{
    PRIM_UNKNOW                 = 0,
    PRIM_POINTLIST              = 1,
    PRIM_LINELIST               = 2,
    PRIM_LINESTRIP              = 3,
    PRIM_TRIANGLELIST           = 4,
    PRIM_TRIANGLESTRIP          = 5,
    PRIM_TRIANGLEFAN            = 6,

    MW_ALIGN_ENUM(PRIMITIVE_TYPE)
};

/* enum: FOG_MODE
----------------------------------------------------------
    @Remark:
        雾效模式.
----------------------------------------------------------
*/
enum FOG_MODE
{
    FOG_NONE                    = 0,
    FOG_LINEAR                  = 1,                
    FOG_EXP_1                   = 2,                
    FOG_EXP_2                   = 3,

    MW_ALIGN_ENUM(FOG_MODE)
};

/* struct: DeviceProperty
----------------------------------------------------------
    @Remark:
        设备属性.
----------------------------------------------------------
*/
struct MW_ENTRY DeviceProperty
{
	HINSTANCE		hInst;					//应用程序句柄
    HWND            hWnd;                   //窗口句柄
    int             Adpter;                 //显示适配器
    bool            bWnded;                 //是否窗口化
    int             Width;                  //窗口宽
    int             Height;                 //窗口高
    bool            bVSync;                 //是否垂直同步 
    int             RefreshRate;            //屏幕刷新率(窗口模式必须为0)
    bool            bNVPerfHUD;             //NVidia PerfHUD

    DeviceProperty()
    {
        hInst = NULL;
        hWnd = NULL;
        Adpter = 0;
        bWnded = TRUE;
        Width = 800;
        Height = 600;
        bVSync = FALSE;
        RefreshRate = 0;
        bNVPerfHUD = TRUE;
    }
};

enum TextureCaps
{
    TEXCAPS_ALPHA           = 1,                //Alpha in texture pixels is supported. 
    TEXCAPS_ALPHAPALETTE    = 2,                //Device can draw alpha from texture palettes. 
    TEXCAPS_CUBEMAP         = 4,                //Supports cube textures. 
    TEXCAPS_MIPCUBEMAP      = 16,               //Device supports mipmapped cube textures. 
    TEXCAPS_MIPMAP          = 32,               //Device supports mipmapped textures. 
    TEXCAPS_MIPVOLUMEMAP    = 64,               //Device supports mipmapped volume textures. 
    TEXCAPS_VOLUMEMAP       = 128,              //Device supports volume textures. 

    MW_ALIGN_ENUM(TextureCaps)
};


/* struct: RenderOp
----------------------------------------------------------
    @Remark:
        设备属性.        
----------------------------------------------------------
*/
struct RenderPerf
{
    int       iTriangleCount;
    int       iDrawPrimtiveCount;
    int       iShaderChangeCount;
};

/* enum: CULL_MODE
----------------------------------------------------------
    @Remark:
        背面裁剪模式.        
----------------------------------------------------------
*/
enum CULL_MODE
{
    CULL_NONE                   = 1,                //不裁剪
    CULL_CW                     = 2,                //逆时针
    CULL_CCW                    = 3,                //顺时针

    MW_ALIGN_ENUM(CULL_MODE)
};


/* enum: FILL_MODE
----------------------------------------------------------
    @Remark:
        填充模式.        
----------------------------------------------------------
*/
enum FILL_MODE
{
    FILL_POINT                  = 1,                //点填充
    FILL_FRAME                  = 2,                //框架填充
    FILL_SOLID                  = 3,                //实填充

    MW_ALIGN_ENUM(FILL_MODE)
};

enum BLEND_MODE
{
    BM_OPATICY,
    BM_ALPHA_TEST,
    BM_ALPHA_BLEND,
    BM_ADD,
    BM_MULTIPLY,
    BM_ALPHA_TEST_ADD,
    BM_ALPHA_TEST_MUL,
};

enum DEPTH_CHECK_MODE
{
    DCM_LESS_EQUAL,
    DCM_LESS,
    DCM_GREATER_EQUAL,
    DCM_GREATER,
    DCM_EQUAL,
	DCM_ALWAYS,
    DCM_NONE,
};


/* enum: COLOR_WRITE_ENABLE
----------------------------------------------------------
    @Remark:
        颜色写入通道.        
----------------------------------------------------------
*/
enum COLOR_WRITE_ENABLE
{
    CWE_RED    = 1 << 0,
    CWE_BLUE   = 1 << 1,
    CWE_GREEN  = 1 << 2,
    CWE_ALPHA  = 1 << 3,
    CWE_ALL    = CWE_RED | CWE_BLUE | CWE_GREEN | CWE_ALPHA,

    MW_ALIGN_ENUM(COLOR_WRITE_ENABLE)
};


struct RenderState
{
    CULL_MODE cullMode;
    FILL_MODE fillMode;
    int colorWrite;
    bool depthWrite;
    DEPTH_CHECK_MODE depthCheck;
    BLEND_MODE blendMode;
    float alphaTestRef;

    RenderState()
    {
        cullMode = CULL_CCW;
        fillMode = FILL_SOLID;
        colorWrite = CWE_ALL;
        depthWrite = true;
        depthCheck = DCM_LESS_EQUAL;
        blendMode = BM_OPATICY;
        alphaTestRef = 0.5f;
    }
};

/* enum: LIGHT_MODE
----------------------------------------------------------
    @Remark:
        光照模式.        
----------------------------------------------------------
*/
enum LIGHT_TYPE
{
    LT_POINT                 = 1,
    LT_SPOT                  = 2,
    LT_DIRECTIONAL           = 3,

    MW_ALIGN_ENUM(LIGHT_TYPE)
};


enum LIGHTING_MODE
{
    LM_NONE                         = 0,
    LM_PHONE                        = 1,

    MW_ALIGN_ENUM(LIGHTING_MODE)
};

enum TRANSFORM_TYPE
{
    TS_LOCAL,
    TS_PARENT,
    TS_WORLD,

    MW_ALIGN_ENUM(TRANSFORM_TYPE)
};


enum CONSTANT_VARIABLE
{
    MAX_RENDER_TARGET       = 4,
    MAX_PASS                = 8,
    MAX_LIGHTS              = 8,
    MAX_SAMPLER_STATE      = 16,
    MAX_VERTEX_STREAM       = 8,
    MAX_SHADOW_CAMERAS      = 8,
    MAX_BLEND_MATRIX        = 256,
    MAX_BLEND_MATRIX_VS     = 75,
    DEFAULT_MIPMAP_LEVEL    = 8,
    MAX_USER_BUFFER         = 4096,
    MAX_TEXTURE_LINKER      = 8,

    SHADOW_TEXTURE_LINKER   = 0,

    MW_ALIGN_ENUM(CONSTANT_VARIABLE)
};


enum IMAGE_FILE_FORMAT
{
    IMAGE_BMP = 0,
    IMAGE_JPG = 1,
    IMAGE_TGA = 2,
    IMAGE_PNG = 3,
    IMAGE_DDS = 4,
    IMAGE_PPM = 5,
    IMAGE_DIB = 6,
    IMAGE_HDR = 7,
    IMAGE_PFM = 8,

    MW_ALIGN_ENUM(IMAGE_FILE_FORMAT)
};

enum IMAGE_FILTER
{
    IMAGE_FILTER_DEFAULT    = 1 << 0,
    IMAGE_FILTER_NONE       = 1 << 1,
    IMAGE_FILTER_POINT      = 1 << 2,
    IMAGE_FILTER_LINEAR     = 1 << 3,
    IMAGE_FILTER_TRIANGLE   = 1 << 4,
    IMAGE_FILTER_BOX        = 1 << 5,

    MW_ALIGN_ENUM(IMAGE_FILTER)
};

enum PROJECTION_TYPE
{
    PROJTYPE_ORTHO,
    PROJTYPE_PROJECTION,

    MW_ALIGN_ENUM(PROJECTION_TYPE)
};


struct eRenderTechType : public IEnumObj
{
	enum enum_t {
		RTT_Base,
		RTT_Mirror,
		RTT_Shadow,

		RTT_Max
	};

	DECLARE_ENUM(eRenderTechType);
};

}