#include "MVegetation.h"
#include "MWResourceManager.h"
#include "MWMeshManager.h"

namespace Myway {









	MVegetationLoader::MVegetationLoader()
	{
	}

	MVegetationLoader::~MVegetationLoader()
	{
	}

	void MVegetationLoader::Load(Array<MVegetation*> * Vegs, const TString128 & source)
	{
		mSource = source;

		DataStreamPtr data = ResourceManager::Instance()->OpenResource(source.c_str());

		xml_doc doc;

		doc.parse<0>((char *)data->GetData());

		xml_node * root = doc.first_node("Vegetation");

		xml_node * ndType = root->first_node("Type");
		xml_node * ndMesh = root->first_node("Mesh");
		xml_node * ndDiffuseMap = root->first_node("DiffuseMap");

		xml_node * ndDiffuse = root->first_node("Diffuse");
		xml_node * ndSpecular = root->first_node("Specular");

		_loadType(ndType);
		_loadMesh(ndMesh);
		_loadDiffuseMap(ndDiffuseMap);
	}

	void MVegetationLoader::_loadType(xml_node * nd)
	{
		TString128 sType = nd->first_attribute("Value")->value();

		MVegetation::GeomType type = MVegetation::GT_BillBoard;

		if (sType == "Billboard")
			type = MVegetation::GT_BillBoard;
		else if (sType == "X2")
			type = MVegetation::GT_X2;
		else if (sType == "Mesh")
			type = MVegetation::GT_Mesh;
		else
			LOG_PRINT_FORMAT("Unknown Vegetation Type '%s' In Source '%s'.\n", sType.c_str(), mSource.c_str());

		mCurVeg->Type = type;
	}

	void MVegetationLoader::_loadMesh(xml_node * nd)
	{
		if (nd == NULL)
			return ;

		TString128 sMeshFile = nd->first_attribute("Value")->value();

		if (mCurVeg->Type == MVegetation::GT_Mesh)
		{
			mCurVeg->pMesh = MeshManager::Instance()->Load(sMeshFile, sMeshFile);
		}
	}

	void MVegetationLoader::_loadDiffuseMap(xml_node * nd)
	{
		if (nd == NULL)
			return ;

		TString128 sMap = nd->first_attribute("DiffuseMap")->value();

		mCurVeg->DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(sMap, sMap);
	}
}