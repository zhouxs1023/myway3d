#include "stdafx.h"

#include "GNpc.h"

namespace game {

	GNpc::GNpc(int id, const char * name, const char * source)
		: GActor(name)
		, mId(id)
	{
		
	}

	GNpc::~GNpc()
	{
	}

	void GNpc::_init(const char * source)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		xml_doc doc;

		XmlHelper::LoadXmlFromMem(doc, stream->GetData());

		doc->first_node()
	}
}