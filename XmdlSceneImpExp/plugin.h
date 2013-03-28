#pragma once

#include "Editor.h"

namespace xmdl {

	class t_plugin : public Infinite::iPlugin
	{
	public:
		t_plugin();
		~t_plugin();

		virtual const char * getName() { return "xmdl import"; }
		virtual const char * GetExpExtern() { return ""; }
		virtual const char * GetImpExtern() { return "stg"; }

		virtual int GetUsages() { return Infinite::iPlugin::eImport; }

		virtual void Import(const char * filename);

	protected:
				void _convertXMDL(const char * filename);
	};
}