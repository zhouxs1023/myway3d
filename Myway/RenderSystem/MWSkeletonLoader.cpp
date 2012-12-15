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
            ReadAnimation(skel, stream);
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

void SkeletonLoader::ReadAnimation(Skeleton * skel, DataStreamPtr & stream)
{
    String name;
    float len;
    Animation * anim;

    stream->ReadString(name);
    stream->Read(&len, sizeof(float));
    anim = skel->CreateAnimation(name.c_str());
    anim->SetLength(len);

    chunk ck;
    while (ReadChunk(ck, stream) &&
           ck.id == SC_SKELETON_ANIMATION)
    {
        switch (ck.id)
        {
        case SC_SKELETON_ANIMATION:
            ReadBoneAnimation(skel, anim, stream);
            break;
        }
    }

    //skip
    if (!stream->Eof())
        stream->Skip(-CHUNK_SIZE);
}

void SkeletonLoader::ReadBoneAnimation(Skeleton * skel, Animation * anim, DataStreamPtr & stream)
{
	short bone_index;
	stream->Read(&bone_index, sizeof(short));

	SkeletonAnimation * skel_anim;
	skel_anim = anim->CreateSkeletonAnimation(bone_index);

	int count;
	stream->Read(&count, sizeof(int));

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


int SkeletonLoader::ComputeAnimationSize(Animation * anim)
{
    int size = 0;

    size += (anim->GetName().Length() + 1) * sizeof(char);
    size += sizeof(float);

    SkeletonAnimation * bn_anim;
    for (int i = 0; i < anim->GetSkeletonAnimationCount(); ++i)
    {
        bn_anim = anim->GetSkeletonAnimation(i);
        size += ComputeSkeletonAnimationSize(bn_anim) + CHUNK_SIZE;
    }

    return size;
}

int SkeletonLoader::ComputeSkeletonAnimationSize(SkeletonAnimation * anim)
{
    int size = 0;

    //bone index
    size += sizeof(short);

    //key frames
    size += sizeof(float) * (1 + 3 + 4 + 3) * anim->GetFrameCount() + sizeof(int);

    return size;
}