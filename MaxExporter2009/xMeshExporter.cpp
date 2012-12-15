#include "stdafx.h"

#include "xMeshExporter.h"
#include "xExportConfig.h"
#include "xMesh.h"
#include "xSkeleton.h"

namespace MaxPlugin {

IMP_SLN(xMeshExporter);

xMeshExporter::xMeshExporter(ExpInterface * ei, Interface * i)
	: mExpInterface(ei)
	, mInterface(i)
	, mGameScene(NULL)
{
	INIT_SLN;
}

xMeshExporter::~xMeshExporter()
{
	SHUT_SLN;
}

void xMeshExporter::Export()
{
	try 
	{
		if (mGameScene)
		{
			mGameScene->ReleaseIGame();
			mGameScene = 0;
		}

		mGameScene = GetIGameInterface();
		IGameConversionManager* cm = GetConversionManager();
		cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
		mGameScene->InitialiseIGame(xExportConfig::Instance()->IsExportSelected());
		mGameScene->SetStaticFrame(0);
		int nodeCount = mGameScene->GetTopLevelNodeCount();

		if (nodeCount == 0)
		{
			MessageBox(GetActiveWindow(), "No nodes available!", "Error", MB_OK);
			mGameScene->ReleaseIGame();
			return ;
		}

		// extract node
		xSkeleton skel;

		for (int i = 0; i < nodeCount; ++i)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(i);

			if (node->IsNodeHidden())
				continue ;
			
			skel.Extrat(node);
		}

		// extract mesh
		xMesh mesh;

		for (int i = 0; i < nodeCount; ++i)
		{
			IGameNode* node = mGameScene->GetTopLevelNode(i);

			if (node->IsNodeHidden())
				continue ;

			mesh.Extract(node);
		}

		if (mesh.GetSubMeshCount() == 0)
		{
			MessageBox(GetActiveWindow(), "No Objects!", "Error", MB_OK);
			return ;
		}

		File file;

		file.Open(xExportConfig::Instance()->GetExportFilename().c_str());

		d_assert (file.IsOpen());

		file.Write(MODEL_FILE_MAGIC, MODEL_FILE_MAGIC_LEN);

		int version = MeshLoader_v1::K_File_Verion;
		file.Write(&version, sizeof (int));

		for (int i = 0; i < mesh.GetSubMeshCount(); ++i)
		{
			WriteSubMesh(mesh.GetSubMesh(i), file);
		}

		WriteSkel(&skel, file);

		mGameScene->ReleaseIGame();
	}
	catch (...) 
	{
		MessageBox(GetActiveWindow(), "Error!", "Error", MB_ICONEXCLAMATION);

		if (mGameScene)
			mGameScene->ReleaseIGame();
	}
}

void xMeshExporter::WriteSubMesh(xSubMesh * mesh, File & file)
{
	int id = MC_SUBMESH;
	file.Write(&id, sizeof(int));

	const xVertexList & vertList = mesh->mVertexList;
	const Array<xFace> & faceList = mesh->mFaces;
	const xMaterial & material = mesh->mMaterial;

	int iVerison = MeshLoader_v1::K_SubMesh_Version;
	int iVertexCount = vertList.Size();
	int iIndexCount = faceList.Size() * 3;
	int iVertexElems = mesh->mVertexElems;

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
		const xBlendIndex & bi = v.GetBlendIndex(); 
		const xBlendWeight & bw = v.GetBlendWeight();

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

		if (iVertexElems & MeshLoader_v1::VE_BLENDINDICES)
			file.Write(&bi, sizeof(xBlendIndex));

		if (iVertexElems & MeshLoader_v1::VE_BLENDWEIGHTS)
		{
			file.Write(&bw, sizeof(xBlendWeight));
		}
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
	int mtlVersion = MeshLoader_v1::K_Material_Version;

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

void xMeshExporter::WriteSkel(xSkeleton * skel, File & file)
{
	if (skel->GetBoneCount() == 0)
		return ;

	int id = MC_SKELETON;
	file.Write(&id, sizeof(int));

	file.Write(&MeshLoader_v1::K_Skeleton_Version, sizeof(int));

	int boneCount = skel->GetBoneCount();

	file.Write(&boneCount, sizeof(int));

	for (int i = 0; i < boneCount; ++i)
	{
		xBone * bone = skel->GetBone(i);

		file.Write(bone->name.c_str(), 128);
		file.Write(&bone->position, sizeof(Point3));
		file.Write(&bone->orientation, sizeof(::Quat));
		file.Write(&bone->scale, sizeof(float));
	}

	for (int i = 0; i < boneCount; ++i)
	{
		xBone * bone = skel->GetBone(i);

		short parent = bone->parent;
		short child = (short)i;

		file.Write(&parent, sizeof(short));
		file.Write(&child, sizeof(short));
	}
}


}
