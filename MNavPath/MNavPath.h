//////////////////////////////////////////////////////////////////////////
//
// Path Engine.
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "MNavEntry.h"
#include "MWMath.h"
#include "MWSerializer.h"

namespace Myway {

	class MNAV_ENTRY NavPath
	{
		DECLARE_ALLOC ();

	public:
		NavPath();
		virtual ~NavPath();

		void Resize(int w, int h);

		void Set(int x, int y);
		void Clear(int x, int y);
		char Get(int x, int y) const;

		void SetHeight(int x, int y, float h);
		float GetHeight(int x, int y) const;

		const Size2 & GetSize() const;

		void Serialize(ISerializer & serializer);
		
	protected:
		Size2 mSize;
		char * mData;
		float * mHeight;
	};

}