#include "PCH.hpp"
#include "TreeGenerator.h"
#include "CylinderGenerator.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexPositionNormalTextureTangent.h"
#include "Log.h"

#include "Model.h"

#include "MemoryDefines.h"

//--------------------------------------------------------------------//
// Constructors
//--------------------------------------------------------------------//
TreeGenerator::TreeGenerator() : Mesh()
{
	mTotalVertices = 0;
	mTotalIndices = 0;
	mTotalPrimitives = 0;
	Seed = 0;
	mName = "Tree";
}
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
// Destructor
//--------------------------------------------------------------------//
TreeGenerator::~TreeGenerator()
{
	delete mLeaves;
}
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
// Methods
//--------------------------------------------------------------------//
//Generate the tree mesh
void TreeGenerator::Generate(DirectXDevice* device)
{
	srand(Seed);

	mLeaves = new Leaves();

	mTempVertices = new VertexPositionNormalTexture[100000];
	mTempIndices = new int[100000 * 3];

	mNextVertex = mTempVertices;
	mNextIndex = mTempIndices;

	Matrix identity;
	D3DXMatrixIdentity(&identity);

	//Create a cylinder generator
	cylinderGenerator = new CylinderGenerator();

	//Create trunk
	GenerateBranch(identity, Radius, Length, 8, 0);

	//Create roots
	int roots = 0;
	for(int i = 0; i < roots; i++)
	{
		float width = (0.5f + 0.5f * rand() / (float)RAND_MAX) * Length * 0.08f;

		mNextVertex->X = cos(i / (float)roots * D3DX_PI * 2) * Radius;
		mNextVertex->Z = sin(i / (float)roots * D3DX_PI * 2) * Radius;
		mNextVertex->Y = width * 0.4f;
		mNextVertex->tU = 0.5f;
		mNextVertex->tV = 0;
		mNextVertex++;

		mNextVertex->X = cos(((float)i + 0.5f) / (float)roots * D3DX_PI * 2) * Radius;
		mNextVertex->Z = sin(((float)i + 0.5f) / (float)roots * D3DX_PI * 2) * Radius;
		mNextVertex->Y = 0.0f;
		mNextVertex->tU = 0.3f;
		mNextVertex->tV = 0;
		mNextVertex++;

		mNextVertex->X = cos(((float)i - 0.5f) / (float)roots * D3DX_PI * 2) * Radius;
		mNextVertex->Z = sin(((float)i - 0.5f) / (float)roots * D3DX_PI * 2) * Radius;
		mNextVertex->Y = 0.0f;
		mNextVertex->tU = 0.7f;
		mNextVertex->tV = 0;
		mNextVertex++;

		
		mNextVertex->X = cos(i / (float)roots * D3DX_PI * 2) * (Radius + width);
		mNextVertex->Z = sin(i / (float)roots * D3DX_PI * 2) * (Radius + width);
		mNextVertex->Y = - width * 0.4f;
		mNextVertex->tU = 0.5f;
		mNextVertex->tV = 0.4f;
		mNextVertex++;

		*mNextIndex = mTotalVertices;
		mNextIndex++;
		*mNextIndex = mTotalVertices + 3;
		mNextIndex++;
		*mNextIndex = mTotalVertices + 1;
		mNextIndex++;
		*mNextIndex = mTotalVertices;
		mNextIndex++;
		*mNextIndex = mTotalVertices + 2;
		mNextIndex++;
		*mNextIndex = mTotalVertices + 3;
		mNextIndex++;

		mTotalIndices += 6;
		mTotalVertices += 4;
	}

	//Create vertex and index buffer
	SetVertexData(new VertexBuffer(*device, mTotalVertices * sizeof(VertexPositionNormalTextureTangent), VertexPositionNormalTextureTangent::vertexDeclaration->GetDeclaration( )), 0);
	mIndices[0] = new IndexBuffer(*device, mTotalIndices * sizeof(int));

	//Setup bounding box
	Vector3 minimum = Vector3(100000000,10000000,100000000);
	Vector3 maximum = Vector3(-100000000,-100000000,-100000000);
	Vector3 boxMin;
	for(int i = 0; i < mTotalVertices; i++)
	{
		boxMin = Vector3(mTempVertices[i].X, mTempVertices[i].Y, mTempVertices[i].Z);

		//Minimum
		if (boxMin.x < minimum.x)
			minimum.x = boxMin.x;
		if (boxMin.y < minimum.y)
			minimum.y = boxMin.y;
		if (boxMin.z < minimum.z)
			minimum.z = boxMin.z;

		//Maximum
		if (boxMin.x > maximum.x)
			maximum.x = boxMin.x;
		if (boxMin.y > maximum.y)
			maximum.y = boxMin.y;
		if (boxMin.z > maximum.z)
			maximum.z = boxMin.z;

		//Add to collision mesh
		mVertexPositions.push_back(Vector3(boxMin.x, boxMin.y, boxMin.z));
	}

	//Create bounding box
	mBoundingBox.Position = (minimum + maximum) / 2;
	mBoundingBox.Dimensions = (maximum - minimum) / 2;

	//Place vertices and indices into buffer
	VertexPositionNormalTextureTangent* verticesWTangents = Model::CalculateTangents(mTempVertices, mTotalVertices, mTempIndices, mTotalIndices / 3);
	mVertices[0]->SetData<VertexPositionNormalTextureTangent>(verticesWTangents);
	mIndices[0]->SetData(mTempIndices);

	//Setup leaves
	mLeaves->SetIntoBuffers(device);

	Log->WriteLine(LOG_TARGET_USER, "Tree generated with %i triangles and %i leaves", mTotalPrimitives, mLeaves->TotalLeaves() + 1);

	//Remove the cylinder generator
	delete cylinderGenerator;	
	delete [] verticesWTangents;
	delete [] mTempIndices;
}
//--------------------------------------------------------------------//
//Generate a branch
void TreeGenerator::GenerateBranch(MatrixU matrix, float radius, float length, int segments, int subDivLevel)
{
	// Make branch mesh
	//////////////////////////////////////////////////

	//Create the branch
	if (subDivLevel == 0)
		cylinderGenerator->Generate(Vector3(0,0,0), Vector3(0,length,0), radius * TrunkRadiusMultiplier, max(MinRadius, radius * RadiusMultiplier), segments, 10);
	else
	{
		if (subDivLevel == BranchLevels)
			cylinderGenerator->Generate(Vector3(0,0,0), Vector3(0,length,0), radius, 0, segments);
		else
			cylinderGenerator->Generate(Vector3(0,0,0), Vector3(0,length,0), radius, max(MinRadius, radius * RadiusMultiplier), segments);
	}

	//Apply the matrix to each point
	int i = 0;
	for(char* vec = (char*)&(cylinderGenerator->mVertices[0].X); 
		i < cylinderGenerator->mNumVertices; 
		vec += sizeof(VertexPositionNormalTexture))
	{
		Vector3* position = (Vector3*)vec;
		D3DXVec3TransformCoord(position, position, &matrix);
		i++;
	}

	//Apply the matrix to each normal
	Matrix rot = matrix;
	rot._41 = 0;
	rot._42 = 0;
	rot._43 = 0;
	i = 0;
	for(char* vec = (char*)&(cylinderGenerator->mVertices[0].nX); 
		i < cylinderGenerator->mNumVertices; 
		vec += sizeof(VertexPositionNormalTexture))
	{
		Vector3* position = (Vector3*)vec;
		D3DXVec3TransformCoord(position, position, &rot);
		i++;
	}

	//Make Leaves 'n' shit yo
	for (int j = 0; j < LeafsPerBranch; j++)
	{
		Vector3* branchPosition = (Vector3*)&cylinderGenerator->mVertices[0].X;
		Vector3 position = Vector3(rand()/(float)RAND_MAX - 0.5, rand()/(float)RAND_MAX - 0.5, rand()/(float)RAND_MAX - 0.5) * LeafOffset;
		position += *branchPosition;
		if (position.y > MinLeafHeight && subDivLevel > MinLeafLevel)
			mLeaves->AddCluster(position, MinLeafSize + ((float)rand() / (float)RAND_MAX) * (MaxLeafSize - MinLeafSize));
	}

	//Increment indices
	for(i = 0; i < cylinderGenerator->mNumIndices; i++)
	{
		cylinderGenerator->mIndices[i] += mTotalVertices;
	}
	mTotalIndices += cylinderGenerator->mNumIndices;
	mTotalVertices += cylinderGenerator->mNumVertices;
	mTotalPrimitives += cylinderGenerator->mNumPrimitives;

	//Copy into array
	memcpy(mNextVertex, cylinderGenerator->mVertices, cylinderGenerator->mNumVertices * sizeof(VertexPositionNormalTexture));
	mNextVertex += cylinderGenerator->mNumVertices;
	memcpy(mNextIndex, cylinderGenerator->mIndices, cylinderGenerator->mNumIndices * sizeof(int));
	mNextIndex += cylinderGenerator->mNumIndices;

	//Create new branches
	//////////////////////////////////////////////////

	//If the level we're on is less than the max allowed, make more
	if (subDivLevel < BranchLevels)
	{
		float yRot = rand() / (float)RAND_MAX * 3.14159f * 2;
		float xRot[2];
		xRot[0] = ((MinBranchAngle) + (rand() / (float)RAND_MAX) * (MaxBranchAngle - MinBranchAngle));
		xRot[1] = xRot[0] - (MaxBranchAngle + MinBranchAngle);
		for (int i = 0; i < 2; i++)
		{
			//Translate along branch length
			Matrix translate;
			D3DXMatrixTranslation(&translate, 0, length, 0);
			//D3DXMatrixIdentity(&translate);

			//Make a random y rotation 
			Matrix yRotation;
			D3DXMatrixRotationY(&yRotation, yRot);

			//Make a random x rotation from -90 - 90 degrees
			Matrix xRotation;
			D3DXMatrixRotationX(&xRotation, xRot[i]);

			//Multiply together
			Matrix rotation;
			D3DXMatrixMultiply(&rotation, &xRotation, &yRotation);

			//Finalise
			Matrix total;
			D3DXMatrixMultiply(&total, &rotation, &translate);

			//Multiply with previous matrix
			Matrix newBranchMatrix;
			D3DXMatrixMultiply(&newBranchMatrix, &total, &matrix);

			//Make the branch
			float newLength = min(MaxLengthMultiplier * length, max(length * MinLengthMultiplier, length * cos(xRot[i])));
			newLength = max(MinLength, newLength);
			GenerateBranch(newBranchMatrix, max(MinRadius, radius * RadiusMultiplier), newLength, segments - 1, subDivLevel + 1);
		}

		//Random branch along length
		if (subDivLevel == 0)
		{
			int lvls = 2;
			float last_angle = 0;
			for (int i = 0; i < lvls; i++)
			{
				while ( abs( last_angle - yRot ) < 2 || abs( ( 6.28f - last_angle ) - yRot ) < 2 )
				{
					yRot = rand() / (float)RAND_MAX * 3.14159f * 2;
				}
				last_angle = yRot;
				xRot[0] = ((MinBranchAngle) + (rand() / (float)RAND_MAX) * (MaxBranchAngle - MinBranchAngle)) * (3.14159f / 2.0f);
			
				//Translate along branch length
				Matrix translate;
				D3DXMatrixTranslation(&translate, 0, 0.4f * length + i / (float)lvls * length * 0.6f, 0);

				//Make a random y rotation 
				Matrix yRotation;
				D3DXMatrixRotationY(&yRotation, yRot);

				//Make a random x rotation from -90 - 90 degrees
				Matrix xRotation;
				D3DXMatrixRotationX(&xRotation, xRot[0]);

				//Multiply together
				Matrix rotation;
				D3DXMatrixMultiply(&rotation, &xRotation, &yRotation);

				//Finalise
				Matrix total;
				D3DXMatrixMultiply(&total, &rotation, &translate);

				//Multiply with previous matrix
				Matrix newBranchMatrix;
				D3DXMatrixMultiply(&newBranchMatrix, &total, &matrix);

				//Make the branch
				float newLength = min(MaxLengthMultiplier * length, max(length * MinLengthMultiplier, length * cos(xRot[i])));
				newLength = max(MinLength, newLength);
				GenerateBranch(newBranchMatrix, max(MinRadius, radius * RadiusMultiplier), newLength, segments - 1, subDivLevel + 1);
			}
		}
	}
}
//--------------------------------------------------------------------//