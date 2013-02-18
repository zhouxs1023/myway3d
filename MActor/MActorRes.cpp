#include "MActorRes.h"
#include "MWDeviceCaps.h"
#include "MWRenderHelper.h"
#include "MWResourceManager.h"
#include "MActorManager.h"
#include <EMotionFX.h> 

namespace Myway {

	MActorRes::MActorRes(const TString128 & source)
		: mActor(NULL)
		, mAabb(Aabb::Invalid)
		, mSphere(Sphere::Zero)
	{
		SetSourceName(source);
	}

	MActorRes::~MActorRes()
	{
		_shutdown();
	}

	void MActorRes::DeleteSelf()
	{
		MActorManager::Instance()->_DeleteActorRes(this);
	}

	void MActorRes::Load()
	{
		if (CanLoad() && mLoadState == UNLOADED)
		{
			ResourceManager::Instance()->GetResourceLoader()->Load(this);

			OnResourceLoaded(this, NULL);

			{
				// 
				TString128 motionPath = File::GetFileDir(mSourceName);

				d_assert (motionPath != "");

				motionPath += "Motion\\";

				Archive::FileInfoList motionList; 

				ResourceManager::Instance()->GetFileInfosByFloder(motionList, motionPath);

				mMotionCount = motionList.Size();

				if (mMotionCount != 0)
				{
					mMotionSet = new MMotion[mMotionCount];

					Archive::FileInfoList::Iterator whr = motionList.Begin();
					Archive::FileInfoList::Iterator end = motionList.End();

					int index = 0;

					while (whr != end)
					{
						mMotionSet[index++]._Load(whr->name);

						++whr;
					}
				}
			}
		}

		mLoadState = LOADED;
	}

	void MActorRes::Reload()
	{
		if (CanLoad() && mLoadState == LOADED)
		{
			Unload();
			Load();
		}

		mLoadState = LOADED;
	}

	void MActorRes::Unload()
	{
		if (CanLoad() && mLoadState == LOADED)
		{
			_shutdown();

			OnResourceUnloaded(this, NULL);

			mLoadState = UNLOADED;
		}
	}

	void MActorRes::LoadImp(DataStreamPtr stream)
	{
		mActor = EMotionFX::IMPORTER.LoadActor((unsigned char*)stream->GetData(), stream->Size(), mSourceName.c_str());

		if (mActor)
		{
			_init();
		}
	}

	void _fillVertexSkined(void * data, EMotionFX::HwShaderBuffer & buffer)
	{
		int count = buffer.GetNumVertices();

		MActorRes::VertexSkined * vert = (MActorRes::VertexSkined *)data;

		for (int v = 0; v < count; ++v)
		{
			EMotionFX::HwShaderVertex &	vtx = buffer.GetVertex(v);
			EMotionFX::Mesh *			mesh = vtx.GetMesh();

			MCore::Vector3 *			pPosAr = mesh->GetOrgPositions();
			MCore::Vector3 *			pNmlAr = mesh->GetOrgNormals();
			MCore::Vector4 *			pTanAr = mesh->GetTangents();

			INT *						pIdxAr = mesh->GetIndices();
			MCore::Vector2 *			pUV0Ar = mesh->GetUVLayer(0) ? mesh->GetUVLayer(0)->GetUVs() : NULL;

			const int					vtxNr = vtx.GetVertexNumber();

			d_assert (vtx.GetNumInfluences() > 0 && vtx.GetInfluence(0).GetBoneNumber() <= 255 && "Bone count > 255!!!");

			vert->Position.x = pPosAr[vtxNr].x;
			vert->Position.y = pPosAr[vtxNr].y;
			vert->Position.z = pPosAr[vtxNr].z;

			vert->Normal.x = pNmlAr[vtxNr].x;
			vert->Normal.y = pNmlAr[vtxNr].y;
			vert->Normal.z = pNmlAr[vtxNr].z;

			if (pTanAr)
			{
				vert->Tangent.x = pTanAr[vtxNr].x;
				vert->Tangent.y = pTanAr[vtxNr].y;
				vert->Tangent.z = pTanAr[vtxNr].z;
				vert->Tangent.w = pTanAr[vtxNr].w;
			}
			else
			{
				vert->Tangent.x = 0;
				vert->Tangent.y = 0;
				vert->Tangent.z = 0;
				vert->Tangent.w = 0;
			}

			int infl = 0;

			while (infl < 4 && infl < vtx.GetNumInfluences())
			{
				int bi = vtx.GetInfluence(infl).GetBoneNumber();
				float bw = vtx.GetInfluence(infl).GetWeight();

				vert->BIndices[infl] = bi;
				vert->BWeights[infl] = bw;

				++infl;
			}

			while (infl < 4)
			{
				vert->BIndices[infl] = 0;
				vert->BWeights[infl] = 0;

				++infl;
			}

			if (pUV0Ar)
			{
				vert->Texcoord.x = pUV0Ar[vtxNr].x;
				vert->Texcoord.y = pUV0Ar[vtxNr].y;
			}
			else
			{
				vert->Texcoord.x = 0;
				vert->Texcoord.y = 0;
			}

			++vert;
		}
	}

	void _fillVertexNoSkined(void * data, EMotionFX::HwShaderBuffer & buffer)
	{
		int count = buffer.GetNumVertices();

		MActorRes::Vertex * vert = (MActorRes::Vertex *)data;

		for (int v = 0; v < count; ++v)
		{
			EMotionFX::HwShaderVertex &	vtx		= buffer.GetVertex(v);
			EMotionFX::Mesh *			mesh	= vtx.GetMesh();

			MCore::Vector3 *			pPosAr = mesh->GetOrgPositions();
			MCore::Vector3 *			pNmlAr = mesh->GetOrgNormals();
			MCore::Vector4 *			pTanAr = mesh->GetTangents();

			INT *						pIdxAr = mesh->GetIndices();
			MCore::Vector2 *			pUV0Ar = mesh->GetUVLayer(0) ? mesh->GetUVLayer(0)->GetUVs() : NULL;
			MCore::Vector2 *			pUV1Ar = mesh->GetUVLayer(1) ? mesh->GetUVLayer(1)->GetUVs() : NULL;

			const int vtxNr = vtx.GetVertexNumber();

			//d_assert(vtx.GetInfluence(0).GetBoneNumber() <= 255);

			vert->Position.x = pPosAr[vtxNr].x;
			vert->Position.y = pPosAr[vtxNr].y;
			vert->Position.z = pPosAr[vtxNr].z;

			vert->Normal.x = pNmlAr[vtxNr].x;
			vert->Normal.y = pNmlAr[vtxNr].y;
			vert->Normal.z = pNmlAr[vtxNr].z;

			if (pTanAr)
			{
				vert->Tangent.x = pTanAr[vtxNr].x;
				vert->Tangent.y = pTanAr[vtxNr].y;
				vert->Tangent.z = pTanAr[vtxNr].z;
				vert->Tangent.w = pTanAr[vtxNr].w;
			}
			else
			{
				vert->Tangent.x = 0;
				vert->Tangent.y = 0;
				vert->Tangent.z = 0;
				vert->Tangent.w = 0;
			}

			if (pUV0Ar)
			{
				vert->Texcoord.x = pUV0Ar[vtxNr].x;
				vert->Texcoord.y = pUV0Ar[vtxNr].y;
			}
			else
			{
				vert->Texcoord.x = 0;
				vert->Texcoord.y = 0;
			}

			++vert;
		}
	}

	void MActorRes::_initMat()
	{
		mMaterialCount = mActor->GetNumMaterials(0);

		mMaterials = new SMtl[mMaterialCount];

		TString128 sSourceDir = File::GetFileDir(mSourceName) + "texture\\";

		for (int i = 0; i < mMaterialCount; ++i)
		{
			EMotionFX::Material * mtl = mActor->GetMaterial(0, i).GetPointer();
			SMtl & mmtl = mMaterials[i];

			if (mtl->GetType() == EMotionFX::StandardMaterial::TYPE_ID)	// convert a standard material
			{
				EMotionFX::StandardMaterial * smtl = (EMotionFX::StandardMaterial *)mtl;

				mmtl.DoubleSide = smtl->GetDoubleSided();
				mmtl.Opacity = smtl->GetOpacity();

				int layerNr = smtl->FindLayer(EMotionFX::StandardMaterialLayer::LAYERTYPE_DIFFUSE);

				if (layerNr != -1)
				{
					TString128 textureFile = smtl->GetLayer(layerNr)->GetFilename().AsChar();

					if (textureFile == "")
					{
						mmtl.DiffuseMap = RenderHelper::Instance()->GetWhiteTexture();
						continue;
					}

					TString128 tFile = File::GetBaseName(textureFile);
					tFile = sSourceDir + tFile + ".dds";

					mmtl.DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(tFile, tFile);
				}

				layerNr = smtl->FindLayer(EMotionFX::StandardMaterialLayer::LAYERTYPE_BUMP);

				if (layerNr != -1)
				{
					TString128 textureFile = smtl->GetLayer(layerNr)->GetFilename().AsChar();

					if (textureFile == "")
					{
						mmtl.NormalMap = RenderHelper::Instance()->GetDefaultNormalTexture();
						continue;
					}

					TString128 tFile = File::GetBaseName(textureFile);
					tFile = sSourceDir + tFile + ".dds";

					mmtl.NormalMap = VideoBufferManager::Instance()->Load2DTexture(tFile, tFile);
				}

				layerNr = smtl->FindLayer(EMotionFX::StandardMaterialLayer::LAYERTYPE_SPECULAR);

				if (layerNr != -1)
				{
					TString128 textureFile = smtl->GetLayer(layerNr)->GetFilename().AsChar();

					if (textureFile == "")
					{
						mmtl.SpecularMap = RenderHelper::Instance()->GetBlackTexture();
						continue;
					}

					TString128 tFile = File::GetBaseName(textureFile);
					tFile = sSourceDir + tFile + ".dds";

					mmtl.SpecularMap = VideoBufferManager::Instance()->Load2DTexture(tFile, tFile);
				}
			}
		}
	}

	void MActorRes::_init()
	{
		_initMat();

		int iVertexCount = 0, iIndexCount = 0;
		mAabb = Aabb::Zero;

		//int numBoneForPrim = (DeviceCaps::Instance()->MaxVertexShaderConst() - K_NumNoSkinShaderConst) / 3;
		int numBoneForPrim = 50;

		MCore::Array<EMotionFX::HwShaderBuffer> hwShaderBufferArray;
		mActor->GenerateHardwareShaderBuffers(hwShaderBufferArray, 0, numBoneForPrim);

		mMeshCount = hwShaderBufferArray.Size();
		mMeshes = new SMesh[mMeshCount];

		for (int i = 0; i < mMeshCount; ++i)
		{
			EMotionFX::HwShaderBuffer & buffer = hwShaderBufferArray[i];

			int iVertexCount = buffer.GetNumVertices();
			bool skined = buffer.GetNumInfluences() > 0;
			int iStride = skined ? sizeof(VertexSkined) : sizeof(Vertex);

			VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iStride * iVertexCount);

			void * vert = vb->Lock(0, 0, LOCK_DISCARD);

			if (skined)
				_fillVertexSkined(vert, buffer);
			else
				_fillVertexNoSkined(vert, buffer);
			
			vb->Unlock();

			int iIndexCount = buffer.GetNumIndices();
			d_assert(iIndexCount <= 65535);

			IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof(short) * iIndexCount);

			unsigned short * idx = (unsigned short *)ib->Lock(0, 0, LOCK_DISCARD);
			for (int k = 0; k < iIndexCount; ++k)
			{
				*idx++ = (unsigned short)buffer.GetIndex(k);
			}

			ib->Unlock();

			SMesh * smesh = &mMeshes[i];

			for (int b = 0; b < buffer.GetNumBones(); ++b)
			{
				smesh->BoneArray.PushBack(buffer.GetBone(b));
			}

			// update aabb
			for (int v = 0; v < iVertexCount; ++v)
			{
				EMotionFX::HwShaderVertex &	vtx		= buffer.GetVertex(v);
				EMotionFX::Mesh *			mesh	= vtx.GetMesh();
				MCore::Vector3 *			pPosAr	= mesh->GetPositions();
				const int					vtxNr	= vtx.GetVertexNumber();

				mAabb.Merge(Vec3(pPosAr[vtxNr].x, pPosAr[vtxNr].y, pPosAr[vtxNr].z));
			}

			const int numPrimitives = buffer.GetNumPrimitives();

			smesh->PrimCount = numPrimitives;
			smesh->Primitives = new SPrim[numPrimitives];

			for (int p=0; p<numPrimitives; ++p)
			{
				const EMotionFX::HwShaderPrimitive & bufferPrim	= buffer.GetPrimitive(p);
				SPrim & prim = smesh->Primitives[p];

				int StartVertex = bufferPrim.GetStartVertex();
				int NumVertices = bufferPrim.GetNumVertices();
				int StartIndex	= bufferPrim.GetStartIndex();
				int NumTriangles = bufferPrim.GetNumTriangles();

				prim.NodeId	= bufferPrim.GetNode()->GetID();
				prim.MaterialId	= bufferPrim.GetMaterialNr();
				
				prim.Rop.vxStream.SetDeclaration(MActorManager::Instance()->GetVertexDeclaration(skined));
				prim.Rop.vxStream.Bind(0, vb, iStride);
				prim.Rop.vxStream.SetStart(StartVertex);
				prim.Rop.vxStream.SetCount(NumVertices);

				prim.Rop.ixStream.Bind(ib, 0);
				prim.Rop.ixStream.SetStart(StartIndex);
				prim.Rop.ixStream.SetCount(iIndexCount);

				prim.Rop.iPrimCount = NumTriangles;
				prim.Rop.ePrimType = PRIM_TRIANGLELIST;

				SMtl * mtl = GetMaterial(prim.MaterialId);

				if (mtl->DoubleSide)
					prim.Rop.rState.cullMode = CULL_NONE;
			}
		}

		mSphere.center = mAabb.GetCenter();
		mSphere.radius = mSphere.center.Distance(mAabb.minimum);
	}

	void MActorRes::_shutdown()
	{
		safe_delete_array (mMaterials);
		safe_delete_array (mMeshes);
		safe_delete_array (mMotionSet);

		mMaterialCount = 0;
		mMeshCount = 0;
		mMotionCount = 0;

		safe_delete (mActor);
	}


	MMotion * MActorRes::GetMotion(const TString128 & motionName)
	{
		TString128 lname = motionName.LowerR();

		for (int i = 0; i < mMotionCount; ++i)
		{
			if (mMotionSet[i].GetName() == lname)
				return &mMotionSet[i];
		}

		return NULL;
	}
}
