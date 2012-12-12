#include "stdafx.h"

#include "xMeshExporter.h"
#include "xExportConfig.h"
#include "xMesh.h"

namespace MaxPlugin {

xMeshExporter::xMeshExporter(ExpInterface * ei, Interface * i)
	: mExpInterface(ei)
	, mInterface(i)
	, mGameScene(NULL)
	, mBoneIndex(0)
{
}

xMeshExporter::~xMeshExporter()
{
}

int xMeshExporter::callback(INode *node)
{
	TimeValue start = mInterface->GetAnimRange().Start();
	Object *obj = node->EvalWorldState(start).obj;
	Class_ID cid = obj->ClassID();

	// nodes that have Biped controllers are bones -- ignore the ones that are dummies
	if (cid == Class_ID(DUMMY_CLASS_ID, 0))
		return TREE_CONTINUE;

	Control *c = node->GetTMController();
	if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
		(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
		(c->ClassID() == FOOTPRINT_CLASS_ID)) {

			if (node->GetParentNode() != NULL)
			{
				mBoneIndexMap.Insert(node->GetName(), mBoneIndex++);
			}

			return TREE_CONTINUE;
	}

	if (!obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		return TREE_CONTINUE;

	if (xExportConfig::Instance()->IsExportSelected())
	{
		if (node->Selected())
		{
			mNodeTab.Append(1, &node);
		}
	}
	else
	{
		mNodeTab.Append(1, &node);
	}

	return TREE_CONTINUE;
}

void xMeshExporter::Build()
{
	try 
	{
		if (mGameScene)
		{
			mGameScene->ReleaseIGame();
			mGameScene = 0;
		}

		mExpInterface->theScene->EnumTree(this);

		mGameScene = GetIGameInterface();
		IGameConversionManager* cm = GetConversionManager();
		cm->SetCoordSystem(IGameConversionManager::IGAME_OGL);
		mGameScene->InitialiseIGame(mNodeTab, false);
		mGameScene->SetStaticFrame(0);
		int nodeCount = mGameScene->GetTopLevelNodeCount();

		if (nodeCount == 0) {
			MessageBox(GetActiveWindow(), "No nodes available!", "Error", MB_ICONINFORMATION);
			mGameScene->ReleaseIGame();
			return ;
		}

		File file;

		file.Open(xExportConfig::Instance()->GetExportFilename().c_str());

		d_assert (file.IsOpen());

		file.Write(MODEL_FILE_MAGIC, MODEL_FILE_MAGIC_LEN);

		int version = MODEL_FILE_VERSION_1;
		file.Write(&version, sizeof (int));

		int nodeIdx = 0;
		while (nodeIdx < nodeCount)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(nodeIdx);
			IGameObject* obj = node->GetIGameObject();

			obj->InitializeData();

			if (obj->GetIGameType() == IGameMesh::IGAME_MESH)
			{
				IGameMaterial* mtl = node->GetNodeMaterial();

				xMesh mesh;

				mesh.Build(obj, mtl);

				WriteSubMesh(&mesh, file);
			}
			
			node->ReleaseIGameObject();
			nodeIdx++;
		}

		mGameScene->ReleaseIGame();
	}
	catch (...) 
	{
		MessageBox(GetActiveWindow(), "Error!", "Error", MB_ICONEXCLAMATION);

		if (mGameScene)
			mGameScene->ReleaseIGame();
	}
}

void xMeshExporter::WriteSubMesh(xMesh * mesh, File & file)
{
	int id = MC_SUBMESH;
	file.Write(&id, sizeof(int));

	const xVertexList & vertList = mesh->GetVertexList();
	const Array<xFace> & faceList = mesh->GetFaces();
	const xMaterial & material = mesh->GetMaterial();

	int iVerison = 0;
	int iVertexCount = vertList.Size();
	int iIndexCount = faceList.Size() * 3;
	int iVertexElems = mesh->GetVertexElems();

	file.Write(&iVerison, sizeof(int));

	// 1. Write Geometry
	file.Write(&iVertexCount, sizeof(int));
	file.Write(&iIndexCount, sizeof(int));
	file.Write(&iVertexElems, sizeof(int));

	for (int i = 0; i < iVertexCount; ++i)
	{
		const xVertex & v = vertList.GetVertex(i);
		const Vec3 & position = v.GetPosition();
		const Vec3 & normal = v.GetNormal();
		const Color4 & color = v.GetColor();
		const Vec3 & tangent = v.GetTangent();
		const Vec2 & texcoord = v.GetTexcoord();
		const Vec2 & lightmapuv = v.GetLightmapUV();

		if (iVertexElems & MeshLoader_v1::VE_POSITION)
			file.Write(&position, sizeof(Vec3));

		if (iVertexElems & MeshLoader_v1::VE_NORMAL)
			file.Write(&normal, sizeof(Vec3));

		if (iVertexElems & MeshLoader_v1::VE_COLOR)
			file.Write(&color, sizeof(Color4));

		if (iVertexElems & MeshLoader_v1::VE_TANGENT)
			file.Write(&tangent, sizeof(Vec3));

		if (iVertexElems & MeshLoader_v1::VE_TEXCOORD)
			file.Write(&texcoord, sizeof(Vec2));

		if (iVertexElems & MeshLoader_v1::VE_LIGHTMAPUV)
			file.Write(&lightmapuv, sizeof(Vec2));
	}

	for (int i = 0; i < faceList.Size(); ++i)
	{
		xFace face = faceList[i];

		if (iIndexCount > 65536)
		{
			unsigned int p0 = face.p[0];
			unsigned int p1 = face.p[1];
			unsigned int p2 = face.p[2];

			file.Write(&p0, sizeof(unsigned int));
			file.Write(&p1, sizeof(unsigned int));
			file.Write(&p2, sizeof(unsigned int));
		}
		else
		{
			unsigned short p0 = unsigned short (face.p[0]);
			unsigned short p1 = unsigned short (face.p[1]);
			unsigned short p2 = unsigned short (face.p[2]);

			file.Write(&p0, sizeof(unsigned short));
			file.Write(&p1, sizeof(unsigned short));
			file.Write(&p2, sizeof(unsigned short));
		}

	}

	// 2. Write Material
	int mtlVersion = 0;

	int doubleSide = material.GetDoubleSide();
	int blendMode = material.GetBlendMode();
	
	const Color4 & emissive = material.GetEmissive();
	const Color4 & ambient = material.GetAmbient();
	const Color4 & diffuse = material.GetDiffuse();
	const Color4 & specular = material.GetSpecular();
	float specularPower = material.GetSpecularPower();

	const TString128 & emissiveMap = material.GetEmissiveMap();
	const TString128 & diffuseMap = material.GetDiffuseMap();
	const TString128 & normalMap = material.GetNormalMap();
	const TString128 & specularMap = material.GetSpecularMap();

	file.Write(&mtlVersion, sizeof(int));

	file.Write(&doubleSide, sizeof(int));
	file.Write(&blendMode, sizeof(int));

	file.Write(&emissive, sizeof(Color4));
	file.Write(&ambient, sizeof(Color4));
	file.Write(&diffuse, sizeof(Color4));
	file.Write(&specular, sizeof(Color4));
	file.Write(&specularPower, sizeof(float));

	file.Write(emissiveMap.c_str(), 128);
	file.Write(diffuseMap.c_str(), 128);
	file.Write(normalMap.c_str(), 128);
	file.Write(specularMap.c_str(), 128);
}

}
