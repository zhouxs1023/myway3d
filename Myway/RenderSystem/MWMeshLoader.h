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

#define MC_SUBMESH_TRANSFROM_STREAM         0x0101
#define MC_SUBMESH_LIGHT_STREAM             0x0102
#define MC_SUBMESH_TEXCOORD_STREAM          0x0103
#define MC_SUBMESH_ANIMATION_STREAM         0x0104
#define MC_SUBMESH_INDEX_STREAM             0x0105

#define MC_STREAM_VERTEX_DATA               0x0201
#define MC_STREAM_DECLARATION               0x0202

#define MODEL_FILE_MAGIC                    "Myway Model"
#define MODEL_FILE_VERSION                  (('M' << 24) | ('D' << 16) | ('L' << 8) | 0)

#define MC_CHUNK_SIZE                       6

/* 
----------------------------------------------------------------------------
----------------------------------------------------------------------------
*/
class MW_ENTRY MeshLoader
{
    struct chunk
    {
        short id;
        int length;
    };

public:
    static void Load(MeshPtr pMesh, DataStreamPtr stream);

protected:
    static bool ReadChunk(chunk & ck, DataStreamPtr & stream);
    static void ReadHeader(DataStreamPtr & stream);
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

protected:
    static int ComputeSubMeshSize(SubMesh * sm);
    static int ComputeBoundsSize(SubMesh * sm);

    static int ComputeTransStreamSize(SubMesh * sm);
    static int ComputeLightStreamSize(SubMesh * sm);
    static int ComputeTexcStreamSize(SubMesh * sm);
    static int ComputeAnimStreamSize(SubMesh * sm);
    static int ComputeIndexStreamSize(SubMesh * sm);
};

}