//////////////////////////////////////////////////////
// TreeGenerator
//----------------------------------------------------
// #include "TreeGenerator.h"
//
// Author: Graeme Pollard
// Date: 
// Description: 
//////////////////////////////////////////////////////

#pragma once

#include "Mesh.h"
#include "Maths.h"
#include "VertexPositionNormalTexture.h"
#include "CylinderGenerator.h"
#include "Leaves.h"

class TreeGenerator : public Mesh
{
	// Public variables
public:
	int					BranchLevels;
	int					Seed;
	float				MinBranchAngle;
	float				MaxBranchAngle;
	float				Length;
	float				MinLeafHeight;
	int					MinLeafLevel;
	int					LeafsPerBranch;
	float				LeafOffset;
	float				MinLeafSize;
	float				MaxLeafSize;
	float				Radius;
	float				MinLength;
	float				MinLengthMultiplier;
	float				MaxLengthMultiplier;
	float				TrunkRadiusMultiplier;
	float				MinRadius;
	float				RadiusMultiplier;
	Leaves*				mLeaves;

	// Constructors
	TreeGenerator(void);

	// Destructors
	~TreeGenerator();

	// Methods
	void				Generate(DirectXDevice* device);

	// Private variables
private:
	void				GenerateBranch(MatrixU matrix, float radius, float length, int segments, int subDivLevel);

	CylinderGenerator*	cylinderGenerator;
	VertexPositionNormalTexture*	mTempVertices;
	VertexPositionNormalTexture*	mNextVertex;
	int					mTotalVertices;
	int*				mTempIndices;
	int*				mNextIndex;
	int					mTotalIndices;
	int					mTotalPrimitives;
};