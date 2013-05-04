#include "MWD3D11ShaderProgram.h"

namespace Myway {


	D3D11_VertexShader::D3D11_VertexShader()
		: mD3D11VertexShader(NULL)
		, mD3D11MicroCode(NULL)
	{
	}

	D3D11_VertexShader::~D3D11_VertexShader()
	{
		safe_release (mD3D11VertexShader);
	}


	D3D11_PixelShader::D3D11_PixelShader()
		: mD3D11PixelShader(NULL)
		, mD3D11MicroCode(NULL)
	{
	}

	D3D11_PixelShader::~D3D11_PixelShader()
	{
		safe_release (mD3D11PixelShader);
	}


}