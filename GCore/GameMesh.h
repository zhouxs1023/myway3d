#pragma once

#include "GameEntity.h"

namespace Game {


	class GameMesh : public IGameEntity
	{
	public:
		GameMesh(const char * sMeshFile);
		virtual ~GameMesh();

	protected:
		Entity * mEntity;
	};
}