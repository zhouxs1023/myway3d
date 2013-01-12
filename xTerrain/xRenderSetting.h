#pragma once


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
	int  SMAAType;

public:
	xRenderSetting();
	~xRenderSetting();

	bool OnPropertyChanged(const Property * p);

protected:
	void _OnSerialize(void * param0, void * param1);

protected:
	tEventListener<xRenderSetting> OnSerialize;
};