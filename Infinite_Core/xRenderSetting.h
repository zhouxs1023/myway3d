//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

namespace Infinite {

class xRenderSetting : public IPropertyObj
{
	DECLARE_SINGLETON(xRenderSetting);
	DECLARE_PROPERTY(IPropertyObj);

public:
	bool SSAOEnable;
	bool GodRayEnable;
	bool ShadowEnable;
	bool ColorSharpEnable;
	bool HDREnable;
	bool SoftLeafEnable;
	int  SMAAType;

public:
	xRenderSetting();
	~xRenderSetting();

	bool OnPropertyChanged(const Property * p);

protected:
	void _OnSerialize(Event * sender);

protected:
	tEventListener<xRenderSetting> OnSerialize;
};

}