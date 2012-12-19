#include "stdafx.h"

#include "MWMeshLoader.h"
#include "xMesh.h"
#include "xExportConfig.h"
#include "xTextureExporter.h"
#include "xSkeleton.h"
#include "xUtility.h"

namespace MaxPlugin {

	xVertex::xVertex()
	{
		mPosition = Vec3::Zero;
		mNormal = Vec3::Zero;
		mTangent = Vec3::Zero;
		mColor = Color4::Black;
		mTexcoord = Vec2::Zero;
		mLightmapUV = Vec2::Zero;
	}

	xVertex::~xVertex()
	{
	}







	xVertexList::xVertexList()
	{
	}

	xVertexList::~xVertexList()
	{
	}

	int xVertexList::Add(const xVertex & v)
	{
		int i = 0;

		for (i = 0; i < Size(); ++i)
		{
			if (mVerts[i] == v)
				return i;
		}
	
		mVerts.PushBack(v);

		return i;
	}

	int xVertexList::Size() const
	{
		return mVerts.Size();
	}

	const xVertex & xVertexList::GetVertex(int index) const
	{
		d_assert (index < Size());

		return mVerts[index];
	}

	xVertex & xVertexList::GetVertex(int index)
	{
		d_assert (index < Size());

		return mVerts[index];
	}





	xMesh::xMesh()
	{
	}

	xMesh::~xMesh()
	{
		for (int i = 0; i < mSubMeshes.Size(); ++i)
		{
			delete mSubMeshes[i];
		}
	}

	void xMesh::Extract(IGameNode * node)
	{
		if (node->IsNodeHidden())
			return  ;

		mMaxMesh.Clear();

		IGameObject* obj = node->GetIGameObject();

		// export option
		bool expColor = xExportConfig::Instance()->IsExportColor();
		bool expNormal = xExportConfig::Instance()->IsExportNormal();
		bool expTexcoord = xExportConfig::Instance()->IsExportTexcoord();
		bool expLightmapUV = xExportConfig::Instance()->IsExportLightmapUV();

		obj->InitializeData();

		if (obj->GetIGameType() == IGameMesh::IGAME_MESH)
		{
			IGameMesh* mesh = (IGameMesh*) obj;
			Tab<int> texMaps = mesh->GetActiveMapChannelNum();

			mVertexElems = 0;
			mVertexElems |= MeshLoader_v1::VE_POSITION;

			// position
			for (int i = 0; i < mesh->GetNumberOfVerts(); ++i)
				mMaxMesh.P.PushBack(xUtility::ToVec3(mesh->GetVertex(i)));

			// vertex color
			for (int i = 0; expColor && i < mesh->GetNumberOfColorVerts(); ++i)
			{
				Point3 c = mesh->GetColorVertex(i);
				float a = mesh->GetAlphaVertex(i);

				mMaxMesh.C.PushBack(Vec4(c.x, c.y, c.z, a));
				
				mVertexElems |= MeshLoader_v1::VE_COLOR;
			}

			// normal
			for (int i = 0; expNormal && i < mesh->GetNumberOfNormals(); ++i)
			{
				mMaxMesh.N.PushBack(xUtility::ToVec3(mesh->GetNormal(i)));

				mVertexElems |= MeshLoader_v1::VE_NORMAL;
			}

			// u v
			for (int i = 0;  expTexcoord && texMaps.Count() && i < mesh->GetNumberOfMapVerts(texMaps[0]); ++i)
			{
				Point3 tv = mesh->GetMapVertex(texMaps[0], i);

				mMaxMesh.UV.PushBack(Vec2(tv.x, tv.y));

				mVertexElems |= MeshLoader_v1::VE_TEXCOORD;
			}

			// light map u v
			for (int i = 0;  expTexcoord && texMaps.Count() > 1 && i < mesh->GetNumberOfMapVerts(texMaps[1]); ++i)
			{
				Point3 tv = mesh->GetMapVertex(texMaps[1], i);

				mMaxMesh.LUV.PushBack(Vec2(tv.x, tv.y));

				mVertexElems |= MeshLoader_v1::VE_TEXCOORD;
			}

			_extractSkinInfo(obj);

			_extractSubMesh(mesh);
		}

		node->ReleaseIGameObject();
	}

	void xMesh::_extractSkinInfo(IGameObject * obj)
	{
		IGameSkin * Skin = obj->GetIGameSkin();

		if (!Skin)
			return ;

		d_assert (Skin->GetNumOfSkinnedVerts() == mMaxMesh.P.Size());

		//Export skinned verts
		for (int i = 0; i < Skin->GetNumOfSkinnedVerts(); i++)
		{
			int type = Skin->GetVertexType(i);

			xBlendIndex bi;
			xBlendWeight bw;

			if (type==IGameSkin::IGAME_RIGID_BLENDED)
			{
				for (int b = 0; b < Skin->GetNumberOfBones(i) && b < 4; b++)
				{
					INode * bone = Skin->GetBone(i, b);
					float weight = Skin->GetWeight(i, b);

					if (bone)
					{
						const char * bname = bone->GetName();
						int boneId = xSkeleton::Instance()->_getBoneId(bname);

						bi.i[b] = boneId;
						bw.w[b] = weight;
					}
				}
			}
			else if (type == IGameSkin::IGAME_RIGID)
			{
				INode * bone = Skin->GetBone(i, 0);

				if (bone)
				{
					const char * bname = bone->GetName();
					int boneId = xSkeleton::Instance()->_getBoneId(bname);

					bi.i[0] = boneId;
					bw.w[0] = 1;
				}
			}

			bw.normalize();

			mMaxMesh.BI.PushBack(bi);
			mMaxMesh.BW.PushBack(bw);
		}

		mVertexElems |= MeshLoader_v1::VE_BLENDWEIGHTS;
		mVertexElems |= MeshLoader_v1::VE_BLENDINDICES;
	}

	void xMesh::_addFace(xSubMesh * subMesh, FaceEx * face)
	{
		xFace xface;

		for (int j = 0; j < 3; ++j)
		{
			xVertex v;

			// position
			Point3 pos = mMaxMesh.P[face->vert[j]];
			v.SetPosition(Vec3(pos.x, pos.y, pos.z));

			// normal
			if (mVertexElems & MeshLoader_v1::VE_NORMAL)
			{
				Point3 nrm = mMaxMesh.N[face->norm[j]];

				v.SetNormal(Vec3(nrm.x, nrm.y, nrm.z));
			}

			// vertex color
			if (mVertexElems & MeshLoader_v1::VE_COLOR)
			{
				Point4 clr = mMaxMesh.C[face->color[j]];

				v.SetColor(Color4(clr.x, clr.y, clr.z, clr.w));
			}

			// u v
			if (mVertexElems & MeshLoader_v1::VE_TEXCOORD)
			{
				Point2 uv = mMaxMesh.UV[face->texCoord[j]];

				v.SetTexcoord(Vec2(uv.x, uv.y));
			}

			// light map u v
			if (mVertexElems & MeshLoader_v1::VE_LIGHTMAPUV)
			{
				Point2 uv = mMaxMesh.LUV[face->texCoord[j]];
				v.SetLightmapUV(Vec2(uv.x, uv.y));
			}

			// blend index
			if (mVertexElems & MeshLoader_v1::VE_BLENDINDICES)
			{
				xBlendIndex bi = mMaxMesh.BI[face->vert[j]];
				v.SetBlendIndex(bi);
			}

			// blend weight
			if (mVertexElems & MeshLoader_v1::VE_BLENDWEIGHTS)
			{
				xBlendWeight bw = mMaxMesh.BW[face->vert[j]];
				v.SetBlendWeight(bw);
			}

			xface.p[j] = subMesh->mVertexList.Add(v);
		}

		subMesh->mFaces.PushBack(xface);
	}

	void xMesh::_extractSubMesh(IGameMesh * mesh)
	{
		Tab<int> matGrps = mesh->GetActiveMatIDs();

		for ( int x=0; x < matGrps.Count(); x++ )
		{
			xSubMesh * subMesh = new xSubMesh();

			subMesh->mVertexElems = mVertexElems;

			Tab <FaceEx *> matFaces = mesh->GetFacesFromMatID(matGrps[x]);
			if (matFaces.Count()>0)
			{
				for(int f=0;f<matFaces.Count();f++)
				{
					FaceEx *fe = matFaces[f];
					_addFace(subMesh, mesh->GetFace(f));
				}
				std::string strDiffuse;
				int nColorID = 0;
				IGameMaterial* mtl = mesh->GetMaterialFromFace( matFaces[0] );
				if (mtl)
					_buildMat(subMesh, mtl);
			}

			mSubMeshes.PushBack(subMesh);
		}
	}

	void xMesh::_buildMat(xSubMesh * subMesh, IGameMaterial * mtl)
	{
		/*Point4 val4;
		Point3 val3;
		PropType pt;
		IGameProperty* p = NULL;


		IGameProperty* p = mtl->GetAmbientData();
		if (p)
		{
		pt = p->GetType();

		if (pt == IGAME_POINT3_PROP) 
		{
		p->GetPropertyValue(val3);
		mMaterial.SetAmbient(Color4(val3.x, val3.y, val3.z, 1));
		}
		}

		p = mtl->GetDiffuseData();
		if (p)
		{
		pt = p->GetType();

		if (pt == IGAME_POINT3_PROP)
		{
		p->GetPropertyValue(val3);
		mMaterial.SetDiffuse(Color4(val3.x, val3.y, val3.z, 1));
		}
		}

		p = mtl->GetSpecularData();
		if (p)
		{
		pt = p->GetType();

		if (pt == IGAME_POINT3_PROP)
		{
		p->GetPropertyValue(val3);
		mMaterial.SetSpecular(Color4(val3.x, val3.y, val3.z, 1));
		}
		}

		p = mtl->GetSpecularLevelData();
		if (p)
		{
		float specularPower;
		p->GetPropertyValue(specularPower);
		mMaterial.SetSpecularPower(specularPower);
		}

		p = mtl->GetEmissiveData();
		if (p)
		{
		pt = p->GetType();

		if (pt == IGAME_POINT3_PROP)
		{
		p->GetPropertyValue(val3);
		mMaterial.SetEmissive(Color4(val3.x, val3.y, val3.z, 1));
		}
		}*/
		int numTextures = mtl->GetNumberOfTextureMaps();

		for ( int index = 0; index < numTextures; ++index )
		{
			IGameTextureMap * pMap = mtl->GetIGameTextureMap(index);

			if (!pMap)
				continue;

			const int type = pMap->GetStdMapSlot();
			const char * tex = pMap->GetBitmapFileName();
			std::string bmap = tex;
			bmap = bmap.substr(bmap.find_last_of('\\') + 1);

			if (type == ID_DI)
				subMesh->mMaterial.SetDiffuseMap(bmap.c_str());
			else if (type == ID_BU)
				subMesh->mMaterial.SetNormalMap(bmap.c_str());
			else if (type == ID_SP)
				subMesh->mMaterial.SetSpecularMap(bmap.c_str());
			else if (type == ID_SI)
				subMesh->mMaterial.SetEmissiveMap(bmap.c_str());

			xTextureExporter::Instance()->Push(pMap->GetBitmapFileName());
		}

	}

	void xMesh::_buildSkinInfo(IGameObject * obj)
	{
		
	}
}