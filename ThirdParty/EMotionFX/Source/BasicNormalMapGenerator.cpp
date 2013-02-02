/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "BasicNormalMapGenerator.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "StandardMaterial.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
BasicNormalMapGenerator::BasicNormalMapGenerator() : NormalMapGenerator()
{
	mTotalProgress  = 0.0f;
	mPartProgress	= 0.0f;
	mResChecker		= new ImageResChecker();
}


// destructor
BasicNormalMapGenerator::~BasicNormalMapGenerator()
{
	delete mResChecker;
}


// get the unique type identification number of this generator
int BasicNormalMapGenerator::GetType() const
{
	return BasicNormalMapGenerator::TYPE_ID;
}


// get the description of this generator
const char* BasicNormalMapGenerator::GetTypeString()
{
	return "BasicNormalMapGenerator";
}


// get progress state
float BasicNormalMapGenerator::GetProgress() const
{
	return mTotalProgress;
}


// get part progress state
float BasicNormalMapGenerator::GetPartProgress() const
{
	return mPartProgress;
}


// the main method, which generates a set of normal maps from a low and high detail model
void BasicNormalMapGenerator::Generate(Actor* lowDetail, Actor* highDetail, bool tangentSpace, bool postFilter)
{
	// get rid of the previous normal maps
	ReleaseNormalMaps();

	// reset some member values
	mTotalFaces		= 0;
	mTotalProgress	= 0;
	mPartProgress	= 0;

	// generate an array of normal map parts
	MCore::Array<NormalMapPart>	parts;
	const int numNodes = lowDetail->GetNumNodes();
	int n;
	for (n=0; n<numNodes; n++)
	{
		Node* curNode = lowDetail->GetNode(n);
		Mesh* curMesh = curNode->GetMesh(0).GetPointer();

		// skip if there is no mesh
		if (curMesh == NULL) continue;

		// get the first UV set
		UVVertexAttributeLayer* uvLayer = curMesh->GetUVLayer( 0 );

		// we cannot generate if the uv data isn't there
		if (uvLayer == NULL) continue;

		const int numSubMeshes = curMesh->GetNumSubMeshes();
		for (int s=0; s<numSubMeshes; s++)
		{
			SubMesh* subMesh = curMesh->GetSubMesh(s);

			String mapName;
			int material = subMesh->GetMaterial();
			Material* baseMaterial = lowDetail->GetMaterial(0, material);
			if (baseMaterial->GetType() == StandardMaterial::TYPE_ID)
			{
				StandardMaterial* mat = (StandardMaterial*)baseMaterial;

				// if there are no texure layers, simply give it the name of the material
				if (mat->GetNumLayers() == 0)
				{
					mapName = mat->GetName();
				}
				else
				{
					// if there are map layers, try to find the diffuse layer
					int layerNr = mat->FindLayer( StandardMaterialLayer::LAYERTYPE_DIFFUSE );
					if (layerNr == -1)
						mapName = mat->GetName();
					else
						mapName = mat->GetLayer(layerNr)->GetFilename();
				}
			}
			else
				mapName = baseMaterial->GetName();

			// add the part
			NormalMapPart newPart;
			newPart.mMaterialNr = material;
			newPart.mMesh = curMesh;
			newPart.mNode = curNode;
			newPart.mSubMesh = subMesh;
			newPart.mDiffuseName = mapName;
			parts.Add(newPart);

			mTotalFaces += subMesh->GetNumIndices() / 3;

			// find the normalmap for this part
			bool found = false;
			for (int i=0; i<mNormalMaps.GetLength() && !found; i++)
			{
				if (mNormalMaps[i]->GetName() == parts.GetLast().mDiffuseName)
				{
					parts.GetLast().mNormalMap = mNormalMaps[i];
					found = true;
				}
			}

			// if no normalmap found, create one
			if (!found)
			{
				const int RES = mDefaultRes;

				int width, height;
				if (mResChecker != NULL)
				{
					if (!mResChecker->GetResolution(mapName, &width, &height))
					{
						width  = RES;
						height = RES;
					}
					else
					{
						width  *= mRatio;
						height *= mRatio;
					}
				}
				else
				{
					width  = RES;
					height = RES;
				}

				MCore::LOG("Allocating normalmap '%s' (%dx%d)", mapName.GetReadPtr(), width, height);
				NormalMap* normalMap = new NormalMap(width, height, mapName);
				mNormalMaps.Add( normalMap );
				parts.GetLast().mNormalMap = normalMap;
			}
		}
	}


	int curFace = 0;

	// process all normalmaps
	const int numParts = parts.GetLength();
	for (n=0; n<numParts; n++)
	{
		Node* curNode = parts[n].mNode;
		Mesh* curMesh = parts[n].mMesh;

		// make sure we have generated our tangent info
		curMesh->CalcTangents(0);	// use the first UV set

		// get some pointers to direct mesh data
		int*			indices		= curMesh->GetIndices();
		MCore::Vector3*	positions	= curMesh->GetPositions();
		MCore::Vector3*	normalData	= curMesh->GetNormals();
		MCore::Vector4*	tangentData	= curMesh->GetTangents();

		// get the first UV set
		UVVertexAttributeLayer* uvLayer = curMesh->GetUVLayer( 0 );
		MCore::Vector2*			uvData	= NULL;	

		if (uvLayer)
			uvData = uvLayer->GetUVs();

		// for all submeshes
		SubMesh* subMesh = parts[n].mSubMesh;

		// get the index offset of the submesh
		const int startIndex = subMesh->GetStartIndex();

		// find the normalmap number
		mCurNormalMap = mNormalMaps.Find( parts[n].mNormalMap );

		// for all faces in the submesh of the lowpoly model
		const int numIndices = subMesh->GetNumIndices();
		for (int f=0; f<numIndices; f+=3)
		{
			// get the face indices
			int indexA = indices[startIndex + f];
			int indexB = indices[startIndex + f + 1];
			int indexC = indices[startIndex + f + 2];

			// get the face positions
			MCore::Vector3 fPos[3];
			fPos[0] = positions[ indexA ];
			fPos[1] = positions[ indexB ];
			fPos[2] = positions[ indexC ];

			MCore::DVector3 pos[3];
			pos[0].Set( fPos[0].x, fPos[0].y, fPos[0].z );
			pos[1].Set( fPos[1].x, fPos[1].y, fPos[1].z );
			pos[2].Set( fPos[2].x, fPos[2].y, fPos[2].z );

			// get the face uvs
			MCore::DVector2 uv[3];
			uv[0] = MCore::DVector2(uvData[indexA].x, uvData[indexA].y);
			uv[1] = MCore::DVector2(uvData[indexB].x, uvData[indexB].y);
			uv[2] = MCore::DVector2(uvData[indexC].x, uvData[indexC].y);

			// get the normals
			MCore::Vector3 fNormals[3];
			fNormals[0] = normalData[ indexA ];
			fNormals[1] = normalData[ indexB ];
			fNormals[2] = normalData[ indexC ];

			MCore::DVector3 normals[3];
			normals[0].Set( fNormals[0].x, fNormals[0].y, fNormals[0].z );
			normals[1].Set( fNormals[1].x, fNormals[1].y, fNormals[1].z );
			normals[2].Set( fNormals[2].x, fNormals[2].y, fNormals[2].z );				

			// get the tangents
			MCore::Vector4 fTangents[3];
			fTangents[0] = tangentData[ indexA ];
			fTangents[1] = tangentData[ indexB ];
			fTangents[2] = tangentData[ indexC ];

			MCore::DVector4 tangents[3];
			tangents[0].Set( fTangents[0].x, fTangents[0].y, fTangents[0].z, fTangents[0].w );
			tangents[1].Set( fTangents[1].x, fTangents[1].y, fTangents[1].z, fTangents[1].w );
			tangents[2].Set( fTangents[2].x, fTangents[2].y, fTangents[2].z, fTangents[2].w );				

			// process the current face
			ProcessFace(pos, uv, normals, tangents, parts[n].mNormalMap, highDetail, curNode, tangentSpace, postFilter);

			curFace++;
			mPartProgress = ((f+1) / (float)numIndices) * 100.0f;
			mTotalProgress= ((curFace+1) / (float)mTotalFaces) * 100.0f;
		} // for all faces (indices)
	} // for all normalmaps parts


	for (int i=0; i<mNormalMaps.GetLength(); i++)
	{
		// save the normalmaps to disk
		NormalMap* normalMap = mNormalMaps[i];

		// fix the errors, and fill the empty pixels
		normalMap->FixErrors();

		// fill the empty pixels
		normalMap->FillEmptyPixels();

		// now it is time to blur the image
		if (postFilter)
			normalMap->Blur();

		// renormalize the image
		normalMap->Normalize();

		// if bumpmap output
		//normalMap->FixBump();
	}
}


void BasicNormalMapGenerator::ProcessFace(MCore::DVector3 pos[3], MCore::DVector2 uv[3], MCore::DVector3 normals[3], MCore::DVector4 tangents[3], NormalMap* normalMap, Actor* highDetail, Node* node, bool tangentSpace, bool postFilter)
{
	// calculate the poly normal
	MCore::DVector3 polyNormal = (pos[1] - pos[0]).Cross( pos[2] - pos[0] ).Normalize();

	// calculate the top most vertex of the 2d triangle
	int	top	= 0;
	if (uv[1].y < uv[0].y)		top = 1;
	if (uv[2].y < uv[top].y)	top = 2;

	// sort the first
	if (top > 0)
	{
		MCore::Swap<MCore::DVector3>(pos[0], pos[top]);
		MCore::Swap<MCore::DVector2>(uv[0], uv[top]);
		MCore::Swap<MCore::DVector3>(normals[0], normals[top]);
		MCore::Swap<MCore::DVector4>(tangents[0], tangents[top]);
	}

	// sort the remaining two
	if (uv[2].y < uv[1].y)
	{
		MCore::Swap<MCore::DVector3>(pos[1], pos[2]);
		MCore::Swap<MCore::DVector2>(uv[1], uv[2]);
		MCore::Swap<MCore::DVector3>(normals[1], normals[2]);
		MCore::Swap<MCore::DVector4>(tangents[1], tangents[2]);
	}

	// output normalmap resolution
	const int XRES = normalMap->GetWidth();
	const int YRES = normalMap->GetHeight();

	// calculate normalmap space vertex positions
	int x1 = uv[0].x * XRES;
	int y1 = uv[0].y * YRES;
	int x2 = uv[1].x * XRES;
	int y2 = uv[1].y * YRES;
	int x3 = uv[2].x * XRES;
	int y3 = uv[2].y * YRES;

	// calculate normalmap space interpolation steps
	double iX1toX2step = (x2-x1) / (double)(y2-y1);
	double iX1toX3step = (x3-x1) / (double)(y3-y1);
	double iX2toX3step = (x3-x2) / (double)(y3-y2);

	// calculate worldspace interpolation steps
	MCore::DVector3 pX1toX2step = (pos[1] - pos[0]) / (double)(y2 - y1);
	MCore::DVector3 pX1toX3step = (pos[2] - pos[0]) / (double)(y3 - y1);
	MCore::DVector3 pX2toX3step = (pos[2] - pos[1]) / (double)(y3 - y2);

	// calculate tangent and normal interpolation steps
	MCore::DVector4 tX1toX2step = (tangents[1] - tangents[0]) / (double)(y2 - y1);
	MCore::DVector4 tX1toX3step = (tangents[2] - tangents[0]) / (double)(y3 - y1);
	MCore::DVector4 tX2toX3step = (tangents[2] - tangents[1]) / (double)(y3 - y2);
	MCore::DVector3 nX1toX2step = (normals[1] - normals[0]) / (double)(y2 - y1);
	MCore::DVector3 nX1toX3step = (normals[2] - normals[0]) / (double)(y3 - y1);
	MCore::DVector3 nX2toX3step = (normals[2] - normals[1]) / (double)(y3 - y2);

	// process upper part of the triangle
	double iX1toX2 = x1;
	double iX1toX3 = x1;
	MCore::DVector3 pX1toX2 = pos[0];
	MCore::DVector3 pX1toX3 = pos[0];
	MCore::DVector3 nX1toX2 = normals[0];
	MCore::DVector3 nX1toX3 = normals[0];
	MCore::DVector4 tX1toX2 = tangents[0];
	MCore::DVector4 tX1toX3 = tangents[0];
	int y;
	for (y=y1; y<y2; y++) 
	{
		// calculate the start and end of the scanline
		int start, end;
		MCore::DVector3 startPos, endPos, startNormal, endNormal;
		MCore::DVector4 startTangent, endTangent;
		if ((int)iX1toX2 > (int)iX1toX3)
		{
			start		= (int)iX1toX3;
			startPos	= pX1toX3;
			startTangent= tX1toX3;
			startNormal = nX1toX3;
			end			= (int)iX1toX2;
			endPos		= pX1toX2;
			endTangent	= tX1toX2;
			endNormal	= nX1toX2;
		}
		else
		{
			start		= (int)iX1toX2;
			startPos	= pX1toX2;
			startTangent= tX1toX2;
			startNormal = nX1toX2;
			end			= (int)iX1toX3;
			endPos		= pX1toX3;
			endTangent	= tX1toX3;
			endNormal	= nX1toX3;
		}

		double edgeLength = end - start;
		if (edgeLength > 0)
		{
			// calculate the worldspace position step when we increase one texel in the normalmap
			MCore::DVector3 posStep	= (endPos - startPos) / edgeLength;
			MCore::DVector3 nStep	= (endNormal - startNormal) / edgeLength;
			MCore::DVector4 tStep	= (endTangent - startTangent) / edgeLength;

			// if we are in height range of the normalmap
			if (y >= 0 && y < YRES)
			{
				// process all texels on the scanline
				for (int i=start; i<=end; i++)
				{
					// if we are inside width range in the normalmap
					if (i>=0 && i<XRES)
					{
						//MCore::DVector3 startPosD(startPos.x, startPos.y, startPos.z);
						//MCore::DRay rayD(startPos + polyNormal * 1000.0, startPos - polyNormal * 1000.0f);
						MCore::DRay rayD(startPos - startNormal * 0.001f, startPos + startNormal * 1000.0);
						Sample(rayD, highDetail, normalMap, i, y, node, startNormal, startTangent, tangentSpace, postFilter);
					}

					// go to the next worldspace position on the scanline
					startPos += posStep;
					startNormal += nStep;
					startTangent += tStep;
				}
			}
		}

		// update interpolation process
		iX1toX2 += iX1toX2step;	pX1toX2 += pX1toX2step; nX1toX2 += nX1toX2step;	tX1toX2 += tX1toX2step;
		iX1toX3 += iX1toX3step;	pX1toX3 += pX1toX3step;	nX1toX3 += nX1toX3step;	tX1toX3 += tX1toX3step;					
	}


	// process lower part of the triangle
	double iX2toX3 = x2;
	MCore::DVector3 pX2toX3 = pos[1];
	MCore::DVector3 nX2toX3 = normals[1];
	MCore::DVector4 tX2toX3 = tangents[1];
	for (y=y2; y<=y3; y++) 
	{
		// calculate the start and the end of the scanline
		int start, end;
		MCore::DVector3 startPos, endPos, startNormal, endNormal;
		MCore::DVector4 startTangent, endTangent;
		if ((int)iX2toX3 > (int)iX1toX3)
		{
			start		= (int)iX1toX3;
			startPos	= pX1toX3;
			startNormal = nX1toX3;
			startTangent= tX1toX3;
			end			= (int)iX2toX3;
			endPos		= pX2toX3;
			endNormal	= nX2toX3;
			endTangent	= tX2toX3;
		}
		else
		{
			start		= (int)iX2toX3;
			startPos	= pX2toX3;
			startNormal	= nX2toX3;
			startTangent= tX2toX3;
			end			= (int)iX1toX3;
			endPos		= pX1toX3;
			endNormal	= nX1toX3;
			endTangent	= tX1toX3;
		}

		double edgeLength = end - start;
		if (edgeLength > 0) 
		{
			// calculate how much to step in worldspace when we step one texel
			MCore::DVector3 posStep	= (endPos - startPos) / edgeLength;
			MCore::DVector3 nStep	= (endNormal - startNormal) / edgeLength;
			MCore::DVector4 tStep	= (endTangent - startTangent) / edgeLength;

			// if we are in height range of the normalmap image
			if (y >= 0 && y < YRES)
			{
				// sample the scanline
				for (int i=start; i<=end; i++)
				{
					// if we are in width range of the normalmap image
					if (i>=0 && i<XRES)
					{
						//MCore::DVector3 startPosD(startPos.x, startPos.y, startPos.z);
						//MCore::DRay rayD(startPos + polyNormal * 1000.0, startPos - polyNormal * 1000.f);
						MCore::DRay rayD(startPos - startNormal * 0.001f, startPos + startNormal * 1000.0);
						Sample(rayD, highDetail, normalMap, i, y, node, startNormal, startTangent, tangentSpace, postFilter);
					}

					// next 3D position on the scanline
					startPos += posStep;
					startNormal += nStep;
					startTangent += tStep;
				}
			}
		}

		// update interpolation process
		iX2toX3 += iX2toX3step;	pX2toX3 += pX2toX3step; nX2toX3 += nX2toX3step; tX2toX3 += tX2toX3step;
		iX1toX3 += iX1toX3step;	pX1toX3 += pX1toX3step;	nX1toX3 += nX1toX3step;	tX1toX3 += tX1toX3step;
	}
}


void BasicNormalMapGenerator::Sample(const MCore::DRay& ray, Actor* highDetail, NormalMap* normalMap, const int x, const int y, Node* node, MCore::DVector3& normal, MCore::DVector4& tangent, bool tangentSpace, bool postFilter)
{
	// if this pixel is already in use, we can skip it
	if (normalMap->IsFree(x, y) == false)
		return;

	// find the same node in the high detail model
	Node* highNode = highDetail->GetNodeByID( node->GetID() );
	assert( highNode );	// if not found, it means the actors are not the same

	MCore::DVector3 pos, pos1, pos2, normal1, normal2;
	double baryU=0, baryV=0, baryU1, baryU2, baryV1, baryV2;
	double dist1 = FLT_MAX;
	double dist2 = FLT_MAX;
	int startIndex=0, startIndex1, startIndex2;
	bool normalFound = true;

	MCore::DRay ray2(ray.GetOrigin(), ray.GetOrigin() - ray.GetDirection() * 1000.0);
	normal1 = ray.GetDirection();
	normal2 = ray2.GetDirection();

	if (CalcIntersection(ray, highNode, &pos1, &baryU1, &baryV1, &startIndex1))
	{
		dist1 = (pos1 - ray.GetOrigin()).SquareLength();

		// calculate the normal at the intersection point
		Mesh* mesh = highNode->GetMesh(0).GetPointer();
		MCore::Vector3* normals = mesh->GetNormals();
		int* indices = mesh->GetIndices();
		MCore::Vector3 normalA = normals[ indices[startIndex1+0] ];
		MCore::Vector3 normalB = normals[ indices[startIndex1+1] ];
		MCore::Vector3 normalC = normals[ indices[startIndex1+2] ];

		MCore::DVector3 dNormalA(normalA.x, normalA.y, normalA.z);
		MCore::DVector3 dNormalB(normalB.x, normalB.y, normalB.z);
		MCore::DVector3 dNormalC(normalC.x, normalC.y, normalC.z);

		normal1 = MCore::BarycentricInterpolate<MCore::DVector3>(baryU1, baryV1, dNormalA, dNormalB, dNormalC);
		normal1.Normalize();

		if (CalcIntersection(ray2, highNode, &pos2, &baryU2, &baryV2, &startIndex2))
		{
			dist2 = (pos2 - ray2.GetOrigin()).SquareLength();

			// calculate the normal at the intersection point
			Mesh* mesh = highNode->GetMesh(0).GetPointer();
			MCore::Vector3* normals = mesh->GetNormals();
			int* indices = mesh->GetIndices();
			MCore::Vector3 normalA = normals[ indices[startIndex2+0] ];
			MCore::Vector3 normalB = normals[ indices[startIndex2+1] ];
			MCore::Vector3 normalC = normals[ indices[startIndex2+2] ];

			MCore::DVector3 dNormalA(normalA.x, normalA.y, normalA.z);
			MCore::DVector3 dNormalB(normalB.x, normalB.y, normalB.z);
			MCore::DVector3 dNormalC(normalC.x, normalC.y, normalC.z);

			normal2 = MCore::BarycentricInterpolate<MCore::DVector3>(baryU2, baryV2, dNormalA, dNormalB, dNormalC);
			normal2.Normalize();

			double dot1 = normal.Dot( normal1 );
			double dot2 = normal.Dot( normal2 );

			if (dot1 < 0 && dot2 > 0)
			{
				baryU = baryU2;
				baryV = baryV2;
				startIndex = startIndex2;
			}
			else
			{	
				if ((dot1 >= 0 && dot2 >= 0) || (dot1<=0 && dot2 <=0))
				{
					if (dist1 < dist2)
					{
						baryU = baryU1;
						baryV = baryV1;
						startIndex = startIndex1;
					}	
					else
					{
						baryU = baryU2;
						baryV = baryV2;
						startIndex = startIndex2;
					}
				}
				else 
				{
					// dot2 is negative, dot1 is positive
					if (dot2 <= 0 && dot1 >= 0)
					{
						baryU = baryU1;
						baryV = baryV1;
						startIndex = startIndex1;
					}
				}
			}
		}
		else
		{
			baryU = baryU1;
			baryV = baryV1;
			startIndex = startIndex1;
		}
	}
	else
	{
		if (CalcIntersection(ray2, highNode, &pos1, &baryU1, &baryV1, &startIndex1))
		{
			baryU = baryU1;
			baryV = baryV1;
			startIndex = startIndex1;
		}
		else
		{
			normalFound = false;
		}
	}

	// if the normal has been found
	if (normalFound)
	{
		// get the mesh of the high detail node
		Mesh* mesh = highNode->GetMesh(0).GetPointer();
		assert(mesh);

		// get the original normal data (in object space)
		MCore::Vector3* normals = mesh->GetNormals();

		// get the face indices
		int* indices = mesh->GetIndices();

		// get the normals of the vertices of the face
		MCore::Vector3 normalA = normals[ indices[startIndex+0] ];
		MCore::Vector3 normalB = normals[ indices[startIndex+1] ];
		MCore::Vector3 normalC = normals[ indices[startIndex+2] ];

		MCore::DVector3 dNormalA(normalA.x, normalA.y, normalA.z);
		MCore::DVector3 dNormalB(normalB.x, normalB.y, normalB.z);
		MCore::DVector3 dNormalC(normalC.x, normalC.y, normalC.z);

		// calculate the interpolated normal
		MCore::DVector3 interpolatedNormal = MCore::BarycentricInterpolate<MCore::DVector3>(baryU, baryV, dNormalA, dNormalB, dNormalC);

		MCore::DVector3 outNormal;
		if (tangentSpace)
		{
			MCore::DVector3 finalTangent(tangent.x, tangent.y, tangent.z);

			// calculate the binormal
			MCore::DVector3 biNormal = tangent.w * normal.Cross( finalTangent );

			// transform the interpolated normal into tangent space 
			outNormal.x = interpolatedNormal.Dot( finalTangent );
			outNormal.y = interpolatedNormal.Dot( biNormal );
			outNormal.z = interpolatedNormal.Dot( normal );
			outNormal.Normalize();
		}
		else							
			outNormal = interpolatedNormal.Normalize();

		// write the encoded normal into the normalmap
		normalMap->EncodeNormal(x, y, outNormal);
/*
		if (tangentSpace)	// heightmap output test
		{
			float dist;
			float sign;
			if (startIndex == startIndex1)
			{
				dist = dist1;
				sign = 1;
			}
			else
			{
				dist = dist2;
				sign = -1;
			}

			float length = Sqrt(dist) * sign;

			int value = length * 255;

			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].r = value;
			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].g = value;
			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].b = value;
			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].isFree = false;
			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].x = x;
			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].y = y;
			normalMap->GetPixelData()[x+y*normalMap->GetWidth()].dist = 0;
		}*/
	}
	else // the normal has not been found, so write the interpolated lowpoly normal
	{
		MCore::DVector3 outNormal;

		if (tangentSpace)
		{
			MCore::DVector3 finalTangent(tangent.x, tangent.y, tangent.z);

			// calculate the binormal
			MCore::DVector3 biNormal = tangent.w * normal.Cross( finalTangent );

			// transform the interpolated normal into tangent space 
			outNormal.x = normal.Dot( finalTangent );
			outNormal.y = normal.Dot( biNormal );
			outNormal.z = normal.Dot( normal );
			outNormal.Normalize();
		}
		else
			outNormal = normal;

		// write the encoded normal into the normalmap
		normalMap->EncodeNormal(x, y, outNormal);
	}
}



bool BasicNormalMapGenerator::CalcIntersection(const MCore::DRay& ray, Node* node, MCore::DVector3* outPos, double* outBaryU, double* outBaryV, int* outStartIndex)
{
	Mesh* mesh = node->GetMesh(0).GetPointer();

	MCore::Vector3*	positions		= mesh->GetPositions();
	int*			indices			= mesh->GetIndices();
	MCore::DVector3	newOrigin		= ray.GetOrigin();
	MCore::DVector3	newDest			= ray.GetDest();

	double			closestDist		= FLT_MAX;
	bool			hasIntersected	= false;
	int				closestStartIndex=0;
	MCore::DVector3	intersectionPoint;
	MCore::DVector3	closestIntersect;
	double			dist, baryU, baryV, closestBaryU=0, closestBaryV=0;

	// loop through all triangles
	const int numIndices = mesh->GetNumIndices();
	for (int i=0; i<numIndices; i+=3)
	{
		MCore::Vector3 fPosA = positions[indices[i]];
		MCore::Vector3 fPosB = positions[indices[i+1]];
		MCore::Vector3 fPosC = positions[indices[i+2]];
		MCore::DVector3 posA(fPosA.x, fPosA.y, fPosA.z);
		MCore::DVector3 posB(fPosB.x, fPosB.y, fPosB.z);
		MCore::DVector3 posC(fPosC.x, fPosC.y, fPosC.z);

		// test the ray with the triangle (in object space)
		if (ray.Intersects(posA, posB, posC, &intersectionPoint, &baryU, &baryV))
		{
			// calculate the squared distance between the intersection point and the ray origin
			dist = (intersectionPoint - newOrigin).SquareLength();

			// if it is the closest intersection point until now, record it as closest intersection
			if (dist < closestDist)
			{
				closestDist			= dist;
				closestIntersect	= intersectionPoint;
				hasIntersected		= true;
				closestStartIndex	= i;
				closestBaryU		= baryU;
				closestBaryV		= baryV;
			}
		}
	}

	// store the closest intersection point (in world space)
	if (hasIntersected)
	{
		if (outPos != NULL)
			*outPos = closestIntersect;

		if (outStartIndex != NULL)
			*outStartIndex = closestStartIndex;

		if (outBaryU)
			*outBaryU = closestBaryU;

		if (outBaryV)
			*outBaryV = closestBaryV;
	}

	// return the result
	return hasIntersected;
}


void BasicNormalMapGenerator::SetResChecker(ImageResChecker* checker)
{
	if (mResChecker)
		delete mResChecker;

	mResChecker = checker;
}

} // namespace EMotionFX