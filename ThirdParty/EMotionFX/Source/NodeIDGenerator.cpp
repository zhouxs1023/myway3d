/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NodeIDGenerator.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
NodeIDGenerator::NodeIDGenerator()
{
}


// destructor
NodeIDGenerator::~NodeIDGenerator()
{
}


// get an unique id for the given string
int NodeIDGenerator::GenerateIDForString(const String& nodeName)
{
	for (int i=0; i<mNodeNames.GetLength(); i++)
	{
		if (mNodeNames[i] == nodeName)
			return i;
	}

	mNodeNames.Add( nodeName );

	return mNodeNames.GetMaxIndex();
}


// get the name for the given node
const String& NodeIDGenerator::GetNodeName(const int id)
{
	assert( id < mNodeNames.GetLength() );

	return mNodeNames[id];
}

} // namespace EMotionFX