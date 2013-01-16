#include "MWRenderDefine.h"

namespace Myway {

	IMP_ENUM(eSmaaType);

	DF_ENUM_BEGIN(eSmaaType)
		DF_ENUM(SMAA_NONE)
		DF_ENUM(SMAA_LOW)
		DF_ENUM(SMAA_MEDIUM)
		DF_ENUM(SMAA_HIGH)
		DF_ENUM(SMAA_ULTRA)
	DF_ENUM_END()


	IMP_ENUM(eRenderTechType);

	DF_ENUM_BEGIN(eRenderTechType)
		DF_ENUM(RTT_Base)
		DF_ENUM(RTT_Mirror)
		DF_ENUM(RTT_Shadow)
	DF_ENUM_END()
}

