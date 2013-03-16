#pragma once

namespace Infinite {

	class xEnvironment
	{
	public:
		static Event OnChanged;

	public:
		xEnvironment();
		~xEnvironment();

	protected:
		void _OnNewScene(Event * sender);
		void _OnLoadScene(Event * sender);
		void _OnUnloadScene(Event * sender);
		void _OnSerialize(Event * sender);

	protected:
		tEventListener<xEnvironment> OnNewScene;
		tEventListener<xEnvironment> OnUnloadScene;
		tEventListener<xEnvironment> OnLoadScene;
		tEventListener<xEnvironment> OnSerialize;
	};

}
