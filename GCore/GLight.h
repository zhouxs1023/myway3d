#pragma once

#include "GEntity.h"

namespace game {

	class GLight : public GEntity
	{
	public:
		GLight(const char * name);
		virtual ~GLight();

		Light * getLight();

	protected:
		Light * mLight;
	};

}