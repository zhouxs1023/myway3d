#pragma once

#include "MOpcodeEntry.h"
#include "Myway.h"

namespace Myway
{

	class MOpcodePlugin : public Plugin
	{
	public:
		MOpcodePlugin();
		virtual ~MOpcodePlugin();

		virtual void Install();
		virtual void Uninstall();
	};

}