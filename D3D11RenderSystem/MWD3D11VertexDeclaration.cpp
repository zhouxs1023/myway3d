#include "MWD3D11VertexDeclaration.h"
#include "MWD3D11Driver.h"

namespace Myway
{
	D3D11VertexDeclaration::D3D11VertexDeclaration()
	{
	}

	D3D11VertexDeclaration::~D3D11VertexDeclaration()
	{
		for (int i = 0; i < mInputLayoutArray.Size(); ++i)
		{
			mInputLayoutArray[i].layout->Release();
		}

		mInputLayoutArray.Clear();
	}

	void D3D11VertexDeclaration::DeleteSelf()
	{
		VideoBufferManager::Instance()->DestroyVertexDeclaration(this);
	}

	void D3D11VertexDeclaration::Init()
	{
	}

	ID3D11InputLayout * D3D11VertexDeclaration::GetD3DInputLayout(D3D11_VertexShader * shader)
	{
		d_assert (mElementCount > 0);

		for (int i = 0; i < mInputLayoutArray.Size(); ++i)
		{
			if (mInputLayoutArray[i].shader == shader)
				return mInputLayoutArray[i].layout;
		}

		ID3D11Device * device = D3D11Driver::Instance()->Device();

		D3D11_INPUT_ELEMENT_DESC descs[MAX_VERTEX_STREAM];

		for (int i = 0; i < mElementCount; ++i)
		{
			descs[i].SemanticName = D3D11Helper::GetSemanticName(mElements[i].Usage);
			descs[i].SemanticIndex = mElements[i].UsageIndex;
			descs[i].Format = D3D11Helper::GetSemanticFormat(mElements[i].Type);
			descs[i].InputSlot = mElements[i].Stream;
			descs[i].AlignedByteOffset = mElements[i].Offset;
			descs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			descs[i].InstanceDataStepRate = 0;
		}

		ID3D11InputLayout * layout = NULL;

		device->CreateInputLayout(descs, mElementCount,
			shader->GetMircoCode()->GetBufferPointer(),
			shader->GetMircoCode()->GetBufferSize(),
			&layout);

		d_assert (layout != NULL);

		ILayoutPair pair = {shader, layout};

		mInputLayoutArray.PushBack(pair);

		return layout;
	}
}
