//////////////////////////////////////////////////////////////////////////
//
// Newton Integration.
//   newton physics engine: http://newtondynamics.com/forum/newton.php
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "NewtonEntry.h"

namespace Myway {

	namespace Newton {

		class tBody;

		class tDebugger
		{
		public:
			tDebugger(tBody * body);
			~tDebugger();

			RenderOp * GetRenderOp() { return &mRenderOp; }

		protected:
			void _OnTransform(Event * sender);

		protected:
			RenderOp mRenderOp;

			tEventListener<tDebugger> OnTransform;
		};

	}

}