#pragma once

namespace game {

	struct GNpcInfo
	{
		int id;
		char name[32];

		char part_Main[32];
		char part_Weapon[32];
		char part_Helmet[32];
		char part_Shoulder[32];
		char part_Clothes[32];
		char part_Shoes[32];
	};



	

	typedef Value<32> GValue;

	class GDataTable
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

	class GDataManager
	{
		DECLARE_SINGLETON(GDataManager);

	public:
		GDataManager();
		~GDataManager();

		void LoadTabel(const char * name);
		GDataTable * GetTable(const char * name);

		const GNpcInfo * GetNpcInfo(int id);

	protected:
		Array<GDataTable *> mTables;
		Array<GNpcInfo> mNpcInfos;
	};

}