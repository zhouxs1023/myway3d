#pragma once

#include "GCoreEntry.h"

namespace game {

	struct GCORE_ENTRY GHelper {
		static const TString32 S_Anim_Idle0;
		static const TString32 S_Anim_Idle1;
		static const TString32 S_Anim_Walk;
		static const TString32 S_Anim_Run;
	};
	




	struct GCORE_ENTRY GNpcInfo
	{
		int id;
		TString32 name;

		TString32 part_main;
		TString32 part_weapon;
		TString32 part_helmet;
		TString32 part_shoulder;
		TString32 part_clothes;
		TString32 part_shoes;

		TString32 anim_Idle0;
		TString32 anim_Idle1;
		TString32 anim_Walk;
		TString32 anim_Run;

		GNpcInfo()
		{
			id = -1;
		}
	};




	typedef Value<32> GValue;

	class GCORE_ENTRY GDataTable
	{
		DECLARE_ALLOC();

	public:
		GDataTable(const TString128 & name, int rows, int columns);
		~GDataTable();

		void SetData(int x, int y, const GValue & val);
		const GValue & GetData(int x, int y) const;

		const TString128 & GetName() const { return mName; }
		int GetRows() const { return mRows; }
		int GetColumns() const { return mColumns; }

	protected:
		TString128 mName;

		int mRows;
		int mColumns;

		GValue * mData;
	};

	class GCORE_ENTRY GDataManager
	{
		DECLARE_SINGLETON(GDataManager);

	public:
		GDataManager();
		~GDataManager();

		void Init();
		void Shutdown();

		void LoadTabel(const char * name);
		GDataTable * GetTable(const char * name);

		const GNpcInfo * GetNpcInfo(int id);

	protected:
		void _loadNpcInfo();

	protected:
		Array<GDataTable *> mTables;
		Array<GNpcInfo> mNpcInfos;
	};

}