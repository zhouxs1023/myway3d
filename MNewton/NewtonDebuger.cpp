#include "NewtonDebuger.h"
#include "NewtonBody.h"

namespace Myway {

	namespace Newton {

		tDebugger::tDebugger(tBody * body)
			: OnTransform(&body->OnTransform, this, &tDebugger::_OnTransform)
		{
			d_assert (0);
		}

		tDebugger::~tDebugger()
		{
		}

		void tDebugger::_OnTransform(Event * sender)
		{
			Vec3 pos = *(Vec3 *)sender->GetParam(0);
			Quat quat = *(Quat *)sender->GetParam(1);

			mRenderOp.xform.MakeTransform(pos, quat, Vec3::Unit);
		}

	}

}