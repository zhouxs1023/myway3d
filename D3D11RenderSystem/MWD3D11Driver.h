#pragma once

#include "MWD3D11RenderSystemEntry.h"

namespace Myway {

	class RS_ENTRY D3D11Driver
	{
		DECLARE_SINGLETON(D3D11Driver);

	public:
		D3D11Driver();
		~D3D11Driver();

		inline ID3D11Device * Device() { return mDevice; }
		inline ID3D11DeviceContext * Context() { return mContext; }

	protected:
		ID3D11Device * mDevice;
		ID3D11DeviceContext * mContext;
	};

}