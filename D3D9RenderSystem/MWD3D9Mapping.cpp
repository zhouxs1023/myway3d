#include "MWD3D9Mapping.h"
#include "MWRenderHelper.h"

namespace Myway
{

DWORD D3D9Mapping::GetD3DUsage(USAGE Usage)
{
    DWORD D3D9Usage = 0;

    if (Usage == USAGE_DYNAMIC)
        D3D9Usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;

    return D3D9Usage;
}

D3DPOOL D3D9Mapping::GetD3DPool(USAGE Usage)
{
    D3DPOOL D3DPool = D3DPOOL_MANAGED;

    if (Usage == USAGE_DYNAMIC)
        D3DPool = D3DPOOL_DEFAULT;

    return D3DPool;
}

DWORD D3D9Mapping::GetD3DLock(int Lock)
{
    DWORD D3DLock = 0;

    if (Lock & LOCK_DISCARD)
        D3DLock |= D3DLOCK_DISCARD;
    if (Lock & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;
    if (Lock & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    return D3DLock;
}

D3DFORMAT D3D9Mapping::GetD3DFormat(FORMAT Format)
{
	switch (Format)
	{
	case FMT_UNKNOWN:
		return D3DFMT_UNKNOWN;

	case FMT_R8G8B8:
		return D3DFMT_R8G8B8;

	case FMT_A8R8G8B8:
		return D3DFMT_A8R8G8B8;

	case FMT_X8R8G8B8:
		return D3DFMT_X8R8G8B8;

	case FMT_R16G16:
		return D3DFMT_G16R16;

	case FMT_A16R16G16B16:
		return D3DFMT_A16B16G16R16;

	case FMT_L8:
		return D3DFMT_L8;

	case FMT_A8L8:
		return D3DFMT_A8L8;

	case FMT_DXT1:
		return D3DFMT_DXT1;

	case FMT_DXT2:
		return D3DFMT_DXT2;

	case FMT_DXT3:
		return D3DFMT_DXT3;

	case FMT_DXT4:
		return D3DFMT_DXT4;

	case FMT_DXT5:
		return D3DFMT_DXT5;

	case FMT_D32:
		return D3DFMT_D32;

	case FMT_D24S8:
		return D3DFMT_D24S8;

	case FMT_D16:
		return D3DFMT_D16;

	case FMT_D24FS8:
		return D3DFMT_D24FS8;

	case FMT_L16:
		return D3DFMT_L16;

	case FMT_R16F:
		return D3DFMT_R16F;

	case FMT_R16G16F:
		return D3DFMT_G16R16F;

	case FMT_A16R16G16B16F:
		return D3DFMT_A16B16G16R16F;

	case FMT_R32F:
		return D3DFMT_R32F;

	case FMT_R32G32F:
		return D3DFMT_G32R32F;

	case FMT_A32R32G32B32F:
		return D3DFMT_A32B32G32R32F;

	default:
		d_assert(0 && "Invalid Format.");
		return D3DFMT_UNKNOWN;
	}
}

BYTE D3D9Mapping::GetD3DDeclType(DECL_TYPE DeclType)
{
    switch (DeclType)
    {
    case DT_FLOAT1:
        return D3DDECLTYPE_FLOAT1;

    case DT_FLOAT2:
        return D3DDECLTYPE_FLOAT2;

    case DT_FLOAT3:
        return D3DDECLTYPE_FLOAT3;

    case DT_FLOAT4:
        return D3DDECLTYPE_FLOAT4;

    case DT_COLOR:
        return D3DDECLTYPE_D3DCOLOR;

    case DT_UBYTE4:
        return D3DDECLTYPE_UBYTE4;

    case DT_SHORT2:
        return D3DDECLTYPE_SHORT2;

    case DT_SHORT4:
        return D3DDECLTYPE_SHORT4;

    default:
        d_assert(0 && "Invalid Declaration Type.");
        return 0;
    }
}

int D3D9Mapping::GetD3DDeclTypeSize(BYTE type)
{
    switch (type)
    {
    case D3DDECLTYPE_FLOAT1:
        return 4;

    case D3DDECLTYPE_FLOAT2:
        return 8;

    case D3DDECLTYPE_FLOAT3:
        return 12;

    case D3DDECLTYPE_FLOAT4:
        return 16;

    case D3DDECLTYPE_D3DCOLOR:
        return 4;

    case D3DDECLTYPE_UBYTE4:
        return 4;

    case D3DDECLTYPE_SHORT2:
        return 4;

    case D3DDECLTYPE_SHORT4:
        return 8;

    case D3DDECLTYPE_UBYTE4N:
        return 4;

    case D3DDECLTYPE_SHORT2N:
        return 4;

    case D3DDECLTYPE_SHORT4N:
        return 8;

    case D3DDECLTYPE_USHORT2N:
        return 4;

    case D3DDECLTYPE_USHORT4N:
        return 8;

    case D3DDECLTYPE_FLOAT16_2:
        return 4;

    case D3DDECLTYPE_FLOAT16_4:
        return 8;

    default:
        d_assert(0 && "Invalid Declaration Type.");
        return 0;
    }
}


BYTE D3D9Mapping::GetD3DDeclUsage(DECL_USAGE DeclUsage)
{
    switch (DeclUsage)
    {
    case DU_POSITION:
        return D3DDECLUSAGE_POSITION;

    case DU_BLENDWEIGHT:
        return D3DDECLUSAGE_BLENDWEIGHT;

    case DU_BLENDINDICES:
        return D3DDECLUSAGE_BLENDINDICES;

    case DU_NORMAL:
        return D3DDECLUSAGE_NORMAL;

    case DU_TEXCOORD:
        return D3DDECLUSAGE_TEXCOORD;

    case DU_TANGENT:
        return D3DDECLUSAGE_TANGENT;

    case DU_BINORMAL:
        return D3DDECLUSAGE_BINORMAL;

    case DU_COLOR:
        return D3DDECLUSAGE_COLOR;

    default:
        d_assert(0 && "Invalid Declaration Usage.");
        return 0;
    }
}

CGprofile D3D9Mapping::GetCgProfile(SHADER_PROFILE Profile)
{
    switch (Profile)
    {
    case SP_VS:
        //return CG_PROFILE_VS_3_0;
        return cgGetProfile("vs_3_0");

    case SP_PS:
        //return CG_PROFILE_PS_3_0;
        return cgGetProfile("ps_3_0");

    default:
        //not support
        return CG_PROFILE_UNKNOWN;
    }
}

TString128 D3D9Mapping::GetHLSLProfile(SHADER_PROFILE Profile)
{
    switch (Profile)
    {
    case SP_VS:
        return "vs_3_0";

    case SP_PS:
        return "ps_3_0";

    default:
        //not support
        return "";
    }
}

int D3D9Mapping::GetTypeSize(SHADER_PARAM_TYPE Type)
{
    switch (Type)
    {
    case SPT_INT4:
        return 16;

    case SPT_FLOAT4:
        return 16;

    case SPT_MATRIX4x4:
        return 64;

    default:
        return 0;
    }
}

D3DXIMAGE_FILEFORMAT D3D9Mapping::GetD3DXImageFileFormat(IMAGE_FILE_FORMAT Format)
{
    D3DXIMAGE_FILEFORMAT D3DImageFormat;

    switch (Format)
    {
    case IMAGE_BMP:
        D3DImageFormat = D3DXIFF_BMP;
        break;

    case IMAGE_JPG:
        D3DImageFormat = D3DXIFF_JPG;
        break;

    case IMAGE_TGA:
        D3DImageFormat = D3DXIFF_TGA;
        break;

    case IMAGE_PNG:
        D3DImageFormat = D3DXIFF_PNG;
        break;

    case IMAGE_DDS:
        D3DImageFormat = D3DXIFF_DDS;
        break;

    case IMAGE_PPM:
        D3DImageFormat = D3DXIFF_PPM;
        break;

    case IMAGE_DIB:
        D3DImageFormat = D3DXIFF_DIB;
        break;

    case IMAGE_HDR:
        D3DImageFormat = D3DXIFF_HDR;
        break;

    case IMAGE_PFM:
        D3DImageFormat = D3DXIFF_PFM;
        break;
    }

    return D3DImageFormat;
}

TString128 D3D9Mapping::GetD3DErrorDescription(HRESULT hr)
{
    return DXErrorDescription(hr);
}

TString128 D3D9Mapping::GetD3DErrorString(HRESULT hr)
{
    return DXErrorString(hr);
}

DWORD D3D9Mapping::GetD3DFilterType(TEXTURE_FILTER_TYPE Type)
{
    if (Type == TEXF_DEFAULT)
        Type = RenderHelper::Instance()->GetDefaultTextureFilter();

    switch (Type)
    {
    case TEXF_POINT:
        return D3DTEXF_POINT;

    case TEXF_LINEAR:
        return D3DTEXF_LINEAR;

    case TEXF_ANISOTROPIC:
        return D3DTEXF_ANISOTROPIC;

    default:
        d_assert(0);
        return D3DTEXF_POINT;
    }
}

DWORD D3D9Mapping::GetD3DXFilter(int filter)
{
    DWORD D3DXFilter = 0;

    if (filter == IMAGE_FILTER_DEFAULT)
        D3DXFilter = D3DX_DEFAULT;
    else
    {
        if (filter & IMAGE_FILTER_NONE)
        {
            D3DXFilter |= D3DX_FILTER_NONE;
        }

        if (filter & IMAGE_FILTER_POINT)
        {
            D3DXFilter |= D3DX_FILTER_POINT;
        }

        if (filter & IMAGE_FILTER_LINEAR)
        {
            D3DXFilter |= D3DX_FILTER_LINEAR;
        }

        if (filter & IMAGE_FILTER_TRIANGLE)
        {
            D3DXFilter |= D3DX_FILTER_TRIANGLE;
        }

        if (filter & IMAGE_FILTER_BOX)
        {
            D3DXFilter |= D3DX_FILTER_BOX;
        }

        if (filter & IMAGE_FILTER_BOX)
        {
            D3DXFilter |= D3DX_FILTER_BOX;
        }
    }

    return D3DXFilter;
}


DWORD D3D9Mapping::GetD3DAddressMode(TEXTURE_ADDRESS_MODE Mode)
{
    switch (Mode)
    {
    case TEXA_WRAP:
        return D3DTADDRESS_WRAP;

    case TEXA_MIRROR:
        return D3DTADDRESS_MIRROR;

    case TEXA_CLAMP:
        return D3DTADDRESS_CLAMP;

    case TEXA_BORDER:
        return D3DTADDRESS_BORDER;

    default:
        d_assert(0);
        return D3DTADDRESS_WRAP;
    }
}

USAGE D3D9Mapping::GetUsage(DWORD Usage)
{
    if (Usage & D3DUSAGE_DYNAMIC)
        return USAGE_DYNAMIC;
    else 
        return USAGE_STATIC;
}

int D3D9Mapping::GetLock(DWORD Lock)
{
    int iLock = 0;

    if (Lock & D3DLOCK_DISCARD)
        iLock |= LOCK_DISCARD;
    if (Lock & D3DLOCK_NOOVERWRITE)
        iLock |= LOCK_NOOVERWRITE;
    if (Lock & D3DLOCK_READONLY)
        iLock |= LOCK_READONLY;

    return iLock;
}

FORMAT D3D9Mapping::GetFormat(D3DFORMAT D3DFormat)
{
    switch (D3DFormat)
    {
    case D3DFMT_UNKNOWN:
        return FMT_UNKNOWN;

    case D3DFMT_R8G8B8:
        return FMT_R8G8B8;

    case D3DFMT_A8R8G8B8:
        return FMT_A8R8G8B8;

    case D3DFMT_X8R8G8B8:
        return FMT_X8R8G8B8;

    case D3DFMT_G16R16:
        return FMT_R16G16;

    case D3DFMT_A16B16G16R16:
        return FMT_A16R16G16B16;

    case D3DFMT_L8:
        return FMT_L8;

	case D3DFMT_A8L8:
		return FMT_A8L8;

    case D3DFMT_DXT1:
        return FMT_DXT1;

    case D3DFMT_DXT2:
        return FMT_DXT2;

    case D3DFMT_DXT3:
        return FMT_DXT3;

    case D3DFMT_DXT4:
        return FMT_DXT4;

    case D3DFMT_DXT5:
        return FMT_DXT5;

    case D3DFMT_D32:
        return FMT_D32;

    case D3DFMT_D24S8:
        return FMT_D24S8;

    case D3DFMT_D16:
        return FMT_D16;

    case D3DFMT_D24FS8:
        return FMT_D24FS8;

    case D3DFMT_L16:
        return FMT_L16;

    case D3DFMT_R16F:
        return FMT_R16F;

    case D3DFMT_G16R16F:
        return FMT_R16G16F;

    case D3DFMT_A16B16G16R16F:
        return FMT_A16R16G16B16F;

    case D3DFMT_R32F:
        return FMT_R32F;

    case D3DFMT_G32R32F:
        return FMT_R32G32F;

    case D3DFMT_A32B32G32R32F:
        return FMT_UNKNOWN;

    default:
        d_assert(0 && "Invalid Format.");
        return FMT_UNKNOWN;
    }
}

TEXTURE_FILTER_TYPE D3D9Mapping::GetFilterType(DWORD Type)
{
    switch (Type)
    {
    case D3DTEXF_POINT:
        return TEXF_POINT;

    case D3DTEXF_LINEAR:
        return TEXF_LINEAR;

    case D3DTEXF_ANISOTROPIC:
        return TEXF_ANISOTROPIC;

    default:
        d_assert(0);
        return TEXF_POINT;
    }
}

TEXTURE_ADDRESS_MODE D3D9Mapping::GetAddressMode(DWORD Mode)
{
    switch (Mode)
    {
    case D3DTADDRESS_WRAP:
        return TEXA_WRAP;

    case D3DTADDRESS_MIRROR:
        return TEXA_MIRROR;

    case D3DTADDRESS_CLAMP:
        return TEXA_CLAMP;

    case D3DTADDRESS_BORDER:
        return TEXA_BORDER;

    default:
        d_assert(0);
        return TEXA_WRAP;
    }
}

DECL_USAGE D3D9Mapping::GetDeclUsage(BYTE usage)
{
    switch (usage)
    {
    case D3DDECLUSAGE_POSITION:
        return DU_POSITION;

    case D3DDECLUSAGE_BLENDWEIGHT:
        return DU_BLENDWEIGHT;

    case D3DDECLUSAGE_BLENDINDICES:
        return DU_BLENDINDICES;

    case D3DDECLUSAGE_NORMAL:
        return DU_NORMAL;

    case D3DDECLUSAGE_TEXCOORD:
        return DU_TEXCOORD;

    case D3DDECLUSAGE_TANGENT:
        return DU_TANGENT;

    case D3DDECLUSAGE_BINORMAL:
        return DU_BINORMAL;

    case D3DDECLUSAGE_COLOR:
        return DU_COLOR;

    default:
        d_assert(0 && "Invalid Declaration Usage.");
        return DU_UNKNOWN;
    }
}

DECL_TYPE D3D9Mapping::GetDeclType(BYTE type)
{
    switch (type)
    {
    case D3DDECLTYPE_FLOAT1:
        return DT_FLOAT1;

    case D3DDECLTYPE_FLOAT2:
        return DT_FLOAT2;

    case D3DDECLTYPE_FLOAT3:
        return DT_FLOAT3;

    case D3DDECLTYPE_FLOAT4:
        return DT_FLOAT4;

    case D3DDECLTYPE_D3DCOLOR:
        return DT_COLOR;

    case D3DDECLTYPE_UBYTE4:
        return DT_UBYTE4;

    case D3DDECLTYPE_SHORT2:
        return DT_SHORT2;

    case D3DDECLTYPE_SHORT4:
        return DT_SHORT4;

    default:
        d_assert(0 && "Invalid Declaration Type.");
        return DT_UNKNOWN;
    }
}



D3DMULTISAMPLE_TYPE D3D9Mapping::GetD3DMultiSampleType(MULTI_SAMPLE ms)
{
    switch(ms)
    {
    case MSAA_NONE:
        return D3DMULTISAMPLE_NONE;

    case MSAA_2x:
        return D3DMULTISAMPLE_2_SAMPLES;

    case MSAA_4x:
        return D3DMULTISAMPLE_4_SAMPLES;

    case MSAA_8x:
        return D3DMULTISAMPLE_8_SAMPLES;

    case MSAA_16x:
        return D3DMULTISAMPLE_16_SAMPLES;

    default:
        d_assert(0 && "Invalid Multi Sample Type.");
        return D3DMULTISAMPLE_NONE;
    }
}

MULTI_SAMPLE D3D9Mapping::GetMultiSampleType(D3DMULTISAMPLE_TYPE ms)
{
    switch(ms)
    {
    case D3DMULTISAMPLE_NONE:
        return MSAA_NONE;

    case D3DMULTISAMPLE_2_SAMPLES:
        return MSAA_2x;

    case D3DMULTISAMPLE_4_SAMPLES:
        return MSAA_4x;

    case D3DMULTISAMPLE_8_SAMPLES:
        return MSAA_8x;

    case D3DMULTISAMPLE_16_SAMPLES:
        return MSAA_16x;

    default:
        d_assert(0 && "Invalid Multi Sample Type.");
        return MSAA_NONE;
    }
}

TString128 D3D9Mapping::GetMSAAString(MULTI_SAMPLE msaa)
{
    switch(msaa)
    {
    case MSAA_NONE:
        return "MSAA_NONE";

    case MSAA_2x:
        return "MSAA_2x";

    case MSAA_4x:
        return "MSAA_4x";

    case MSAA_8x:
        return "MSAA_8x";

    case MSAA_16x:
        return "MSAA_16x";

    default:
        d_assert(0 && "Invalid Multi Sample Type.");
       return "";
    }
}

TString128 D3D9Mapping::GetUsageString(USAGE Usage)
{
    TString128 sUsage = "";

    if (Usage == USAGE_DYNAMIC)
        sUsage = "USAGE_DYNAMIC";
    else
        sUsage = "USAGE_STATIC";

    return sUsage;
}

TString128 D3D9Mapping::GetFormatString(FORMAT Format)
{
    switch (Format)
    {
    case FMT_UNKNOWN:
        return "FMT_UNKNOWN";

    case FMT_R8G8B8:
        return "FMT_R8G8B8";

    case FMT_A8R8G8B8:
        return "FMT_A8R8G8B8";

    case FMT_X8R8G8B8:
        return "FMT_X8R8G8B8";

    case FMT_R16G16:
        return "FMT_R16G16";

    case FMT_A16R16G16B16:
        return "FMT_A16R16G16B16";

    case FMT_L8:
        return "FMT_L8";

    case FMT_DXT1:
        return "FMT_DXT1";

    case FMT_DXT2:
        return "FMT_DXT2";

    case FMT_DXT3:
        return "FMT_DXT3";

    case FMT_DXT4:
        return "FMT_DXT4";

    case FMT_DXT5:
        return "FMT_DXT5";

    case FMT_D32:
        return "FMT_D32";

    case FMT_D24S8:
        return "FMT_D24S8";

    case FMT_D16:
        return "FMT_D16";

    case FMT_D24FS8:
        return "FMT_D24FS8";

    case FMT_L16:
        return "FMT_L16";

    case FMT_R16F:
        return "FMT_R16F";

    case FMT_R16G16F:
        return "FMT_R16G16F";

    case FMT_A16R16G16B16F:
        return "FMT_A16R16G16B16F";

    case FMT_R32F:
        return "FMT_R32F";

    case FMT_R32G32F:
        return "FMT_R32G32F";

    case FMT_A32R32G32B32F:
        return "FMT_A32R32G32B32F";

    default:
        d_assert(0 && "Invalid Format.");
        return "FMT_UNKNOWN";
    }
}

TString128 D3D9Mapping::GetShaderProfileString(SHADER_PROFILE profile)
{
    switch (profile)
    {
    case SP_VS:
        return "vs_3_0";

    case SP_PS:
        return "ps_3_0";

    default:
        return "unknown";
    }
}

}
