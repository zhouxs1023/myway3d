#include "MOpcodeTerrain.h"

namespace Myway {

	MOpcodeTerrain::MOpcodeTerrain(void * uid, float scale)
		: MOpcodeColObj(uid, scale)
	{
	}

	MOpcodeTerrain::~MOpcodeTerrain()
	{
	}

	void MOpcodeTerrain::DeleteSelf()
	{
		d_assert (0);
	}

	void MOpcodeTerrain::Build()
	{
	}

}