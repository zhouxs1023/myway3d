#include "MWD3D11Helper.h"

namespace Myway {

	DXGI_FORMAT D3D11Helper::GetD3DFormat(FORMAT fmt)
	{
		switch (fmt)
		{
		case FMT_UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;

		case FMT_R8G8B8:
		case FMT_A8R8G8B8:
		case FMT_X8R8G8B8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case FMT_R16G16:
			return DXGI_FORMAT_R16G16_UNORM;

		case FMT_A16R16G16B16:
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case FMT_L8:
			return DXGI_FORMAT_R8_UNORM;

		case FMT_L16:
			return DXGI_FORMAT_R16_UNORM;

		case FMT_DXT1:
			return DXGI_FORMAT_BC1_UNORM;

		case FMT_DXT2:
			return DXGI_FORMAT_BC2_UNORM;

		case FMT_DXT3:
			return DXGI_FORMAT_BC3_UNORM;

		case FMT_DXT4:
			return DXGI_FORMAT_BC4_UNORM;

		case FMT_DXT5:
			return DXGI_FORMAT_BC5_UNORM;

		case FMT_D32:
			return DXGI_FORMAT_D32_FLOAT;

		case FMT_D24S8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;

		case FMT_D16:
			return DXGI_FORMAT_D16_UNORM;

		case FMT_D24FS8:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

		case FMT_R16F:
			return DXGI_FORMAT_R16_FLOAT;

		case FMT_R16G16F:
			return DXGI_FORMAT_R16G16_FLOAT;

		case FMT_A16R16G16B16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case FMT_R32F:
			return DXGI_FORMAT_R32_FLOAT;

		case FMT_R32G32F:
			return DXGI_FORMAT_R32G32_FLOAT;

		case FMT_A32R32G32B32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		default:
			d_assert(0 && "Invalid Format.");
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	FORMAT D3D11Helper::GetFormat(DXGI_FORMAT fmt)
	{
		switch (fmt)
		{
		case DXGI_FORMAT_UNKNOWN:
			return FMT_UNKNOWN;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return FMT_A8R8G8B8;

		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return FMT_A16R16G16B16;

		case DXGI_FORMAT_R8_UNORM:
			return FMT_L8;

		case DXGI_FORMAT_BC1_UNORM:
			return FMT_DXT1;

		case DXGI_FORMAT_BC2_UNORM:
			return FMT_DXT2;

		case DXGI_FORMAT_BC3_UNORM:
			return FMT_DXT3;

		case DXGI_FORMAT_BC4_UNORM:
			return FMT_DXT4;

		case DXGI_FORMAT_BC5_UNORM:
			return FMT_DXT5;

		case DXGI_FORMAT_D32_FLOAT:
			return FMT_D32;

		case DXGI_FORMAT_D16_UNORM:
			return FMT_D16;

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return FMT_D24S8;

		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			return FMT_D24FS8;

		case DXGI_FORMAT_R16_FLOAT:
			return FMT_R16F;

		case DXGI_FORMAT_R16G16_FLOAT:
			return FMT_R16G16F;

		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return FMT_A16R16G16B16F;

		case DXGI_FORMAT_R32_FLOAT:
			return FMT_R32F;

		case DXGI_FORMAT_R32G32_FLOAT:
			return FMT_R32G32F;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return FMT_A32R32G32B32F;

		default:
			d_assert(0 && "Invalid Format.");
			return FMT_UNKNOWN;
		}
	}


	CGprofile D3D11Helper::GetCgProfile(SHADER_PROFILE Profile)
	{
		switch (Profile)
		{
		case SP_VS:
			//return CG_PROFILE_VS_3_0;
			return cgGetProfile("vs_5_0");

		case SP_PS:
			//return CG_PROFILE_PS_3_0;
			return cgGetProfile("ps_5_0");

		default:
			//not support
			return CG_PROFILE_UNKNOWN;
		}
	}


	const char * D3D11Helper::GetSemanticName(DECL_USAGE usage)
	{
		static const TString32 S_SemanticName[] = {
			"POSITION",
			"BLENDWEIGHT"
			"BLENDINDICES"
			"NORMAL",
			"TEXCOORD",
			"TANGENT",
			"BINORMAL"
			"COLOR",
		};

		d_assert (usage != DU_UNKNOWN);

		return S_SemanticName[usage].c_str();
	}

	DXGI_FORMAT	D3D11Helper::GetSemanticFormat(DECL_TYPE type)
	{
		d_assert (type != DT_UNKNOWN);

		switch (type)
		{
		case DT_FLOAT1:
			return DXGI_FORMAT_R32_FLOAT;

		case DT_FLOAT2:
			return DXGI_FORMAT_R32G32_FLOAT;

		case DT_FLOAT3:
			return DXGI_FORMAT_R32G32B32_FLOAT;

		case DT_FLOAT4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case DT_COLOR:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case DT_UBYTE4:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case DT_SHORT2:
			return DXGI_FORMAT_R16G16_UNORM;

		case DT_SHORT4:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		}

		d_assert (0);

		return DXGI_FORMAT_UNKNOWN;
	}

}
