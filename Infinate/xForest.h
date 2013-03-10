#pragma once

namespace Infinite {

	class xForest
	{
	public:
		xForest();
		~xForest();

	protected:
		void _OnNewScene(Event * sender);
		void _OnLoadScene(Event * sender);
		void _OnUnloadScene(Event * sender);
		void _OnSerialize(Event * sender);

	protected:
		tEventListener<xForest> OnNewScene;
		tEventListener<xForest> OnUnloadScene;
		tEventListener<xForest> OnLoadScene;
		tEventListener<xForest> OnSerialize;
	};

}
