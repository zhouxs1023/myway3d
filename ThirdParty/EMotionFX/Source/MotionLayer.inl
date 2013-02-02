/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionLayer.h"


// return the actor to which this layer belongs to
inline Actor* MotionLayer::GetActor() const
{
	return mActor;
}


// return the actor to which this layer belongs to
MotionLayerSystem* MotionLayer::GetMotionSystem() const
{
	return mMotionSystem;
}


// return source motion instance
inline MotionInstance* MotionLayer::GetSource() const
{
	return mSource;
}


// return parent layer
inline MotionLayer* MotionLayer::GetParent() const
{
	return mParent;
}


// return destination layer
inline MotionLayer* MotionLayer::GetDestLayer() const
{
	return mDestLayer;
}


// set the source instance
inline void MotionLayer::SetSource(MotionInstance* source)
{
	mSource = source;
}


// set the parent layer
inline void MotionLayer::SetParent(MotionLayer* parent)
{
	mParent = parent;
}


// set the destination layer
inline void MotionLayer::SetDestLayer(MotionLayer* dest)
{
	mDestLayer = dest;
}

