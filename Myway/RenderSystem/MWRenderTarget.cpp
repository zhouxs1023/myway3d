#include "MWRenderTarget.h"
#include "Engine.h"

using namespace Myway;

RenderTarget::RenderTarget()
: mMSAA(MSAA_NONE)
{
}

RenderTarget::~RenderTarget()
{
}

const TString128 & RenderTarget::GetName() const
{ 
	return mName;
}

int RenderTarget::GetWidth() const
{ 
	if (mWidth != -1)
		return mWidth;
	else
		return Engine::Instance()->GetDeviceProperty()->Width;
}

int RenderTarget::GetHeight() const
{
	if (mHeight != -1)
		return mHeight;
	else
		return Engine::Instance()->GetDeviceProperty()->Height;
}

FORMAT RenderTarget::GetFormat() const
{ 
	return mFormat;
}

MULTI_SAMPLE RenderTarget::GetMultiSampleType() const
{ 
	return mMSAA;
}