/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */


// constructor
inline SkinInfluence::SkinInfluence(Node* bone, const float weight, const int boneNr) : mBone(bone), mWeight(weight), mBoneNr(boneNr)
{
}


// get the bone of this influence
inline Node* SkinInfluence::GetBone() const
{ 
	return mBone; 
}


// get the weight of this influence
inline float SkinInfluence::GetWeight() const
{
	return mWeight;
}


// adjust the pointer to the bone
inline void SkinInfluence::SetBone(Node* bone)
{
	mBone = bone;
}


// adjust the weight value
inline void SkinInfluence::SetWeight(const float& weight)
{
	mWeight = weight;
}


// set the bone number, used for precalculations
inline void SkinInfluence::SetBoneNr(const int boneNr)
{
	mBoneNr = boneNr;
}


// get the bone number, which is used for precalculations
inline int SkinInfluence::GetBoneNr() const
{
	return mBoneNr;
}


// get a given incluence
inline SkinInfluence& SkinningInfoVertexAttributeLayer::GetInfluence(const int attributeNr, const int influenceNr)
{
	return mInfluences[attributeNr][influenceNr];
}


// get the number of influences
inline int SkinningInfoVertexAttributeLayer::GetNumInfluences(const int attributeNr)
{
	return mInfluences[attributeNr].GetLength();
}