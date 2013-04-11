#pragma once

#include "GEntity.h"

namespace game {

	class GMesh : public GEntity
	{
	public:
		GMesh(const char * name, const char * meshFile);
		virtual ~GMesh();

	protected:
		Entity * mEntity;
	};
}