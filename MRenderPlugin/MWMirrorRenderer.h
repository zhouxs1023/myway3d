#pragma once

#include "MRenderEntry.h"
#include "MWRenderScheme.h"
#include "MWRenderTarget.h"
#include "MWDepthStencil.h"
#include "MWWorld.h"

namespace Myway {

	class MirrorRenderQueue : public RenderQueue
	{
		DECLARE_ALLOC();

	public:
		MirrorRenderQueue () {}
		virtual ~MirrorRenderQueue () {}

		virtual void PushRenderer(const List<SceneNode *> & nodes)
		{
			List<SceneNode *>::ConstIterator whr = nodes.Begin();
			List<SceneNode *>::ConstIterator end = nodes.End();

			while (whr != end)
			{
				_pushRenderer(*whr);

				++whr;
			}
		}

		virtual void AddRenderer(Renderer * obj)
		{
			if (!obj->GetMaterial()->IsTransparency())
			{
				mSolidEntry.PushBack(obj);
			}
		}

	protected:
		void _pushRenderer(SceneNode * node)
		{
			SceneNode::MoverVisitor vr = node->GetMovers();

			while (!vr.Endof())
			{
				Mover * m = *vr.Cursor();

				if (m->IsVisible() && m->IsMirrorEnable())
					m->AddRenderQueue(this);

				++vr;
			}
		}
	};


	class MirrorRenderLoop
	{
		friend class DeferredRenderer;

		DECLARE_ALLOC();

	public:
		MirrorRenderLoop(DeferredRenderer * sch);
		~MirrorRenderLoop();

		void DoRender(Camera * cam);

	protected:
		DeferredRenderer * mScheme;
	};

}