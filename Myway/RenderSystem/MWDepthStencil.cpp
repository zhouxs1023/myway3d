#include "MWDepthStencil.h"
#include "Engine.h"

using namespace Myway;

DepthStencil::DepthStencil()
{
}

DepthStencil::~DepthStencil()
{
}

inline const TString128 & DepthStencil::GetName()
{
	return mName;
}

inline int DepthStencil::GetWidth()
{
	if (mWidth != -1)
		return mWidth;
	else
		return Engine::Instance()->GetDeviceProperty()->Width;
}

inline int DepthStencil::GetHeight()
{
	if (mHeight != -1)
		return mHeight;
	else
		return Engine::Instance()->GetDeviceProperty()->Height;
}

inline FORMAT DepthStencil::GetFormat()
{
	return mFormat;
}

inline MULTI_SAMPLE DepthStencil::GetMultiSampleType()
{
	return mMSAA;
}
