#include "MWSkeletonLoader.h"
#include "MWSkeletonManager.h"
#include "MWResourceManager.h"


/*
-----------------------------------------------------------------------------------------------------------
    Format:
        head
            -- magic
            -- version
        bone
            -- count
            -- data
                -- name
                -- position
                -- orientation
                -- scale

        hierarchy
            -- count
            -- data
                -- parent
                -- child

        animations
            -- name
            -- time
                -- skeleton animation       (chunk)
                    -- bone index
                    -- key frames
                        -- count
                        -- time
                        -- translate
                        -- rotate
                        -- scale

-----------------------------------------------------------------------------------------------------------
*/

#define CHUNK_SIZE              6

using namespace Myway;


bool SkeletonLoader::ReadChunk(chunk & ck, DataStreamPtr & stream)
{
    return stream->Read(&ck.id, sizeof(short)) == sizeof(short) &&
           stream->Read(&ck.length, sizeof(int)) == sizeof(int);
}

void SkeletonLoader::Load(Skeleton * skel, const TString128 & source)
{
	DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source.c_str());

	if (stream == NULL)
		return ;

	skel->SetRelative(true);

    ReadHead(stream);

    chunk ck;
    while (ReadChunk(ck, stream))
    {
        switch (ck.id)
        {
        case SC_BONE:
            ReadBone(skel, stream);
            break;

        case SC_HIERARCHY:
            ReadHierarchy(skel, stream);
            break;

        case SC_ANIMATION:
			d_assert (0);
            break;

        default:
            debug_assert(0, "error: skeleton file.");
            break;
        }
    }

    stream->Close();

    if (skel != NULL && skel->GetJointCount() > MAX_BLEND_MATRIX_VS)
    {
        LOG_PRINT_FORMAT("Warning: mesh '%s' joint count more than %d, so using soft skined.\n",
            source.c_str(), MAX_BLEND_MATRIX_VS);
    }
}

void SkeletonLoader::ReadHead(DataStreamPtr & stream)
{
    String sMagic;
    int iVersion;

   stream->ReadString(sMagic);
   stream->Read(&iVersion, sizeof(int));

   d_assert (sMagic == SKELETON_FILE_MAGIC && iVersion == SKELETON_FILE_VERSION);
}

void SkeletonLoader::ReadBone(Skeleton * skel, DataStreamPtr & stream)
{
    int count;
    String name;
    joint * bn;

    stream->Read(&count, sizeof(int));
    assert(count > 0);

    for (int i = 0; i < count; ++i)
    {
        stream->ReadString(name);
        bn = skel->CreateJoint(name.c_str());

		stream->Read(&bn->position, sizeof(Vec3));
		stream->Read(&bn->orientation, sizeof(Quat));
		stream->Read(&bn->scale, sizeof(Vec3));
    }
}

void SkeletonLoader::ReadHierarchy(Skeleton * skel, DataStreamPtr & stream)
{
    int count;
    short parent;
    short child;

    stream->Read(&count, sizeof(int));
    assert(count > 0);

    for (int i = 0; i < count; ++i)
    {
        stream->Read(&parent, sizeof(short));
        stream->Read(&child, sizeof(short));
        skel->SetupHiberarchy(parent, child);
    }
}







int SkeletonLoader::ComputeBoneSize(Skeleton * skel)
{
    int count = (int)skel->GetJointCount();
    d_assert(count < 65536);

    joint * bn;
    int size = 0;
    for (short i = 0; i < count; ++i)
    {
        bn = skel->GetJoint(i);
        size += (bn->sName.Length() + 1) * sizeof(char);
        size += sizeof(float) * (3 + 4 + 3);
    }

    return size + sizeof(int);
}

int SkeletonLoader::ComputeHierarchySize(Skeleton * skel)
{
    return skel->GetHiberarchyCount() * (sizeof(short) + sizeof(short)) + sizeof(int);
}
