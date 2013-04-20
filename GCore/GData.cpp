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




	IMP_SLN(GDataManager);

	GDataManager::GDataManager()
	{
		INIT_SLN;
	}

	GDataManager::~GDataManager()
	{
		SHUT_SLN;
	}

	void GDataManager::Init()
	{
	}

	void GDataManager::Shutdown()
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




	void _loadNpcPart(GNpcInfo & info, xml_node * node)
	{
		xml_node * nd_main = node->first_node("main");
		xml_node * nd_weapon = node->first_node("weapon");
		xml_node * nd_helmet = node->first_node("helmet");
		xml_node * nd_shoulder = node->first_node("shoulder");
		xml_node * nd_clothes = node->first_node("clothes");
		xml_node * nd_shoes = node->first_node("shoes");

		d_assert (nd_main != NULL);

		info.part_main = nd_main->first_attribute("mesh")->value();

		if (nd_weapon)
			info.part_weapon = nd_weapon->first_attribute("mesh")->value();

		if (nd_helmet)
			info.part_helmet = nd_helmet->first_attribute("helmet")->value();

		if (nd_shoulder)
			info.part_shoulder = nd_helmet->first_attribute("shoulder")->value();

		if (nd_shoes)
			info.part_shoes = nd_helmet->first_attribute("shoes")->value();
	}

	void GDataManager::_loadNpcInfo()
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource("NpcInfo.xml");

		xml_doc doc;

		XmlHelper::LoadXmlFromMem(doc, (char *)stream->GetData());

		xml_node * root = doc.first_node("npcs");
		xml_node * node = root->first_node("npc");

		while (node)
		{
			GNpcInfo info;

			xml_attri * nd_id = node->first_attribute("id");
			xml_attri * nd_name = node->first_attribute("name");
			
			info.id = atoi(nd_id->value());
			info.name = nd_name->value();

			xml_node * nd_part = node->first_node("part");

			_loadNpcPart(info, nd_part);

			mNpcInfos.PushBack(info);

			node = node->next_sibling();
		}
	}

}