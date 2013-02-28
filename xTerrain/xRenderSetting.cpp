#include "stdafx.h"

#include "xRenderSetting.h"
#include "xEvent.h"
#include "xScene.h"
#include "xSerializer.h"

IMP_SLN(xRenderSetting);

xRenderSetting gRenderSetting;

DF_PROPERTY_BEGIN(xRenderSetting)
	DF_PROPERTY(xRenderSetting, SSAOEnable, "Generic", "SSAO", PT_Bool, sizeof(bool))
	DF_PROPERTY(xRenderSetting, GodRayEnable, "Generic", "GodRay", PT_Bool, sizeof(bool))
	DF_PROPERTY(xRenderSetting, ShadowEnable, "Generic", "Shadow", PT_Bool, sizeof(bool))
	DF_PROPERTY(xRenderSetting, HDREnable, "Generic", "HDR", PT_Bool, sizeof(bool))
	DF_PROPERTY(xRenderSetting, ColorSharpEnable, "Generic", "ColorSharp", PT_Bool, sizeof(bool))
	DF_PROPERTY(xRenderSetting, SoftLeafEnable, "Generic", "SoftLeaf", PT_Bool, sizeof(bool))
	DF_PROPERTY_ENUM(xRenderSetting, SMAAType, "Generic", "SMAA", PT_Int, sizeof(int), eSmaaType::Instance())
DF_PROPERTY_END()

xRenderSetting::xRenderSetting()
	: OnSerialize(&xEvent::OnSerialize, this, &xRenderSetting::_OnSerialize) 
	, SSAOEnable(false)
	, GodRayEnable(false)
	, ShadowEnable(false)
	, ColorSharpEnable(false)
	, HDREnable(false)
	, SoftLeafEnable(false)
	, SMAAType(eSmaaType::SMAA_NONE)
{
	INIT_SLN;
}

xRenderSetting::~xRenderSetting()
{
	SHUT_SLN;
}

bool xRenderSetting::OnPropertyChanged(const Property * p)
{
	if (!xScene::Instance()->IsInited())
		return true;

	Environment::Instance()->SetSSAOEnable(SSAOEnable);
	Environment::Instance()->SetGodRayEnable(GodRayEnable);
	Environment::Instance()->SetShadowEnable(ShadowEnable);
	Environment::Instance()->SetColorSharpEnable(ColorSharpEnable);
	Environment::Instance()->SetHDREnable(HDREnable);
	Environment::Instance()->SetSoftLeafEnable(SoftLeafEnable);
	RenderSystem::Instance()->SetSMAAType((eSmaaType::enum_t)SMAAType);

	return true;
}

void xRenderSetting::_OnSerialize(Event * sender)
{
	const int K_ChunkId = 'xRS0';
	int K_Version = 0;

	int chunkId = *(int *)sender->GetParam(0);
	xSerializer & Serializer = *(xSerializer *)sender->GetParam(1);

	if (Serializer.IsSave())
	{
		Serializer << K_ChunkId;
		Serializer << K_Version;
		Serializer << this;
	}
	else if (chunkId == K_ChunkId)
	{
		Serializer >> K_Version;

		d_assert(K_Version == 0);

		Serializer >> this;
	}
}
