#pragma once

#include "MWD3D11Helper.h"

namespace Myway {

	class RS_ENTRY D3D11_VertexShader : public ShaderProgram
	{
		friend class D3D11ShaderProgramManager;

	public:
		D3D11_VertexShader();
		virtual ~D3D11_VertexShader();

		ID3D11VertexShader * GetD3DVertexShader() { return mD3D11VertexShader; }
		ID3D10Blob * GetMircoCode() { return mD3D11MicroCode; }

	protected:
		ID3D11VertexShader * mD3D11VertexShader;
		ID3D10Blob * mD3D11MicroCode;
	};

	class RS_ENTRY D3D11_PixelShader : public ShaderProgram
	{
		friend class D3D11ShaderProgramManager;

	public:
		D3D11_PixelShader();
		virtual ~D3D11_PixelShader();

		ID3D11PixelShader * GetD3DVertexShader() { return mD3D11PixelShader; }
		ID3D10Blob * GetMircoCode() { return mD3D11MicroCode; }

	protected:
		ID3D11PixelShader * mD3D11PixelShader;
		ID3D10Blob * mD3D11MicroCode;
	};

}