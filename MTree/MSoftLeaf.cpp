#include "MSoftLeaf.h"

namespace Myway {

	MSoftLeaf::MSoftLeaf()
	{
	}

	MSoftLeaf::~MSoftLeaf()
	{
	}

	void MSoftLeaf::Do()
	{
		RenderTarget * oldRT = RenderSystem::Instance()->GetRenderTarget(0);

		_downSample();
		_blur();

		RenderSystem::Instance()->SetRenderTarget(0, oldRT);

		_blend();
	}

	void MSoftLeaf::_resize(void *, void *)
	{
	}

	void MSoftLeaf::_downSample()
	{
	}

	void MSoftLeaf::_blur()
	{
	}

	void MSoftLeaf::_blend()
	{
	}
}