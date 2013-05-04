#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWD3D11Helper.h"
#include "MWVertexDeclaration.h"
#include "MWD3D11ShaderProgram.h"

namespace Myway {

class RS_ENTRY D3D11VertexDeclaration : public VertexDeclaration
{
    DeclareRefObj();

   friend class D3D11VideoBufferManager;
   friend class D3D11RenderSystem;

   struct ILayoutPair
   {
	   D3D11_VertexShader * shader;
	   ID3D11InputLayout * layout;
   };

public: 
	D3D11VertexDeclaration();
    ~D3D11VertexDeclaration();

    void Init();

    ID3D11InputLayout * GetD3DInputLayout(D3D11_VertexShader * shader);

protected:
	Array<ILayoutPair> mInputLayoutArray;
};

}
