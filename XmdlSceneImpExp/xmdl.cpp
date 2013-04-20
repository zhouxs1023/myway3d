#include "stdafx.h"

#include "xmdl.h"

namespace xmdl {

	// 场景模型阴影属性(2 Bits)
#define GEOSET_NOCASTSHADOW	(1L<<0)	// 是否投射阴影
#define GEOSET_NORCVSHADOW	(1L<<1)	// 是否接受阴影

	// 场景模型光照属性(2 Bits)
#define GEOSET_RENDERMASK 	(3L<<2)	// 光照方式
#define GEOSET_RENDERSHIFT	2		//

	// 其他属性(1 bits)
#define GEOSET_BRIGE		(1L<<4)	// 是否垫高

	// Billboard 属性(2 bits)
#define GEOSET_BILLBOARD_MASK	(3L<<5)
#define GEOSET_BILLBOARD_SHIFT	5

	// 碰撞
#define GEOSET_COLLISION_MASK	(7L<<7)
#define GEOSET_COLLISION_SHIFT	7

#define GEOSET_COLLISIONPOLYGON		(1L<<15)	// todo:去掉.输出专门的Chunhk

	// 用户定义时间可见属性
#define GEOSET_TIMEHIDE_MASK		(0xff<<16)
#define GEOSET_TIMEHIDE_SHIFT	16



	t_xmdl::t_xmdl()
	{
		mStrTableSize = 0;
		mStrTable = NULL;

		mCount = 0;
		mModel = NULL;

		mTextureCount = 0;
		mTextures = NULL;

		mNodeCount = 0;
		mNodes = NULL;

		mSkinAnimCount = 0;
		mSkinAnims = NULL;
	}

	t_xmdl::~t_xmdl()
	{
		safe_delete_array (mStrTable);
		safe_delete_array (mModel);
		safe_delete_array (mTextures);

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			delete mMeshes[i];
		}

		safe_delete_array (mNodes);
		mNodeCount = 0;

		safe_delete_array (mSkinAnims);
		mSkinAnimCount = 0;
	}

	void t_xmdl::load(const char * filename)
	{
		mFilename = TString128(filename);

		DataStreamPtr file = ResourceManager::Instance()->OpenResource(mFilename.c_str(), true);

		if (file == NULL)
			return ;

		_loadHead(file);

		t_chunk ck;

		while ( file->Read(&ck, sizeof(t_chunk)) )
		{
			if (ck.dwFlag == MODEL_FLAG)
			{
				mCount = ck.dwChunkSize / sizeof(t_model);
				mModel = new t_model[mCount];

				d_assert (mCount * sizeof(t_model) == ck.dwChunkSize);

				file->Read(mModel, sizeof(t_model) * mCount);
			}

			else if (ck.dwFlag == TEXS_FLAG)
			{
				mTextureCount = ck.dwChunkSize / sizeof(t_texture);
				mTextures = new t_texture[mTextureCount];

				d_assert (mTextureCount * sizeof(t_texture) == ck.dwChunkSize);

				file->Read(mTextures, sizeof(t_texture) * mTextureCount);
			}

			else if (ck.dwFlag == FXS_FLAG)
			{
				mFxCount = ck.dwChunkSize / sizeof(t_fx);
				mFxs = new t_fx[mFxCount];

				d_assert (mFxCount * sizeof(t_fx) == ck.dwChunkSize);

				file->Read(mFxs, sizeof(t_fx) * mFxCount);
			}

			else if (ck.dwFlag == STR_FLAG)
			{
				mStrTableSize = ck.dwChunkSize;
				mStrTable= new char[mStrTableSize];

				file->Read(mStrTable, mStrTableSize);
			}

			else if (ck.dwFlag == GEOS_FLAG)
			{
				_loadGeoset(file, ck.dwChunkSize);
			}

			else if (ck.dwFlag == NODE_FLAG)
			{
				mNodeCount = ck.dwChunkSize / sizeof (t_node);

				d_assert (mNodeCount * sizeof(t_node) == ck.dwChunkSize);

				mNodes = new t_node[mNodeCount];
				file->Read(mNodes, ck.dwChunkSize);
			}

			else if (ck.dwFlag == NTRACK_FLAG)
			{
				_loadTrack(file, ck.dwChunkSize);
			}

			else
			{
				file->Skip(ck.dwChunkSize);
			}
		}
	}

	void t_xmdl::_loadHead(DataStreamPtr & file)
	{
		t_head head;

		file->Read(&head, sizeof (head));

		d_assert (head.dwFlag == XMDL_FLAG && head.dwVersion == XMDL_VER);
	}

	void t_xmdl::_loadGeoset(DataStreamPtr & file, int size)
	{
		t_geoset geoset;

		file->Read(&geoset, sizeof(t_geoset));

		size -= sizeof (t_geoset);

		if (size == 0 || (geoset.dwFlags & GEOSET_COLLISIONPOLYGON) || (geoset.dwFlags & GEOSET_BRIGE))
		{
			file->Skip(size);
			return ;
		}

		t_mesh * mesh = new t_mesh();

		mesh->mGeoset = geoset;

		while (size > 0)
		{
			t_chunk ck;
			file->Read(&ck, sizeof(t_chunk));

			if (ck.dwFlag == GEOSET_VERTEX)
			{
				mesh->mVertexCount = ck.dwChunkSize / sizeof (Vec3);
				mesh->mVertex = new Vec3[mesh->mVertexCount];

				d_assert (mesh->mVertexCount * sizeof (Vec3) == ck.dwChunkSize);

				file->Read(mesh->mVertex, mesh->mVertexCount * sizeof(Vec3));
			}
			else if (ck.dwFlag == GEOSET_NORMAL)
			{
				mesh->mVertexCount = ck.dwChunkSize / sizeof (Vec3);
				mesh->mNormal = new Vec3[mesh->mVertexCount];

				d_assert (mesh->mVertexCount * sizeof (Vec3) == ck.dwChunkSize);

				file->Read(mesh->mNormal, mesh->mVertexCount * sizeof(Vec3));
			}

			else if (ck.dwFlag == GEOSET_TEXCOORD)
			{
				mesh->mVertexCount = ck.dwChunkSize / sizeof (Vec2);
				mesh->mUV = new Vec2[mesh->mVertexCount];

				d_assert (mesh->mVertexCount * sizeof (Vec2) == ck.dwChunkSize);

				file->Read(mesh->mUV, mesh->mVertexCount * sizeof(Vec2));
			}

			else if (ck.dwFlag == GEOSET_SKINS)
			{
				mesh->mVertexCount = ck.dwChunkSize / sizeof (t_blend_infl);
				mesh->mBInfl = new t_blend_infl[mesh->mVertexCount];

				d_assert (mesh->mVertexCount * sizeof (t_blend_infl) == ck.dwChunkSize);

				file->Read(mesh->mBInfl, mesh->mVertexCount * sizeof (t_blend_infl));
			}

			else if (ck.dwFlag == GEOSET_BWEIGHT)
			{
				mesh->mSkinCount = ck.dwChunkSize / sizeof (t_skin);
				mesh->mSkin = new t_skin[mesh->mSkinCount];

				d_assert (mesh->mSkinCount * sizeof (t_skin) == ck.dwChunkSize);

				file->Read(mesh->mSkin, mesh->mSkinCount * sizeof (t_skin));
			}

			else if (ck.dwFlag == GEOSET_INDEX)
			{
				mesh->mIndexCount = ck.dwChunkSize / sizeof (WORD);
				mesh->mIndex = new WORD[mesh->mIndexCount];

				d_assert (mesh->mIndexCount * sizeof (WORD) == ck.dwChunkSize);

				file->Read(mesh->mIndex, mesh->mIndexCount * sizeof(WORD));
			}

			else if (ck.dwFlag == GEOSET_PARAM)
			{
				mesh->mTextureId = _loadTexture(file, ck.dwChunkSize);
			}

			else
			{
				file->Skip(ck.dwChunkSize);
			}

			size -= sizeof(t_chunk) + ck.dwChunkSize;

			d_assert (size >= 0);
		}

		mMeshes.PushBack(mesh);
	}

	int t_xmdl::_loadTexture(DataStreamPtr & file, int size)
	{
		enum FxParamType
		{
			FXPARAM_UNKNOWN = -1,
			FXPARAM_TEXTURE = 0,
			FXPARAM_FLOAT = 1,	/*!<Property of Type float*/
			FXPARAM_POINT3 = 2,	/*!<Property of Type Point3*/
			FXPARAM_INT = 3,	/*!<Property of Type int*/
			FXPARAM_POINT4 = 4,	/*!<Property of Type Point4*/
			FXPARAM_LAST,
		};

		struct FxParamChunk2
		{
			BYTE byType;		// 数据类型
			BYTE bAnimated;		// 是否动画
			WORD wParam;		// 参数名
		};

		struct TrackChunk
		{
			BYTE byTrackType;			// TrackType
			BYTE byInterpType;			// InterpolationType
			WORD wTrackSubType;			// 轨迹类型 NodeTrackType/UVWTrackType/参数ID/FxParamType
			WORD wKeyCount;				// 关键桢数量
			WORD wKeySize;				// 关键桢结构大小
		};

		size_t nFxParamSize[FXPARAM_LAST] = 
		{
			sizeof(void*),
			sizeof(float),
			sizeof(float)*3,
			sizeof(int),
			sizeof(float)*4,
		};

		BYTE buffer[1024 * 64];

		d_assert (size < 1024 * 64);

		file->Read(buffer, size);

		BYTE * p = (BYTE *)buffer;

		while ((int)(p - buffer) < size)
		{
			FxParamChunk2 * ck2 = (FxParamChunk2*)p;
			p += sizeof(FxParamChunk2);

			if (!ck2->bAnimated)
			{
				void * val = (void *)p;
				p += nFxParamSize[ck2->byType];

				if (ck2->byType == FXPARAM_TEXTURE)
				{
					return *(BYTE*)val;
				}
			}
			else
			{
				TrackChunk * pTrackChunk = (TrackChunk*)p;
				p += sizeof(TrackChunk);
				BYTE * pTrackData = (BYTE*)p;
				p += pTrackChunk->wKeyCount * pTrackChunk->wKeySize;
			}
		}

		return -1;
	}

	void t_xmdl::_loadTrack(DataStreamPtr & file, int size)
	{
		struct BoneAnimChunk
		{
			// 动画信息
			USHORT nFrames;		// 骨骼动画桢数
			USHORT nNodes;		// 骨骼总数
		};

		struct NodeTrackChunk
		{
			USHORT nNodeIndex;
			USHORT nTrackCount;
		};

		enum NodeTrackType
		{
			NodeTrack_Position,
			NodeTrack_Rotation,
			NodeTrack_scaling,
			NodeTrack_Visible,
			NodeTrack_Horz,
			NodeTrack_Vert,
			NodeTrack_PosX,
			NodeTrack_PosY,
			NodeTrack_PosZ,
			NodeTrack_EulerX,
			NodeTrack_EulerY,
			NodeTrack_EulerZ,
			NodeTrack_Sample,
			NodeTrack_Max,
		};

		enum InterpolationType
		{
			Interpolation_None,
			Interpolation_Linear,
			Interpolation_TCB,
			Interpolation_Bezier,
			Interpolation_Cubic,
			Interpolation_CatmullRom,
		};

		struct TrackChunk
		{
			BYTE byTrackType;			// TrackType
			BYTE byInterpType;			// InterpolationType
			WORD wTrackSubType;			// 轨迹类型 NodeTrackType/UVWTrackType/参数ID/FxParamType
			WORD wKeyCount;				// 关键桢数量
			WORD wKeySize;				// 关键桢结构大小
		};

		BoneAnimChunk bac;
		file->Read(&bac, sizeof (BoneAnimChunk));
		size -= sizeof(BoneAnimChunk);

		mSkinAnimCount = bac.nNodes;
		mSkinAnims = new t_skin_anim[mSkinAnimCount];

		while (size > 0)
		{
			NodeTrackChunk ntc;
			file->Read(&ntc, sizeof(NodeTrackChunk));
			size -= sizeof(NodeTrackChunk);

			t_skin_anim * anim = &mSkinAnims[ntc.nNodeIndex];
			anim->nNodeIndex = ntc.nNodeIndex;

			for (int i = 0; i < ntc.nTrackCount; ++i)
			{
				TrackChunk tc;
				file->Read(&tc, sizeof(TrackChunk));
				size -= sizeof(TrackChunk);

				InterpolationType eiType = (InterpolationType)tc.byInterpType;
				NodeTrackType etType = (NodeTrackType)tc.wTrackSubType;

				if (etType == NodeTrack_Sample)
				{
					d_assert (tc.wKeySize == sizeof(t_track));

					for (int i = 0; i < tc.wKeyCount; ++i)
					{
						t_track track;
						file->Read(&track, sizeof (t_track));
						anim->vTracks.PushBack(track);
					}
				}
				else
				{
					file->Skip(tc.wKeySize * tc.wKeyCount);
				}

				size -= tc.wKeySize * tc.wKeyCount;
			}
		}

		d_assert (size == 0);
	}


	void t_xmdl::save(const char * filename)
	{
		_saveMesh(filename);
		_saveAnim(filename);
	}

	t_mesh * t_xmdl::_mergeMesh(t_mesh * mesh0, t_mesh * mesh1)
	{
		t_mesh * mesh = new t_mesh;

		int gset = 0;

		const char * strFxName = mStrTable + mFxs[mesh1->mGeoset.byFxId].dwFileNameStr;
		if (strstr(strFxName, "AlphaTest") != NULL)
			gset = 1;

		mesh->mGeoset = (gset == 0) ? mesh0->mGeoset : mesh1->mGeoset;

		mesh->mTextureId = mesh0->mTextureId;

		mesh->mVertexCount = mesh0->mVertexCount + mesh1->mVertexCount;

		mesh->mVertex = new Vec3[mesh->mVertexCount];
		mesh->mNormal = new Vec3[mesh->mVertexCount];
		mesh->mUV = new Vec2[mesh->mVertexCount];

		if (mesh0->mSkinInfo)
		{
			mesh->mSkinInfo = new t_skin_info[mesh->mVertexCount];
		}

		int index = 0;
		for (int i = 0; i < mesh0->mVertexCount; ++i)
		{
			mesh->mVertex[index] = mesh0->mVertex[i];

			if (mesh0->mNormal)
				mesh->mNormal[index] = mesh0->mNormal[i];
			else
				mesh->mNormal[index] = Vec3::UnitY;

			if (mesh0->mUV)
				mesh->mUV[index] = mesh0->mUV[i];
			else
				mesh->mUV[index] = Vec2::Zero;

			if (mesh0->mSkinInfo)
				mesh->mSkinInfo[index] = mesh0->mSkinInfo[i];

			++index;
		}

		for (int i = 0; i < mesh1->mVertexCount; ++i)
		{
			mesh->mVertex[index] = mesh1->mVertex[i];

			if (mesh1->mNormal)
				mesh->mNormal[index] = mesh1->mNormal[i];
			else
				mesh->mNormal[index] = Vec3::UnitY;

			if (mesh1->mUV)
				mesh->mUV[index] = mesh1->mUV[i];
			else
				mesh->mUV[index] = Vec2::Zero;

			if (mesh0->mSkinInfo)
				mesh->mSkinInfo[index] = mesh1->mSkinInfo[i];

			++index;
		}


		// index
		mesh->mIndexCount = mesh0->mIndexCount + mesh1->mIndexCount;
		mesh->mIndex = new WORD[mesh->mIndexCount];
		index = 0;

		for (int i = 0; i < mesh0->mIndexCount; ++i)
		{
			mesh->mIndex[index++] = mesh0->mIndex[i];
		}

		for (int i = 0; i < mesh1->mIndexCount; ++i)
		{
			mesh->mIndex[index++] = mesh1->mIndex[i] + mesh0->mVertexCount;
		}

		return mesh;
	}

	void t_xmdl::_optimize()
	{
		for (int i = 0; i < mMeshes.Size(); )
		{
			t_mesh * mesh = mMeshes[i];

			const char * strFxName = mStrTable + mFxs[mesh->mGeoset.byFxId].dwFileNameStr;
			if (strstr(strFxName, "粒子") != NULL)
			{
				delete mesh;
				mMeshes.Erase(i);
				continue;
			}

			++i;
		}

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			t_mesh * mesh = mMeshes[i];

			if (mesh->mBInfl && mesh->mSkin)
			{
				mesh->mSkinInfo = new t_skin_info[mesh->mVertexCount];

				ZeroMemory(mesh->mSkinInfo, sizeof(t_skin_info) * mesh->mVertexCount);

				for (int v = 0; v < mesh->mVertexCount; ++v)
				{
					unsigned char bindex[4] = { 0 };
					float bweight[4] = {0};

					for (int b = 0; b < 4 && b < mesh->mBInfl[v].wWeightCount; ++b)
					{
						int _i = mesh->mBInfl[v].wWeightOffset + b;
						bindex[b] = mesh->mSkin[_i].byBoneID;
						bweight[b] = mesh->mSkin[_i].fWeight;
					}

					float sum = bweight[0] + bweight[1] + bweight[2] + bweight[3];

					if (sum < 0.1f)
						sum = 1;

					bweight[0] /= sum;
					bweight[1] /= sum;
					bweight[2] /= sum;
					bweight[3] /= sum;

					for (int k = 0; k < 4; ++k)
					{
						mesh->mSkinInfo[v].bindex[k] = bindex[k];
						mesh->mSkinInfo[v].bweight[k] = bweight[k];
					}
				}
			}
			else if (mesh->mSkinCount == 1)
			{
				mesh->mSkinInfo = new t_skin_info[mesh->mVertexCount];

				for (int v = 0; v < mesh->mVertexCount; ++v)
				{
					unsigned char bindex[4] = { mesh->mSkin->byBoneID, 0, 0, 0};
					float bweight[4] = {1, 0, 0, 0};

					for (int k = 0; k < 4; ++k)
					{
						mesh->mSkinInfo[v].bindex[k] = bindex[k];
						mesh->mSkinInfo[v].bweight[k] = bweight[k];
					}
				}
			}
		}

		for (int i = 0; i < mMeshes.Size() - 1; ++i)
		{
			t_mesh * mesh = mMeshes[i];

			for (int j = i + 1; j < mMeshes.Size();)
			{
				t_mesh * nmesh = mMeshes[j];

				if (nmesh->mTextureId == mesh->mTextureId &&
					((nmesh->mSkin != NULL) == (mesh->mSkin != NULL)) &&
					nmesh->mIndexCount + mesh->mIndexCount < 65536)
				{
					t_mesh * newMesh = _mergeMesh(mesh, nmesh);
					delete mesh;
					delete nmesh;

					mMeshes[i] = newMesh;
					mMeshes.Erase(j);
					continue;
				}

				++j;
			}
		}

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			t_mesh * mesh = mMeshes[i];

			if (!mesh->mSkinInfo)
				continue ;

			for (int v = 0; v < mesh->mVertexCount; ++v)
			{
				for (int k = 0; k < 4; ++k)
				{
					if (mesh->mSkinInfo[v].bweight[k] > 0.01f)
						mesh->mSkinInfo[v].bindex[k] = mesh->MapBoneId(mesh->mSkinInfo[v].bindex[k]);
				}
			}

			d_assert (mesh->boneIds.Size() <= MAX_BLEND_MATRIX_VS);
		}
	}

	void t_xmdl::_saveMesh(const char * filename)
	{
		if (mMeshes.Size() == 0)
			return ;

		_optimize();

		File file;

		file.Open(filename, OM_WRITE_BINARY);

		d_assert (file.IsOpen());

		char magic[MODEL_FILE_MAGIC_LEN];
		int version;

		strcpy_s(magic, MODEL_FILE_MAGIC_LEN, MODEL_FILE_MAGIC);
		version = MeshLoader_v1::K_File_Verion;

		file.Write(magic, MODEL_FILE_MAGIC_LEN);
		file.Write(&version, sizeof(int));

		for (int i = 0; i < mMeshes.Size(); ++i)
		{
			t_mesh * mesh = mMeshes[i];

			int ckId = MC_SUBMESH;

			file.Write(&ckId, sizeof(int));

			file.Write(&MeshLoader_v1::K_SubMesh_Version_1, sizeof(int));

			int iVertexCount = mesh->mVertexCount;
			int iIndexCount = mesh->mIndexCount;
			int iVertexElem = MeshLoader_v1::VE_POSITION | MeshLoader_v1::VE_NORMAL | MeshLoader_v1::VE_TEXCOORD;

			if (mesh->mSkinInfo)
				iVertexElem |= MeshLoader_v1::VE_BLENDINDICES | MeshLoader_v1::VE_BLENDWEIGHTS;

			file.Write(&iVertexCount, sizeof(int));
			file.Write(&iIndexCount, sizeof(int));
			file.Write(&iVertexElem, sizeof(int));

			for (int v = 0; v < iVertexCount; ++v)
			{
				file.Write(&mesh->mVertex[v], sizeof(Vec3));

				if (mesh->mNormal)
					file.Write(&mesh->mNormal[v], sizeof(Vec3));
				else
					file.Write(&Vec3::UnitY, sizeof(Vec3));

				if (mesh->mUV)
					file.Write(&mesh->mUV[v], sizeof(Vec2));
				else
					file.Write(&Vec2::Zero, sizeof(Vec2));

				if (mesh->mSkinInfo)
				{
					file.Write(mesh->mSkinInfo[v].bindex, 4 * sizeof(unsigned char));
					file.Write(mesh->mSkinInfo[v].bweight, 4 * sizeof(float));
				}
			}

			file.Write(mesh->mIndex, sizeof(WORD) * iIndexCount);

			int NumBoneIdMaps = mesh->boneIds.Size();
			file.Write(&NumBoneIdMaps, sizeof(int));
			file.Write(&mesh->boneIds[0], NumBoneIdMaps * sizeof(short));

			// write material
			file.Write(&MeshLoader_v1::K_Material_Version, sizeof(int));

			int doubleSide = 0;
			int blendMode = BM_OPATICY;
			Color4 emissive = Color4::Black;
			Color4 ambient = Color4::Gray;
			Color4 diffuse = Color4::White;
			Color4 specular = Color4::Black;
			float specularPower = 80;
			TString128 emissiveMap = "";
			TString128 diffuseMap = "";
			TString128 normalMap = "";
			TString128 specularMap = "";

			if (mesh->mTextureId != -1)
			{
				const char * texture = mStrTable + mTextures[mesh->mTextureId].dwFileNameStr;

				diffuseMap = "Texture\\" + TString128(texture) + ".dds";
			}

			const char * strFxName = mStrTable + mFxs[mesh->mGeoset.byFxId].dwFileNameStr;

			if (strstr(strFxName, "AlphaTest") != NULL)
			{
				blendMode = BM_ALPHA_TEST;
				doubleSide = 1;
			}

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

		if (mModel)
		{
			Aabb bound;
			bound.minimum = Vec3(mModel->bound.fMinimumExtent[0],
				mModel->bound.fMinimumExtent[1],
				mModel->bound.fMinimumExtent[2]);
			bound.maximum = Vec3(mModel->bound.fMaximumExtent[0],
				mModel->bound.fMaximumExtent[1],
				mModel->bound.fMaximumExtent[2]);

			Sphere sph;
			sph.center = bound.GetCenter();
			sph.radius = mModel->bound.fBoundsRadius;

			int ckId = MC_BOUNDS;
			file.Write(&ckId, sizeof(int));
			file.Write(&MeshLoader_v1::K_Bound_Version, sizeof(int));

			file.Write(&bound.minimum, sizeof(Vec3));
			file.Write(&bound.maximum, sizeof(Vec3));

			file.Write(&sph.center, sizeof(Vec3));
			file.Write(&sph.radius, sizeof(float));
		}

		mBoneNames.Clear();

		// skeleton
		if (mNodes != NULL)
		{
			int ckId = MC_SKELETON;
			file.Write(&ckId, sizeof(int));
			file.Write(&MeshLoader_v1::K_Skeleton_Version, sizeof(int));

			file.Write(&mNodeCount, sizeof(int));

			for (int i = 0; i < mNodeCount; ++i)
			{
				TString128 name = mStrTable + mNodes[i].wNameStr;

				int index = 0;
				if (_exsitBoneName(name))
				{
					TString128 boneName = name + "_" + index;

					while (_exsitBoneName(boneName))
						++index;

					name = boneName;
				}

				file.Write(name.c_str(), 128);

				int flag = 0;

				if (mNodes->wFlags & NODE_IS_CSBONE)
					flag |= Bone::K_Flag_CsBone;

				file.Write(&flag, sizeof(int));
				file.Write(mNodes[i].Pos, 3 * sizeof(float));
				file.Write(mNodes[i].Rot, 4 * sizeof(float));

				if (mNodes->wFlags & NODE_IS_CSBONE)
					file.Write(Vec3::Unit, 3 * sizeof(float));
				else
					file.Write(mNodes[i].Scl, 3 * sizeof(float));
			}

			for (int i = 0; i < mNodeCount; ++i)
			{
				short parent = mNodes[i].byParent;
				short child = (short)i;

				if (parent == 255)
					parent = -1;

				d_assert (parent < mNodeCount);

				file.Write(&parent, sizeof(short));
				file.Write(&child, sizeof(short));
			}
		}

		file.Close();
	}

	bool t_xmdl::_exsitBoneName(const TString128 & name)
	{
		for (int i = 0; i < mBoneNames.Size(); ++i)
		{
			if (mBoneNames[i] == name)
				return true;
		}

		mBoneNames.PushBack(name);

		return false;
	}

	void t_xmdl::_saveAnim(const char * filename)
	{
		if (mSkinAnims == NULL)
			return ;

		TString128 animFile = File::RemoveExternName(filename) + ".anim";

		File file;

		file.Open(animFile.c_str(), OM_WRITE_BINARY);

		d_assert (file.IsOpen());

		file.Write(&AnimationLoader::K_Magic, sizeof(int));
		file.Write(&AnimationLoader::K_Version, sizeof(int));

		if (mSkinAnimCount > 0)
		{
			file.Write(&AnimationLoader::K_SkinAnim, sizeof(int));
			file.Write(&AnimationLoader::K_SkinAnimVersion, sizeof(int));

			file.Write(&mSkinAnimCount, sizeof(int));

			for (int i = 0; i < mSkinAnimCount; ++i)
			{
				int boneId = mSkinAnims[i].nNodeIndex;
				int count = mSkinAnims[i].vTracks.Size();

				file.Write(&boneId, sizeof(int));
				file.Write(&count, sizeof(int));

				for (int j = 0; j < count; ++j)
				{
					const t_track & track = mSkinAnims[i].vTracks[j];

					float time = track.nFrame * (1.0f / 30);

					file.Write(&time, sizeof(float));
					file.Write(&track.p, sizeof(Vec3));
					file.Write(&track.q, sizeof(Quat));
					file.Write(&track.s, sizeof(Vec3));
				}
			}
		}
	}
}