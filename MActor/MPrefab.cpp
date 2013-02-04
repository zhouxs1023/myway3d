#include "MPrefab.h"
#include "MActorManager.h"
#include "MWResourceManager.h"

namespace Myway {

	MPrefab::MPrefab(const TString128 & name)
	{
		mName = name;
		mRoot = World::Instance()->CreateSceneNode();
	}

	MPrefab::~MPrefab()
	{
		Unload();

		World::Instance()->DestroySceneNode(mRoot);
	}

	void MPrefab::Load(const TString128 & source)
	{
		Unload();

		mSource = source;

		xml_doc doc;

		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source.c_str());

		d_assert (stream != NULL);

		XmlHelper::LoadXmlFromMem(doc, (char *)stream->GetData());

		xml_node * root = doc.first_node("Prefab");

		d_assert (root != NULL);

		xml_node * modelNode = root->first_node("Model");

		while (modelNode)
		{
			char SourceFile[128];
			Vec3 Position = Vec3::Zero;
			Quat Orientation = Quat::Identity;
			float Scale = 1;

			XmlHelper::GetAttribute(modelNode->first_attribute("File"), SourceFile);
			XmlHelper::GetAttribute(modelNode->first_attribute("pos"), Position.x, Position.y, Position.z);
			XmlHelper::GetAttribute(modelNode->first_attribute("ort"), Orientation.x, Orientation.y, Orientation.z, Orientation.w);
			XmlHelper::GetAttribute(modelNode->first_attribute("scale"), Scale);

			d_assert (strcmp(SourceFile, "") != 0);

			MActor * actor = MActorManager::Instance()->CreateActorU(SourceFile);
			SceneNode * node = World::Instance()->CreateSceneNode();

			node->Attach(actor);

			mModels.PushBack(actor);
			mNodes.PushBack(node);

			mRoot->AddChildNode(node);

			modelNode = modelNode->next_sibling();
		}
	}

	void MPrefab::Unload()
	{
		for (int i = 0; i < mModels.Size(); ++i)
		{
			MActorManager::Instance()->DestroyActor(mModels[i]);
		}

		for (int i = 0; i < mNodes.Size(); ++i)
		{
			World::Instance()->DestroySceneNode(mNodes[i]);
		}

		mModels.Clear();
		mNodes.Clear();
	}
}