#pragma once

namespace Game {

	class cMainMode : public GmMode
	{
	public:
		cMainMode();
		virtual ~cMainMode();

		virtual void Init();
		virtual void Shutdown();

	protected:
		GmMap mMap;
	};

}