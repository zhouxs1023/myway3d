#pragma once

namespace MotionFX {

	class BackCell
	{
	public:
		BackCell();
		~BackCell();

		void Init();
		void Shutdown();

	protected:
		void _Render(Event * _sender);

	protected:
		RenderOp * mRenderOp;
		Technique * mTech;

		tEventListener<BackCell> OnRender;
	};

}