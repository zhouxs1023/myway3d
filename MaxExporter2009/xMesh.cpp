#include "stdafx.h"

#include "MWMeshLoader.h"
#include "xMesh.h"
#include "xExportConfig.h"

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
			{
				return i;
			}
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






	xMesh::xMesh()
	{
		mVertexElems = 0;
	}

	xMesh::~xMesh()
	{
	}

	void xMesh::Build(IGameObject * obj, IGameMaterial * mtl)
	{
		if (obj->GetIGameType() != IGameMesh::IGAME_MESH)
			return ;

		mVertexElems |= MeshLoader_v1::VE_POSITION;

		// extract mesh
		IGameMesh* mesh = (IGameMesh*) obj;
		int vertCount = mesh->GetNumberOfVerts();
		int faceCount = mesh->GetNumberOfFaces();

		Tab<int> texMaps = mesh->GetActiveMapChannelNum();

		for (int i=0; i<faceCount; i++)
		{
			FaceEx* face = mesh->GetFace(i);

			xFace xface;

			for (int vi=0; vi<3; vi++) 
			{
				Point3 p = mesh->GetVertex(face->vert[vi]);

				xVertex v;
				
				v.SetPosition(Vec3(p.x, p.y, p.z));

				if (xExportConfig::Instance()->IsExportColor())
				{
					Point3 c = mesh->GetColorVertex(face->vert[vi]);
					float a = mesh->GetAlphaVertex(face->vert[vi]);

					v.SetColor(Color4(c.x, c.y, c.z, a));

					mVertexElems |= MeshLoader_v1::VE_COLOR;
				}

				if (xExportConfig::Instance()->IsExportNormal())
				{
					Point3 n = mesh->GetNormal(face, vi);
					v.SetNormal(Vec3(n.x, n.y, n.z));

					mVertexElems |= MeshLoader_v1::VE_NORMAL;
				}

				if (xExportConfig::Instance()->IsExportTexcoord() && texMaps.Count())
				{
					Point3 tv;
					DWORD indices[3];

					if (mesh->GetMapFaceIndex(texMaps[0], i, indices))
						tv = mesh->GetMapVertex(texMaps[0], indices[vi]);
					else
						tv = mesh->GetMapVertex(texMaps[0], face->vert[vi]);

					v.SetTexcoord(Vec2(tv.x, tv.y));

					mVertexElems |= MeshLoader_v1::VE_TEXCOORD;
				}

				if (xExportConfig::Instance()->IsExportLightmapUV() && texMaps.Count() > 1)
				{
					Point3 tv;
					DWORD indices[3];

					if (mesh->GetMapFaceIndex(texMaps[1], i, indices))
						tv = mesh->GetMapVertex(texMaps[1], indices[vi]);
					else
						tv = mesh->GetMapVertex(texMaps[1], face->vert[vi]);

					v.SetLightmapUV(Vec2(tv.x, tv.y));

					mVertexElems |= MeshLoader_v1::VE_LIGHTMAPUV;
				}

				xface.p[vi] = mVertexList.Add(v);
			}

			mFaces.PushBack(xface);
		}
	}


	void xMesh::_buildMat(IGameMaterial * mtl)
	{
		if (!mtl)
			return ;
		
		int count = mtl->GetSubMaterialCount();

		d_assert (count == 0);

		Point4 val4;
		Point3 val3;
		PropType pt;
		IGameProperty* p = mtl->GetAmbientData();

		if (p)
		{
			pt = p->GetType();

			if (pt == IGAME_POINT3_PROP) 
			{
				p->GetPropertyValue(val3);
				mMaterial.SetAmbient(Color4(val3.x, val3.y, val3.z, 1));
			}

			if (pt == IGAME_POINT4_PROP)
			{
				p->GetPropertyValue(val4);
				mMaterial.SetAmbient(Color4(val4.x, val4.y, val4.z, val4.w));
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

			if (pt == IGAME_POINT4_PROP)
			{
				p->GetPropertyValue(val4);
				mMaterial.SetDiffuse(Color4(val4.x, val4.y, val4.z, val4.w));
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

			if (pt == IGAME_POINT4_PROP)
			{
				p->GetPropertyValue(val4);
				mMaterial.SetSpecular(Color4(val4.x, val4.y, val4.z, val4.w));
			}
		}

		p = mtl->GetGlossinessData();
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

			if (pt == IGAME_POINT4_PROP)
			{
				p->GetPropertyValue(val4);
				mMaterial.SetEmissive(Color4(val4.x, val4.y, val4.z, val4.w));
			}
		}

		int numTextures = mtl->GetNumberOfTextureMaps();

		for ( int index = 0; index < numTextures; ++index )
		{
			IGameTextureMap * pMap = mtl->GetIGameTextureMap(index);

			if (!pMap)
				continue;

			const int type = pMap->GetStdMapSlot();
			std::string bmap(pMap->GetBitmapFileName());
			bmap = bmap.substr(bmap.find_last_of('\\') + 1);

			if (type == ID_DI)
				mMaterial.SetDiffuseMap(bmap.c_str());
			else if (type == ID_BU)
				mMaterial.SetNormalMap(bmap.c_str());
			else if (type == ID_SP)
				mMaterial.SetSpecularMap(bmap.c_str());
			else if (type == ID_SI)
				mMaterial.SetEmissiveMap(bmap.c_str());
		}
	}


}