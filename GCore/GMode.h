#pragma once

namespace game {

	class GMode
	{
	public:
		GMode() {}
		virtual ~GMode() {}

		virtual int GetType() = 0;
		virtual void OnInit() = 0;
		virtual void OnRun() = 0;
		virtual void OnShutdown() = 0;

		virtual void OnWndMessage(UINT uMsg, LPARAM lParam, WPARAM wParam) = 0;
	};

}