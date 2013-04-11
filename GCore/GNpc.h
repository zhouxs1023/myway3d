#pragma once

#include "GActor.h"

namespace game {

	class GNpc : public GActor
	{
	public:
		GNpc(int id, const char * name, const char * source);
		virtual ~GNpc();

	protected:
		void _init(const char * source);

	protected:
		int mId;
	};

}