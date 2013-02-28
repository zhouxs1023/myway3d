#pragma once

#include "NewtonPhysics.h"

namespace Myway {

	class Player
	{
	public:
		Player();
		~Player() {}

		void Init();
		void Shutdown();

		void Update();

	protected:
		Newton::tPlayerController * mController;
		Newton::tShape * mShape;

		SceneNode * mSceneNode;
		Entity * mEntity;
	};
}