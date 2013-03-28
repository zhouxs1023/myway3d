#pragma once

#include "MWMesh.h"

namespace Myway
{


/*
-----------------------------------------------------------------------------------------------------------
    Format:
            Header.
                -- magic
                -- version

            submesh
                -- vertex count
                -- index count
                -- primtive count
                -- primtive type
                -- material
                -- transfrom stream
                        -- declaraion
                                -- offset
                                -- type
                                -- usage
                                -- index

                        -- stream       (position)
                                -- stride
                                -- data

                -- light stream
                        -- declaraion
                        -- stream       (normal, color)

                -- texture stream
                        -- declaraion
                        -- stream       (texture coord, tangent)

                -- anim stream
                        -- declaraion
                        -- stream       (blend indices, blend weight)

                -- index stream
                         -- stride
                         -- data

            bounds_info
                -- aabb
                -- sphere

            skeleton
-----------------------------------------------------------------------------------------------------------
*/

#define MC_SUBMESH                          0x0100
#define MC_BOUNDS                           0x0200
#define MC_SKELETON                         0x0300
#define MC_SKELANIM							0x0400

#define MC_SUBMESH_TRANSFROM_STREAM         0x0101
#define MC_SUBMESH_LIGHT_STREAM             0x0102
#define MC_SUBMESH_TEXCOORD_STREAM          0x0103
#define MC_SUBMESH_ANIMATION_STREAM         0x0104
#define MC_SUBMESH_INDEX_STREAM             0x0105

#define MC_STREAM_VERTEX_DATA               0x0201
#define MC_STREAM_DECLARATION               0x0202

#define MODEL_FILE_MAGIC                    "Myway Model"
#define MODEL_FILE_MAGIC_LEN				(12)
#define MODEL_FILE_VERSION                  (('M' << 24) | ('D' << 16) | ('L' << 8) | 0)

#define MC_CHUNK_SIZE                       6

/* 
----------------------------------------------------------------------------
----------------------------------------------------------------------------
*/
class MW_ENTRY MeshLoader
{
public:
    static void Load(MeshPtr pMesh, DataStreamPtr stream);
};




class MeshLoader_v0
{
	struct chunk
	{
		short id;
		int length;
	};

	static bool ReadChunk(chunk & ck, DataStreamPtr & stream)
	{
		return stream->Read(&ck.id, sizeof(short)) && stream->Read(&ck.length, sizeof(int));
	}


public:
	static void Load(MeshPtr pMesh, DataStreamPtr stream);

protected:
	static void ReadSubMesh(SubMesh * sm, DataStreamPtr & stream);
	static void ReadSkeleton(MeshPtr mesh, DataStreamPtr & stream);
	static void ReadBounds(MeshPtr mesh, DataStreamPtr & stream);
	static void ReadTransStream(SubMesh * sm, DataStreamPtr & stream);
	static void ReadLightStream(SubMesh * sm, DataStreamPtr & stream);
	static void ReadTexcStream(SubMesh * sm, DataStreamPtr & stream);
	static void ReadAnimStream(SubMesh * sm, DataStreamPtr & stream);
	static void ReadIndexStream(SubMesh * sm, DataStreamPtr & stream);
	static void ReadDeclaration(short id, VertexDeclarationPtr & decl, DataStreamPtr & stream);
	static void ReadVertexStream(VertexBufferPtr & vb, int & stride, int count, DataStreamPtr & stream);
};


class MeshLoader_v1
{
public:
	struct chunk
	{
		int id;
	};

	static bool ReadChunk(chunk & ck, DataStreamPtr & stream)
	{
		return stream->Read(&ck.id, sizeof(int)) == 1;
	}

public:
	enum eVertexElement {
		VE_POSITION = 1 << 0,
		VE_NORMAL = 1 << 1,
		VE_COLOR = 1 << 2,
		VE_TANGENT = 1 << 3,
		VE_TEXCOORD = 1 << 4,
		VE_LIGHTMAPUV = 1 << 5,
		VE_BLENDWEIGHTS = 1 << 6,
		VE_BLENDINDICES = 1 << 7
	};

	static const int K_File_Verion = MODEL_FILE_VERSION + 1;
	
	static const int K_SubMesh_Version = 0;
	static const int K_Material_Version = 0;
	static const int K_Bound_Version = 0;
	static const int K_Skeleton_Version = 0;
	static const int K_SkelAnim_Version = 0;

public:
	static void Load(MeshPtr pMesh, DataStreamPtr stream);

protected:
	static void ReadSubMesh(SubMesh * sm, DataStreamPtr & stream);
	static void ReadMaterial(SubMesh * sm, DataStreamPtr & stream);
	static void ReadSkeleton(MeshPtr mesh, DataStreamPtr & stream);
	static void ReadSkelAnim(MeshPtr mesh, DataStreamPtr & stream);
	static void ReadBounds(MeshPtr mesh, DataStreamPtr & stream);

	static int GenVertexDecl(VertexDeclarationPtr decl, int vertexElems);
};

}