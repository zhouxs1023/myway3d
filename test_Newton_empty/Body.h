#pragma once

#include "Newton.h"
#include "Myway.h"
#include "Shape.h"

namespace Myway {

	namespace Newton {

		typedef NewtonBody ntBody;

		ntBody * CreateRigidBody(SceneNode * node, ntShape * shape, float mass);
		void DestroyRigidBody(ntBody * body);
	}
}