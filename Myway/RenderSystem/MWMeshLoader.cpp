#include "MWMeshLoader.h"
#include "MWMeshManager.h"
#include "MWResourceManager.h"

#define _GEOM_STREAM    0
#define _LIGHT_STREAM   1
#define _TEXC_STREAM    2
#define _ANIM_STREAM    3

using namespace Myway;


bool MeshLoader::ReadChunk(chunk & ck, DataStreamPtr & stream)
{
    return stream->Read(&ck.id, sizeof(short)) == sizeof(short) &&
           stream->Read(&ck.length, sizeof(int)) == sizeof(int);
}

void MeshLoader::Load(MeshPtr mesh, DataStreamPtr stream)
{
    profile_code();

    ReadHeader(stream);

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

    stream->Close();

    if (!hasBound)
        mesh->CalcuBounds();
}

void MeshLoader::ReadHeader(DataStreamPtr & stream)
{
    String magic;
    int version;

    stream->ReadString(magic);
    debug_assert(MODEL_FILE_MAGIC == magic, "this isn't Myway model file.");

    stream->Read(&version, sizeof(int));

#if 0

    char * v = (char*)&version;
    char c0 = v[0];
    char c1 = v[1];
    char c2 = v[2];
    char c3 = v[3];

#endif

    debug_assert(MODEL_FILE_VERSION == version, "this model file isn't version 0.");
}

void MeshLoader::ReadSubMesh(SubMesh * sm, DataStreamPtr & stream)
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

    assert(sm->GetVertexStream()->GetStream(_GEOM_STREAM).NotNull());

    //skip
    if (!stream->Eof())
        stream->Skip(-MC_CHUNK_SIZE);
}

void MeshLoader::ReadSkeleton(MeshPtr mesh, DataStreamPtr & stream)
{
    String sSkeletonName;
    stream->ReadString(sSkeletonName);
    mesh->SetSkeletonName(sSkeletonName.c_str());
}

void MeshLoader::ReadBounds(MeshPtr mesh, DataStreamPtr & stream)
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

void MeshLoader::ReadTransStream(SubMesh * sm, DataStreamPtr & stream)
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

void MeshLoader::ReadLightStream(SubMesh * sm, DataStreamPtr & stream)
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

void MeshLoader::ReadTexcStream(SubMesh * sm, DataStreamPtr & stream)
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

void MeshLoader::ReadAnimStream(SubMesh * sm, DataStreamPtr & stream)
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

void MeshLoader::ReadIndexStream(SubMesh * sm, DataStreamPtr & stream)
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

void MeshLoader::ReadDeclaration(short id, VertexDeclarationPtr & decl, DataStreamPtr & stream)
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

void MeshLoader::ReadVertexStream(VertexBufferPtr & vb, int & stride, int count, DataStreamPtr & stream)
{
    EXCEPTION_DEBUG(vb.IsNull(), "model file error.");

    stream->Read(&stride, sizeof(int));

    vb = VideoBufferManager::Instance()->CreateVertexBuffer(stride * count);
    
    void * data = vb->Lock(0, 0, LOCK_DISCARD);

    stream->Read(data, stride * count);

    vb->Unlock();
}




int MeshLoader::ComputeSubMeshSize(SubMesh * sm)
{
    int size = 0;

    //vertex count, index count, primtive count, primtive type
    size += sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);

    //material name
    //size += (sm->GetMaterialName().Length() + 1) * sizeof(char);

    //transfrom, light, texcoord. animation, index stream
    size += ComputeTransStreamSize(sm) + MC_CHUNK_SIZE;

    if (sm->GetVertexStream()->GetStream(_LIGHT_STREAM).NotNull())
        size += ComputeLightStreamSize(sm) + MC_CHUNK_SIZE;

    if (sm->GetVertexStream()->GetStream(_TEXC_STREAM).NotNull())
        size += ComputeTexcStreamSize(sm) + MC_CHUNK_SIZE;

    if (sm->GetVertexStream()->GetStream(_ANIM_STREAM).NotNull())
        size += ComputeAnimStreamSize(sm) + MC_CHUNK_SIZE;

    if (sm->GetIndexStream()->GetStream().NotNull())
        size += ComputeIndexStreamSize(sm) + MC_CHUNK_SIZE;
    
    return size;
}

int MeshLoader::ComputeBoundsSize(SubMesh * sm)
{
    return sizeof(float) * 10;
}


int MeshLoader::ComputeTransStreamSize(SubMesh * sm)
{
    VertexElement ve[MAX_ELEMENT];
    int count;
    int size = 0;
    sm->GetVertexStream()->GetDeclaration()->GetStreamItem(_GEOM_STREAM, ve, count);
    EXCEPTION_DEBUG(count != 0, "transfrom stream error.");

    size += (sizeof(short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char)) * count;

    return size + sizeof(int) + sm->GetVertexStream()->GetStream(_GEOM_STREAM)->GetSize();
}

int MeshLoader::ComputeLightStreamSize(SubMesh * sm)
{
    VertexElement ve[MAX_ELEMENT];
    int count;
    int size = 0;
    sm->GetVertexStream()->GetDeclaration()->GetStreamItem(_LIGHT_STREAM, ve, count);
    EXCEPTION_DEBUG(count != 0, "light stream error.");

    size += (sizeof(short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char)) * count;

    return size + sizeof(int) + sm->GetVertexStream()->GetStream(_LIGHT_STREAM)->GetSize();
}

int MeshLoader::ComputeTexcStreamSize(SubMesh * sm)
{
    VertexElement ve[MAX_ELEMENT];
    int count;
    int size = 0;
    sm->GetVertexStream()->GetDeclaration()->GetStreamItem(_TEXC_STREAM, ve, count);
    EXCEPTION_DEBUG(count != 0, "texcoord stream error.");

    size += (sizeof(short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char)) * count;

    return size + sizeof(int) + sm->GetVertexStream()->GetStream(_TEXC_STREAM)->GetSize();
}

int MeshLoader::ComputeAnimStreamSize(SubMesh * sm)
{
    VertexElement ve[MAX_ELEMENT];
    int count;
    int size = 0;
    sm->GetVertexStream()->GetDeclaration()->GetStreamItem(_ANIM_STREAM, ve, count);
    EXCEPTION_DEBUG(count != 0, "animation stream error.");

    size += (sizeof(short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char)) * count;

    return size + sizeof(int) + sm->GetVertexStream()->GetStream(_ANIM_STREAM)->GetSize();
}

int MeshLoader::ComputeIndexStreamSize(SubMesh * sm)
{
    return sizeof(unsigned char) + sizeof(int) + sm->GetIndexStream()->GetStream()->GetSize();
}