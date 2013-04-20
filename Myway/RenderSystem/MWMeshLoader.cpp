#include "MWMeshLoader.h"
#include "MWMaterial.h"
#include "MWMeshManager.h"
#include "MWResourceManager.h"
#include "MWSkeletonLoader.h"

#define _GEOM_STREAM    0
#define _LIGHT_STREAM   1
#define _TEXC_STREAM    2
#define _ANIM_STREAM    3

using namespace Myway;



void MeshLoader::Load(MeshPtr mesh, DataStreamPtr stream)
{
    profile_code();

	char magic[MODEL_FILE_MAGIC_LEN];
	int version;

	stream->Read(magic, MODEL_FILE_MAGIC_LEN);

	d_assert (strcmp(magic, MODEL_FILE_MAGIC) == 0);

	stream->Read(&version, sizeof(int));

	if (version == MODEL_FILE_VERSION)
		MeshLoader_v0::Load(mesh, stream);
	else if (version == MeshLoader_v1::K_File_Verion)
		MeshLoader_v1::Load(mesh, stream);
	else
		d_assert (0);

	stream->Close();
}


























void MeshLoader_v0::Load(MeshPtr mesh, DataStreamPtr stream)
{
	bool hasBound = false;

	chunk ck;
	while (ReadChunk(ck, stream) &&
		(ck.id == MC_SUBMESH ||
		ck.id == MC_BOUNDS ||
		ck.id == MC_SKELETON))
	{
		switch (ck.id)
		{
		case MC_SUBMESH:
			ReadSubMesh(mesh->CreateSubMesh(), stream);
			break;

		case MC_BOUNDS:
			ReadBounds(mesh, stream);
			hasBound = TRUE;
			break;

		case MC_SKELETON:
			ReadSkeleton(mesh, stream);
			break;
		}
	}

	if (!hasBound)
		mesh->CalcuBounds();

	TString128 mtlSource = File::RemoveExternName(mesh->GetSourceName()) + ".material";

	DataStreamPtr mtlStream = ResourceManager::Instance()->OpenResource(mtlSource.c_str());

	if (mtlStream.NotNull())
	{
		MaterialLoader loader;
		loader.Load(mesh.c_ptr(), mtlStream);
	}
}


void MeshLoader_v0::ReadSubMesh(SubMesh * sm, DataStreamPtr & stream)
{
    int iVertexCount;
    int iIndexCount;
    int iPrimitiveCount;
    int iPrimitiveType;
    String sMaterial;

    stream->Read(&iVertexCount, sizeof(int));
    stream->Read(&iIndexCount, sizeof(int));
    stream->Read(&iPrimitiveCount, sizeof(int));
    stream->Read(&iPrimitiveType, sizeof(int));

    sm->GetVertexStream()->SetCount(iVertexCount);
    sm->GetIndexStream()->SetCount(iIndexCount);
    sm->SetPrimitiveCount(iPrimitiveCount);
    sm->SetPrimitiveType((PRIMITIVE_TYPE)iPrimitiveType);

    stream->ReadString(sMaterial);
    //sm->SetMaterialName(sMaterial.c_str());

    VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
    sm->GetVertexStream()->SetDeclaration(decl);

    //read sub chunk
    chunk ck;
    while (ReadChunk(ck, stream) &&
           (ck.id == MC_SUBMESH_TRANSFROM_STREAM ||
            ck.id == MC_SUBMESH_LIGHT_STREAM ||
            ck.id == MC_SUBMESH_TEXCOORD_STREAM ||
            ck.id == MC_SUBMESH_ANIMATION_STREAM ||
            ck.id == MC_SUBMESH_INDEX_STREAM))
    {
        switch (ck.id)
        {
        case MC_SUBMESH_TRANSFROM_STREAM:
            debug_assert(sm->GetVertexStream()->GetStream(_GEOM_STREAM).IsNull(), "model file error.");
            ReadTransStream(sm, stream);
            break;

        case MC_SUBMESH_LIGHT_STREAM:
            debug_assert(sm->GetVertexStream()->GetStream(_LIGHT_STREAM).IsNull(), "model file error.");
            ReadLightStream(sm, stream);
            break;

        case MC_SUBMESH_TEXCOORD_STREAM:
            debug_assert(sm->GetVertexStream()->GetStream(_TEXC_STREAM).IsNull(), "model file error.");
            ReadTexcStream(sm, stream);
            break;

        case MC_SUBMESH_ANIMATION_STREAM:
            debug_assert(sm->GetVertexStream()->GetStream(_ANIM_STREAM).IsNull(), "model file error.");
            ReadAnimStream(sm, stream);
            break;

        case MC_SUBMESH_INDEX_STREAM:
            debug_assert(sm->GetIndexStream()->GetStream().IsNull(), "model file error.");
            ReadIndexStream(sm, stream);
            break;
        }
    }

    decl->Init();

    d_assert(sm->GetVertexStream()->GetStream(_GEOM_STREAM).NotNull());

    //skip
    if (!stream->Eof())
        stream->Skip(-MC_CHUNK_SIZE);
}

void MeshLoader_v0::ReadSkeleton(MeshPtr mesh, DataStreamPtr & stream)
{
    String sSkeletonName;
    stream->ReadString(sSkeletonName);
	
	TString128 Source = mesh->GetSourceName();
	Source = File::GetFileDir(Source);

	if (Source == "")
	{
		Source = sSkeletonName.c_str();
	}
	else
	{
		Source += "\\";
		Source += sSkeletonName.c_str();
	}

	SkeletonLoader::Load(mesh->GetSkeleton(), Source);
    //mesh->SetSkeletonName(sSkeletonName.c_str());
}

void MeshLoader_v0::ReadBounds(MeshPtr mesh, DataStreamPtr & stream)
{
    Vec3 vMin, vMax, vCenter;
    float fRadius;

    stream->Read(&vMin, sizeof(Vec3));
    stream->Read(&vMax, sizeof(Vec3));

    stream->Read(&vCenter, sizeof(Vec3));
    stream->Read(&fRadius, sizeof(float));

    mesh->SetAabb(vMin, vMax);
    mesh->SetBoundingSphere(vCenter, fRadius);
}

void MeshLoader_v0::ReadTransStream(SubMesh * sm, DataStreamPtr & stream)
{
    VertexDeclarationPtr decl = sm->GetVertexStream()->GetDeclaration();
    VertexBufferPtr buffer;
    int stride = 0;

    chunk ck;
    while (ReadChunk(ck, stream) &&
           (ck.id == MC_STREAM_DECLARATION ||
            ck.id == MC_STREAM_VERTEX_DATA))
    {
        switch (ck.id)
        {
        case MC_STREAM_DECLARATION:
            //read declaration
            ReadDeclaration(_GEOM_STREAM, decl, stream);
            break;

        case MC_STREAM_VERTEX_DATA:
            //read data
            ReadVertexStream(buffer, stride, sm->GetVertexStream()->GetCount(), stream);
            break;
        }
    }

    sm->GetVertexStream()->Bind(_GEOM_STREAM, buffer, stride);

    //skip
    if (!stream->Eof())
        stream->Skip(-MC_CHUNK_SIZE);
}

void MeshLoader_v0::ReadLightStream(SubMesh * sm, DataStreamPtr & stream)
{
    VertexDeclarationPtr decl = sm->GetVertexStream()->GetDeclaration();
    VertexBufferPtr buffer;
    int stride = 0;

    chunk ck;
    while (ReadChunk(ck, stream) &&
          (ck.id == MC_STREAM_DECLARATION ||
           ck.id == MC_STREAM_VERTEX_DATA))
    {
        switch (ck.id)
        {
        case MC_STREAM_DECLARATION:
            ReadDeclaration(_LIGHT_STREAM, decl, stream);
            break;

        case MC_STREAM_VERTEX_DATA:
            ReadVertexStream(buffer, stride, sm->GetVertexStream()->GetCount(), stream);
            break;
        }
    }

    sm->GetVertexStream()->Bind(_LIGHT_STREAM, buffer, stride);

    //skip
    if (!stream->Eof())
        stream->Skip(-MC_CHUNK_SIZE);
}

void MeshLoader_v0::ReadTexcStream(SubMesh * sm, DataStreamPtr & stream)
{
    VertexDeclarationPtr decl = sm->GetVertexStream()->GetDeclaration();
    VertexBufferPtr buffer;
    int stride = 0;

    chunk ck;
    while (ReadChunk(ck, stream) &&
           (ck.id == MC_STREAM_DECLARATION ||
            ck.id == MC_STREAM_VERTEX_DATA))
    {
        switch (ck.id)
        {
        case MC_STREAM_DECLARATION:
            ReadDeclaration(_TEXC_STREAM, decl, stream);
            break;

        case MC_STREAM_VERTEX_DATA:
            ReadVertexStream(buffer, stride, sm->GetVertexStream()->GetCount(), stream);
            break;
        }
    }

    sm->GetVertexStream()->Bind(_TEXC_STREAM, buffer, stride);

    //skip
    if (!stream->Eof())
        stream->Skip(-MC_CHUNK_SIZE);
}

void MeshLoader_v0::ReadAnimStream(SubMesh * sm, DataStreamPtr & stream)
{
    VertexDeclarationPtr decl = sm->GetVertexStream()->GetDeclaration();
    VertexBufferPtr buffer;
    int stride = 0;

    chunk ck;
    while (ReadChunk(ck, stream) &&
        (ck.id == MC_STREAM_DECLARATION ||
         ck.id == MC_STREAM_VERTEX_DATA))
    {
        switch (ck.id)
        {
        case MC_STREAM_DECLARATION:
            ReadDeclaration(_ANIM_STREAM, decl, stream);
            break;

        case MC_STREAM_VERTEX_DATA:
            ReadVertexStream(buffer, stride, sm->GetVertexStream()->GetCount(), stream);
            break;
        }
    }

    sm->GetVertexStream()->Bind(_ANIM_STREAM, buffer, stride);

    //skip
    if (!stream->Eof())
        stream->Skip(-MC_CHUNK_SIZE);
}

void MeshLoader_v0::ReadIndexStream(SubMesh * sm, DataStreamPtr & stream)
{
    int count = sm->GetIndexStream()->GetCount();
    unsigned char stride;
    FORMAT format;

    stream->Read(&stride, sizeof(unsigned char));

    switch (stride)
    {
    case sizeof(short):
        format = FMT_INDEX16;
        break;

    case sizeof(int):
        format = FMT_INDEX32;
        break;

    default:
        EXCEPTION_DEBUG(0, "invalid index format.");
        break;
    }

    IndexBufferPtr pIndexBuffer = VideoBufferManager::Instance()->CreateIndexBuffer(stride * count, format);
    void * indices;

    int start;
    stream->Read(&start, sizeof(int));

    indices = pIndexBuffer->Lock(0, 0, LOCK_DISCARD);
    stream->Read(indices, stride * count);
    pIndexBuffer->Unlock();

    sm->GetIndexStream()->Bind(pIndexBuffer, start);
}

void MeshLoader_v0::ReadDeclaration(short id, VertexDeclarationPtr & decl, DataStreamPtr & stream)
{
    short offset;
    unsigned char type;
    unsigned char usage;
    unsigned char index;

    stream->Read(&offset, sizeof(short));
    stream->Read(&type, sizeof(unsigned char));
    stream->Read(&usage, sizeof(unsigned char));
    stream->Read(&index, sizeof(unsigned char));

    decl->AddElement(id, offset, (DECL_TYPE)type, (DECL_USAGE)usage, index);
}

void MeshLoader_v0::ReadVertexStream(VertexBufferPtr & vb, int & stride, int count, DataStreamPtr & stream)
{
    EXCEPTION_DEBUG(vb.IsNull(), "model file error.");

    stream->Read(&stride, sizeof(int));

    vb = VideoBufferManager::Instance()->CreateVertexBuffer(stride * count);
    
    void * data = vb->Lock(0, 0, LOCK_DISCARD);

    stream->Read(data, stride * count);

    vb->Unlock();
}





















void MeshLoader_v1::Load(MeshPtr mesh, DataStreamPtr stream)
{
	bool hasBound = false;

	chunk ck;
	while (ReadChunk(ck, stream))
	{
		switch (ck.id)
		{
		case MC_SUBMESH:
			ReadSubMesh(mesh->CreateSubMesh(), stream);
			break;

		case MC_BOUNDS:
			ReadBounds(mesh, stream);
			hasBound = TRUE;
			break;

		case MC_SKELETON:
			ReadSkeleton(mesh, stream);
			break;

		case MC_SKELANIM:
			ReadSkelAnim(mesh, stream);
			break;
		}
	}

	if (!hasBound)
		mesh->CalcuBounds();
}

void MeshLoader_v1::ReadSubMesh(SubMesh * sm, DataStreamPtr & stream)
{
	const int K_Version = 0;

	int iVersion;
	int iVertexCount;
	int iIndexCount;
	int iVertexElems;

	stream->Read(&iVersion, sizeof(int));

	d_assert (iVersion == K_SubMesh_Version || iVersion == K_SubMesh_Version_1);

	stream->Read(&iVertexCount, sizeof(int));
	stream->Read(&iIndexCount, sizeof(int));
	stream->Read(&iVertexElems, sizeof(int));

	int iPrimitiveCount = iIndexCount / 3;
	int iPrimitiveType = PRIM_TRIANGLELIST;

	d_assert (iVertexElems | VE_POSITION);

	sm->GetVertexStream()->SetCount(iVertexCount);
	sm->GetIndexStream()->SetCount(iIndexCount);
	sm->SetPrimitiveCount(iPrimitiveCount);
	sm->SetPrimitiveType((PRIMITIVE_TYPE)iPrimitiveType);

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	
	int vstride = GenVertexDecl(decl, iVertexElems);

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(vstride * iVertexCount);

	void * vdata = vb->Lock(0, 0, LOCK_NORMAL);
	{
		stream->Read(vdata, vstride * iVertexCount);
	}
	vb->Unlock();

	FORMAT indexFormat = FMT_INDEX16;
	if (iIndexCount > 65535)
		indexFormat = FMT_INDEX32;

	int istride = (indexFormat == FMT_INDEX16) ? 2 : 4;
	IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(istride * iIndexCount, indexFormat);

	void * idata = ib->Lock(0, 0, LOCK_NORMAL);
	{
		stream->Read(idata, istride * iIndexCount);
	}
	ib->Unlock();

	sm->GetVertexStream()->SetDeclaration(decl);
	sm->GetVertexStream()->Bind(0, vb, vstride);
	sm->GetIndexStream()->Bind(ib, 0);

	if (iVersion >= K_SubMesh_Version_1)
	{
		Array<short> & boneMap = sm->GetBoneIdMap();

		int numBoneMap = 0;

		stream->Read(&numBoneMap, sizeof(int));

		if (numBoneMap > 0 && numBoneMap < MAX_BLEND_MATRIX_VS)
		{
			boneMap.Resize(numBoneMap);

			stream->Read(&boneMap[0], sizeof(short) * numBoneMap);
		}
	}

	ReadMaterial(sm, stream);
}

void MeshLoader_v1::ReadMaterial(SubMesh * sm, DataStreamPtr & stream)
{
	int version;
	int doubleSide, blendMode;
	Color4 emissive, ambient, diffuse, specular;
	float specularPower;
	char emissiveMap[128], diffuseMap[128], normalMap[128], specularMap[128];

	stream->Read(&version, sizeof(int));

	d_assert (version == K_Material_Version);

	stream->Read(&doubleSide, sizeof(int));
	stream->Read(&blendMode, sizeof(int));

	stream->Read(&emissive, sizeof(Color4));
	stream->Read(&ambient, sizeof(Color4));
	stream->Read(&diffuse, sizeof(Color4));
	stream->Read(&specular, sizeof(Color4));
	stream->Read(&specularPower, sizeof(float));

	stream->Read(emissiveMap, 128);
	stream->Read(diffuseMap, 128);
	stream->Read(normalMap, 128);
	stream->Read(specularMap, 128);

	Material * mtl = sm->GetMaterial();

	if (doubleSide == 1)
	{
		mtl->SetCullMode(CULL_NONE);
	}

	mtl->SetBlendMode((BLEND_MODE)blendMode);
	mtl->SetAlphaTestRef(0.5f);

	mtl->SetEmissive(emissive);
	mtl->SetAmbient(ambient);
	mtl->SetDiffuse(diffuse);
	mtl->SetSpecular(specular);

	mtl->SetEmissiveMap(emissiveMap);
	mtl->SetDiffuseMap(diffuseMap);
	mtl->SetNormalMap(normalMap);
	mtl->SetSpecularMap(specularMap);
}

void MeshLoader_v1::ReadSkeleton(MeshPtr mesh, DataStreamPtr & stream)
{
	Skeleton * skel = mesh->GetSkeleton();
	int version = -1;

	stream->Read(&version, sizeof (int));

	d_assert (version == K_Skeleton_Version);

	int boneCount = 0;

	stream->Read(&boneCount, sizeof (int));

	for (int i = 0; i < boneCount; ++i)
	{
		char Name[128];
		int flag;
		Vec3 Position;
		Quat Orientation;
		Vec3 Scale;

		stream->Read(&Name[0],  128);
		stream->Read(&flag, sizeof(int));
		stream->Read(&Position, sizeof(Vec3));
		stream->Read(&Orientation, sizeof(Quat));
		stream->Read(&Scale, sizeof(Vec3)); // equal ratio scale.

		joint * bn = skel->CreateJoint(Name);

		bn->flag = flag;
		bn->position = Position;
		bn->orientation = Orientation;
		bn->scale = Scale;
	}

	for (int i = 0; i < boneCount; ++i)
	{
		short parent;
		short child;

		stream->Read(&parent, sizeof(short));
		stream->Read(&child, sizeof(short));

		skel->SetupHiberarchy(parent, child);
	}
}

void MeshLoader_v1::ReadSkelAnim(MeshPtr mesh, DataStreamPtr & stream)
{
	Skeleton * skel = mesh->GetSkeleton();
	int version = -1;

	stream->Read(&version, sizeof (int));

	d_assert (version == K_SkelAnim_Version);

	char name[128];

	stream->Read(name, 128);

	Animation * anim = mesh->CreateAnimation(name);

	int numAnims = 0;
	stream->Read(&numAnims, sizeof(int));

	for (int i = 0; i < numAnims; ++i)
	{
		int boneId, count;

		stream->Read(&boneId, sizeof(int));
		stream->Read(&count, sizeof(int));

		if (count == 0)
			continue;

		SkeletonAnimation * skel_anim;
		skel_anim = anim->CreateSkeletonAnimation(boneId);

		KeyFrame * kf;
		float time;
		Vec3 trans;
		Quat rotate;
		Vec3 scale;

		for (int i = 0; i < count; ++i)
		{
			stream->Read(&time, sizeof(float));
			stream->Read(&trans, sizeof(float) * 3);
			stream->Read(&rotate, sizeof(float) * 4);
			stream->Read(&scale, sizeof(float) * 3);

			kf = skel_anim->CreateKeyFrame();
			kf->SetTime(time);
			kf->SetTranslate(trans);
			kf->SetRotation(rotate);
			kf->SetScale(scale);
		}
	}

	anim->_calcuLength();
}

void MeshLoader_v1::ReadBounds(MeshPtr mesh, DataStreamPtr & stream)
{
	int iVerison;
	Vec3 vMin, vMax, vCenter;
	float fRadius;

	stream->Read(&iVerison, sizeof(int));

	d_assert (iVerison == K_Bound_Version);

	stream->Read(&vMin, sizeof(Vec3));
	stream->Read(&vMax, sizeof(Vec3));

	stream->Read(&vCenter, sizeof(Vec3));
	stream->Read(&fRadius, sizeof(float));

	mesh->SetAabb(vMin, vMax);
	mesh->SetBoundingSphere(vCenter, fRadius);
}

int MeshLoader_v1::GenVertexDecl(VertexDeclarationPtr decl, int vertexElems)
{
	int voffset = 0;

	if (vertexElems & VE_POSITION)
	{
		decl->AddElement(0, voffset, DT_FLOAT3, DU_POSITION, 0);
		voffset += 12;
	}

	if (vertexElems & VE_NORMAL)
	{
		decl->AddElement(0, voffset, DT_FLOAT3, DU_NORMAL, 0);
		voffset += 12;
	}

	if (vertexElems & VE_COLOR)
	{
		decl->AddElement(0, voffset, DT_FLOAT4, DU_COLOR, 0);
		voffset += 16;
	}

	if (vertexElems & VE_TANGENT)
	{
		decl->AddElement(0, voffset, DT_FLOAT3, DU_TANGENT, 0);
		voffset += 12;
	}

	if (vertexElems & VE_TEXCOORD)
	{
		decl->AddElement(0, voffset, DT_FLOAT2, DU_TEXCOORD, 0);
		voffset += 8;
	}

	if (vertexElems & VE_LIGHTMAPUV)
	{
		decl->AddElement(0, voffset, DT_FLOAT2, DU_TEXCOORD, 1);
		voffset += 8;
	}

	if (vertexElems & VE_BLENDINDICES)
	{
		decl->AddElement(0, voffset, DT_UBYTE4, DU_BLENDINDICES, 0);
		voffset += 4;
	}

	if (vertexElems & VE_BLENDWEIGHTS)
	{
		decl->AddElement(0, voffset, DT_FLOAT4, DU_BLENDWEIGHT, 0);
		voffset += 16;
	}

	decl->Init();

	return voffset;
}