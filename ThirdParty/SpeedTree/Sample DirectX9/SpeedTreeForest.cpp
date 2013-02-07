///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest Class
//
//	(c) 2003 IDV, Inc.
//
//	This class is provided to illustrate one way to incorporate
//	SpeedTreeRT into an OpenGL application.  All of the SpeedTreeRT
//	calls that must be made on a per tree basis are done by this class.
//	Calls that apply to all trees (i.e. static SpeedTreeRT functions)
//	are made in the functions in main.cpp.
//
//
//	*** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//	This software is supplied under the terms of a license agreement or
//	nondisclosure agreement with Interactive Data Visualization and may
//	not be copied or disclosed except in accordance with the terms of
//	that agreement.
//
//      Copyright (c) 2001-2003 IDV, Inc.
//      All Rights Reserved.
//
//		IDV, Inc.
//		1233 Washington St. Suite 610
//		Columbia, SC 29201
//		Voice: (803) 799-1699
//		Fax:   (803) 931-0320
//		Web:   http://www.idvinc.com
//


///////////////////////////////////////////////////////////////////////  
//	Include Files

#include "Random.h"
#include "Vector.h"
#include "Filename.h"
#include "SpeedTreeForest.h"
using namespace std;


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest constructor

CSpeedTreeForest::CSpeedTreeForest( ) :
	m_fWindStrength(0.3f)
{
	m_afForestExtents[0] = m_afForestExtents[1] = m_afForestExtents[2] = FLT_MAX;
	m_afForestExtents[3] = m_afForestExtents[4] = m_afForestExtents[5] = -FLT_MAX;
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest destructor

CSpeedTreeForest::~CSpeedTreeForest( )
{
	for (unsigned int i = 0; i < m_vAllTrees.size( ); ++i)
	{
		delete m_vAllTrees[i];
		m_vAllTrees[i] = NULL;
	}
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::Load
//
//	A (null) pFilename value forces the load routine to generate the
//	trees procedurally instead of loading from an STF file

bool CSpeedTreeForest::Load(const char* pFilename)
{
	bool bSuccess = false;

	// setup a few parameters shared by all tree models
	CSpeedTreeRT::SetNumWindMatrices(c_nNumWindMatrices);

	bool bLoaded = false;
	if (pFilename)
		bLoaded = LoadFromStfFile(pFilename);
	else
		bLoaded = LoadProcedurally( );

	if (bLoaded)
	{
		// setup initial wind strength
		SetWindStrength(m_fWindStrength);

		// set LOD limits
		SetLodLimits( );

		// graphics API specific
		bSuccess = InitGraphics( );

		// once the trees are loaded, the geometry buffered, and instances
		// created, we can delete some of the memory in the RT class
		#ifdef WRAPPER_RENDER_VERTEX_OBJECTS
			for (int i = 0; i < m_vAllTrees.size( ); ++i)
				m_vAllTrees[i]->GetSpeedTree( )->DeleteTransientData( );
		#endif
	}
	
	return bSuccess;
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::AdjustExtents

void CSpeedTreeForest::AdjustExtents(float x, float y, float z)
{
    if (m_vAllTrees.size() == 1)
		memcpy(m_afForestExtents, m_vAllTrees[0]->GetBoundingBox( ), 6 * sizeof(float));
    else
    {
        // min
        m_afForestExtents[0] = __min(m_afForestExtents[0], x); 
        m_afForestExtents[1] = __min(m_afForestExtents[1], y); 
        m_afForestExtents[2] = __min(m_afForestExtents[2], z); 

        // max
        m_afForestExtents[3] = __max(m_afForestExtents[3], x); 
        m_afForestExtents[4] = __max(m_afForestExtents[4], y); 
        m_afForestExtents[5] = __max(m_afForestExtents[5], z);
	}
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::LoadFromStfFile

bool CSpeedTreeForest::LoadFromStfFile(const char* pFilename)
{
    bool bSuccess = false;

	// open the file
    int nAttemptedReads = 0, nSuccessfulReads = 0;            
	FILE* pFile = fopen(pFilename, "r");
    if (pFile)
    {
	    // find the end of the file
	    fseek(pFile, 0L, SEEK_END);
	    int nEnd = ftell(pFile);
	    fseek(pFile, 0L, SEEK_SET);

        int nTree = 0;
	    while (ftell(pFile) != nEnd)
	    {
		    // keep track of the tree for error reporting
		    ++nTree;

		    // read the mandatory data
		    char szName[1024];
		    unsigned int nSeed = 0, nNumInstances = 0;
            float fSize = 0.0f, fSizeVariance = 0.0f;
			float afPos[3] = { 0.0f };
            nAttemptedReads++;
		    if (fscanf(pFile, "%s %d %g %g %g %g %g %d\n", szName, &nSeed, &fSize, &fSizeVariance, afPos, afPos + 1, afPos + 2, &nNumInstances) == 8)
            {
                nSuccessfulReads++;
                nAttemptedReads += nNumInstances;
		        // make a new tree structure
		        CSpeedTreeWrapper* pTree = new CSpeedTreeWrapper;

		        // load the tree with its position
		        string strTreeFilename = CFilename(c_pStfFilename).GetPath( ) + szName;
		        if (pTree->LoadTree(strTreeFilename.c_str( ), nSeed, fSize, fSizeVariance))
                {
					// set the tree's location
					pTree->SetPos(afPos);

		            // add tree to the scene and associate it with appropriate poperty nodes
		            m_vAllTrees.push_back(pTree);

		            // instances
		            for (unsigned int i = 0; i < nNumInstances; ++i)
		            {
			            // read the instance location
			            if (fscanf(pFile, "%g %g %g\n", afPos, afPos + 1, afPos + 2) == 3)
                        {
			                // make an instance of this tree
			                CSpeedTreeWrapper* pInstance = pTree->MakeInstance( );
                            if (pInstance)
                            {
								pInstance->SetPos(afPos);
								AdjustExtents(afPos[0], afPos[1], afPos[2]);
								AdjustExtents(afPos[0], afPos[1], afPos[2] + pTree->GetBoundingBox( )[5]);
			                    // add instance to the scene and associate it with appropriate poperty nodes
			                    m_vAllTrees.push_back(pInstance);
                                ++nSuccessfulReads;
                            }
                            else
				                fprintf(stderr, "SpeedTree instancing function failed\n", nTree, pFilename);
                        }
                        else
				            fprintf(stderr, "Error reading STF data [tree %d in %s]\n", nTree, pFilename);
		            }
                }
            }
            else
			    fprintf(stderr, "Error reading STF data [tree %d in %s]\n", nTree, pFilename);
        }
        fclose(pFile);
    }
    else
		fprintf(stderr, "Error opening '%s'\n", pFilename);

    if (nAttemptedReads == nSuccessfulReads &&
        nAttemptedReads > 0)
        bSuccess = true;

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::LoadProcedurally

bool CSpeedTreeForest::LoadProcedurally(void)
{
	// create and load the main tree
	const char** pTreeModelFilename = c_pTreeModelFilenames;
	while (*pTreeModelFilename)
	{
		CSpeedTreeWrapper* pTree = new CSpeedTreeWrapper;
		if (pTree->LoadTree(*pTreeModelFilename))
		{
			float pos[3] = {0, 0, 0};
			pTree->SetPos(pos);
			m_vAllTrees.push_back(pTree);
			// place tree in forest, making sure no trees overlap
			/*if (PlaceTree(pTree))
			{
				// make instances
				bool bPlacementError = false;
				for (int i = 0; i < 1; ++i)
				{
					CSpeedTreeWrapper* pInstance = pTree->MakeInstance( );
					if (!PlaceTree(pInstance))
					{
						bPlacementError = true;
						delete pInstance;
						pInstance = NULL;
						fprintf(stderr, "failed to place instance %d\n", i);
					}
				}
			}
			else
			{
			delete pTree;
			pTree = NULL;
			fprintf(stderr, "failed to place tree %d\n", m_vAllTrees.size( ));
			}*/
		}
		else
			fprintf(stderr, "failed to load tree model [%s] (%s)\n", *pTreeModelFilename, CSpeedTreeRT::GetCurrentError( ));
		pTreeModelFilename++;
	}

	return !m_vAllTrees.empty( );
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::PlaceTree

bool CSpeedTreeForest::PlaceTree(CSpeedTreeWrapper* pTree)
{
	// pick trees random tree positions until one doesn't
	// overlay with the existing trees
	int nIterations = 0;
	bool bPlaced = false;
	while (!bPlaced && nIterations < c_nMaxPlacementIterations)
	{
		float afPos[3] = { GetRandom(-c_fForestSize, c_fForestSize),
						   GetRandom(-c_fForestSize, c_fForestSize),
						   0.0f };

		// Run through the other trees, seeing if they're too close.  This
		// results in O(N^2) time, but this isn't exactly the "huge forest" sample
		bool bTooClose = false;
		for (unsigned int i = 0; i < m_vAllTrees.size( ) && !bTooClose; ++i)
			if (VectorDistance(m_vAllTrees[i]->GetPos( ), afPos) < c_fSpacingTolerance)
				bTooClose = true;

		if (!bTooClose)
		{
			bPlaced = true;

			// store in running list of trees
			pTree->SetPos(afPos);
			m_vAllTrees.push_back(pTree);

			// adjust forest's extents
			AdjustExtents(afPos[0], afPos[1], afPos[2]);
			AdjustExtents(afPos[0], afPos[1], afPos[2] + 0.5f * pTree->GetBoundingBox( )[5]);
		}
		else
			nIterations++;
	}

	return bPlaced;
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::SetWindStrength

void CSpeedTreeForest::SetWindStrength(float fStrength)
{
	m_fWindStrength = fStrength;

	for (unsigned int i = 0; i < m_vAllTrees.size( ); ++i)
		m_vAllTrees[i]->GetSpeedTree( )->SetWindStrength(m_fWindStrength);
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::SetupWindMatrices

void CSpeedTreeForest::SetupWindMatrices(float fTimeInSecs)
{
	// matrix computational data
	static float afMatrixTimes[c_nNumWindMatrices] = { 0.0f };
	static float afFrequencies[c_nNumWindMatrices][2] = 
	{
		{ 0.15f, 0.17f },
		{ 0.25f, 0.15f },
		{ 0.19f, 0.05f },
		{ 0.15f, 0.22f }
	};

	// compute time since last call
	static float fTimeOfLastCall = 0.0f;
	float fTimeSinceLastCall = fTimeInSecs - fTimeOfLastCall;
	fTimeOfLastCall = fTimeInSecs;

	// wind strength
	static float fOldStrength = m_fWindStrength;

	// increment matrix times
	for (int i = 0; i < c_nNumWindMatrices; ++i)
		afMatrixTimes[i] += fTimeSinceLastCall;

	// compute maximum branch throw
	float fBaseAngle = m_fWindStrength * 35.0f;

	// build rotation matrices
	for (int i = 0; i < c_nNumWindMatrices; ++i)
	{
		// adjust time to prevent "jumping"
		if (m_fWindStrength != 0.0f)
			afMatrixTimes[i] = (afMatrixTimes[i] * fOldStrength) / m_fWindStrength;

		// compute percentages for each axis
		float fBaseFreq = m_fWindStrength * 20.0f;
		float fXPercent = sinf(fBaseFreq * afFrequencies[i % c_nNumWindMatrices][0] * afMatrixTimes[i]);
		float fYPercent = cosf(fBaseFreq * afFrequencies[i % c_nNumWindMatrices][1] * afMatrixTimes[i]);

		// build compound rotation matrix (rotate on 'x' then on 'y')
		const float c_fDeg2Rad = 57.2957795f;
        float fSinX = sinf(fBaseAngle * fXPercent / c_fDeg2Rad);
        float fSinY = sinf(fBaseAngle * fYPercent / c_fDeg2Rad);
        float fCosX = cosf(fBaseAngle * fXPercent / c_fDeg2Rad);
        float fCosY = cosf(fBaseAngle * fYPercent / c_fDeg2Rad);

        float afMatrix[16] = { 0.0f };
        afMatrix[0] = fCosY;
        afMatrix[2] = -fSinY;
        afMatrix[4] = fSinX * fSinY;
        afMatrix[5] = fCosX;
        afMatrix[6] = fSinX * fCosY;
        afMatrix[8] = fSinY * fCosX;
        afMatrix[9] = -fSinX;
        afMatrix[10] = fCosX * fCosY;
        afMatrix[15] = 1.0f;

		#ifdef WRAPPER_USE_CPU_WIND
			CSpeedTreeRT::SetWindMatrix(i, afMatrix);
		#endif
		#ifdef WRAPPER_USE_GPU_WIND
			// graphics API specific
			UploadWindMatrix(c_nVertexShader_WindMatrices + i * 4, afMatrix);
		#endif
	}

	// track wind strength
	fOldStrength = m_fWindStrength;
}


///////////////////////////////////////////////////////////////////////  
//	CSpeedTreeForest::SetLodLimits

void CSpeedTreeForest::SetLodLimits(void)
{
	// find tallest tree
	float fTallest = -1.0f;
	for (unsigned int i = 0; i < m_vAllTrees.size( ); ++i)
	{
		float fHeight = m_vAllTrees[i]->GetBoundingBox( )[5] - m_vAllTrees[i]->GetBoundingBox( )[0];
		fTallest = __max(fHeight, fTallest);
	}

	// assign all trees based on tallest
	for (unsigned int i = 0; i < m_vAllTrees.size( ); ++i)
		m_vAllTrees[i]->GetSpeedTree( )->SetLodLimits(fTallest * c_fNearLodFactor, fTallest * c_fFarLodFactor);
}


