//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "Infinite.h"

namespace Infinite {

	class INFI_ENTRY iPlugin
	{
	public:
		enum eUsage
		{
			eImport = 1,
			eExport = 2,
			eEditor = 4,
		};

	public:
		iPlugin() {}
		virtual ~iPlugin() {}

		virtual const char * getName() = 0;
		virtual const char * GetExpExtern() { return ""; }
		virtual const char * GetImpExtern() { return ""; }

		virtual void Import(const char * filename) {}
		virtual void Export(const char * filename) {}

		virtual int GetUsages() = 0;
	};
}