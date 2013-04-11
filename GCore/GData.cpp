#include "stdafx.h"

#include "GData.h"

namespace game {

	GDataTable::GDataTable(const TString128 & name, int rows, int columns)
	{
		d_assert (rows > 0 && columns > 0);

		mName = name;
		mRows = rows;
		mColumns = columns;
		mData = new GValue[rows * columns];
	}

	GDataTable::~GDataTable()
	{
		delete[] mData;
	}

	void GDataTable::SetData(int x, int y, const GValue & val)
	{
		d_assert (y < mColumns && x < mRows);

		mData[y * mRows + x] = val;
	}

	const GValue & GDataTable::GetData(int x, int y) const
	{
		d_assert (y < mColumns && x < mRows);

		return mData[y * mRows + x];
	}






	GDataManager::GDataManager()
	{
	}

	GDataManager::~GDataManager()
	{
	}

	void GDataManager::LoadTabel(const char * name)
	{
	}

	GDataTable * GDataManager::GetTable(const char * name)
	{
		for (int i = 0; i < mTables.Size(); ++i)
		{
			if (mTables[i]->GetName() == name)
				return mTables[i];
		}

		return NULL;
	}

	const GNpcInfo * GDataManager::GetNpcInfo(int id)
	{
		for (int i = 0; i < mNpcInfos.Size(); ++i)
		{
			if (mNpcInfos[i].id == id)
				return &mNpcInfos[i];
		}

		return NULL;
	}

}