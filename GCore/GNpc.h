#pragma once

#include "GActor.h"
#include "GData.h"

namespace game {

	class GCORE_ENTRY GNpc : public GActor
	{
		DECLARE_ALLOC();

	public:
		static const int K_Type = 'GNPC';

	public:
		GNpc(int uid, int templateId);
		virtual ~GNpc();

		virtual int GetUId() const { return mUId; }

		virtual int GetType() const { return K_Type; };

	protected:
		void _init(int templateId);

	protected:
		const GNpcInfo * mNpcInfo;
		int mUId;
	};

}