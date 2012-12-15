#pragma once

#include "MWSkeleton.h"

namespace Myway
{

#define SKELETON_FILE_MAGIC     "Myway Skeleton"
#define SKELETON_FILE_VERSION   (('S' << 24) | ('K' << 16) | ('L' << 8) | 0)
#define SC_BONE                 0x00001000
#define SC_HIERARCHY            0x00002000
#define SC_ANIMATION            0x00003000
#define SC_SKELETON_ANIMATION   0x00003100

class MW_ENTRY SkeletonLoader
{
    struct chunk
    {
        short id;
        int length;
    };

public:
    static void Load(Skeleton * skel, const TString128 & source);

protected:
    static  bool ReadChunk(chunk & ck, DataStreamPtr & stream);

    static void ReadHead(DataStreamPtr & stream);
    static void ReadBone(Skeleton * skel, DataStreamPtr & stream);
    static void ReadHierarchy(Skeleton * skel, DataStreamPtr & stream);
    static void ReadAnimation(Skeleton * skel, DataStreamPtr & stream);
    static void ReadBoneAnimation(Skeleton * skel, Animation * anim, DataStreamPtr & stream);

protected:

    static int ComputeBoneSize(Skeleton * skel);
    static int ComputeHierarchySize(Skeleton * skel);
    static int ComputeAnimationSize(Animation * anim);
    static int ComputeSkeletonAnimationSize(SkeletonAnimation * anim);
};

}