#include "MOpcodeTerrain.h"
#include "MWEnvironment.h"

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
		Terrain * terrain = Environment::Instance()->GetTerrain();

		d_assert (terrain);
	}

}