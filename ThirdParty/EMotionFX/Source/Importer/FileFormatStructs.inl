/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FILEFORMATSTRUCTS_INL
#define __FILEFORMATSTRUCTS_INL


/*
	Fileformat description:
	NOTE: [] means an array, so LMA_Normal[] is an array of LMA_Normal structs
	----------------------------------------------------------------------------
	LMA_Header

	while (not end of file)
	{
		Read LMA_Chunk and check the chunk type...

		if (LMA_CHUNK_NODE):
			LMA_Node

		if (LMA_CHUNK_MESH):
			LMA_Mesh
				LMA_SubMesh[mesh.numSubMeshes]
					LMA_SubMeshVertex[subMesh.numVerts]
					int[subMesh.numIndices]

		if (LMA_CHUNK_SKINNINGINFO):
			int	nodeNumber (node number this data belongs to (0 means the first node in the file, 1 means the second, etc.))
			for all vertices in the mesh of node 'nodeNumber'
				char numInfluences (so 1 byte)
					LMA_SkinInfluence[numInfluences]

		if (LMA_CHUNK_MOTIONPART):
			LMA_MotionPart

		if (LMA_CHUNK_ANIM):
			LMA_Anim (belongs to the last read LMA_MotionPart)
				LMA_Vector3Key[anim.numKeys] or LMA_QuaternionKey[anim.numKeys] (depending on animation type (pos/scale=Vector3Key  rot=QuaternionKey)

		if (LMA_CHUNK_COLLISIONMESH):
			LMA_CollisionMesh (belongs to the last read LMA_Node)
				LMA_Vector3[colMesh.numVertices]
				int[colMesh.numFaces*3]

		if (LMA_CHUNK_MATERIAL):
			LMA_Material

		if (LMA_CHUNK_MATERIALLAYER):
			LMA_MaterialLayer (belongs to a given material number 'layer.materialNumber', where 0 means the first material in the file, etc.

		if (LMA_CHUNK_FXMATERIAL)
			LMA_FXMaterial
				LMA_FXIntParameter    [fxMaterial.mNumIntParams]
				LMA_FXFloatParameter  [fxMaterial.mNumFloatParams]
				LMA_FXColorParameter  [fxMaterial.mNumColorParams]
				LMA_FXBitmapParameter [fxMaterial.mNumBitmapParams]

		if (LMA_CHUNK_SUBMESH):
			LMA_SubMesh
				LMA_SubMeshVertex[subMesh.numVerts]
				int[subMesh.numIndices]

		if (LMA_CHUNK_LIMIT)
			LMA_Limit

		if (LMA_CHUNK_PHYSICINFO)
			LMA_PhysicInfo

		if (LMA_CHUNK_BONESEXPRESSIONPART)
			LMA_BonesExpressionPart
				LMA_FacialTransformation[bonesExpPart.mNumTransformations]

		if (LMA_CHUNK_MESHEXPRESSIONPART)
			LMA_MeshExpressionPart
				LMA_FacialDeformData[meshExpressionPart.mNumDeformDatas]
				LMA_FacialTransformation[meshExpPart.mNumTransformations]
	}

	//-----------------------------
	Facial Motions:
	//-----------------------------

	LMF_Header

	while (not end of file)
	{
		if (LMA_CHUNK_EXPRESSIONMOTIONPART):
			LMA_UnsignedShortKey[expressionMotionPart.numKeys]

		if (LMA_CHUNK_PHONEMEMOTIONDATA):
			phonemeName(char[40])[phonemeMotionData.mNumPhonemes]
			LMA_PhonemeKey[phonemeMotionData.numKeys]
	}
*/


// version info
#define LMA_HIGHVERSION 1
#define LMA_LOWVERSION  1


// chunk IDs
enum
{
	LMA_CHUNK_NODE					= 0,	// a node (LMA_Node) coming up next
	LMA_CHUNK_MOTIONPART			= 1,	// a motion part (LMA_MotionPart) coming up next
	LMA_CHUNK_ANIM					= 2,	// an animation (LMA_Anim) coming up next
	LMA_CHUNK_MESH					= 3,	// a mesh (LMA_Mesh) coming up next
	LMA_CHUNK_SKINNINGINFO			= 4,	// skinning information (LMA_SkinInfluence)
	LMA_CHUNK_COLLISIONMESH			= 5,	// a collision mesh
	LMA_CHUNK_MATERIAL				= 6,	// a material (LMA_Material)
	LMA_CHUNK_MATERIALLAYER			= 7,	// a material layer (LMA_MaterialLayer)
	LMA_CHUNK_LIMIT					= 8,	// a node limit information
	LMA_CHUNK_PHYSICSINFO			= 9,	// physic information
	LMA_CHUNK_MESHEXPRESSIONPART	= 10,	// a mesh expression part
	LMA_CHUNK_EXPRESSIONMOTIONPART	= 11,	// a expression motion part
	LMA_CHUNK_PHONEMEMOTIONDATA		= 12,	// list of phonemes and keyframe data
	LMA_CHUNK_FXMATERIAL			= 13	// a FX material
};


// material layer map types
enum
{
	LMA_LAYERID_AMBIENT		= 0,	// ambient layer
	LMA_LAYERID_DIFFUSE		= 1,	// a diffuse layer
	LMA_LAYERID_SPECULAR	= 2,	// specular layer
	LMA_LAYERID_BUMP		= 3,	// bump layer
	LMA_LAYERID_REFLECT		= 4,	// reflection layer
	LMA_LAYERID_REFRACT		= 5,	// refraction layer
	LMA_LAYERID_OPACITY		= 6,	// opacity layer
	LMA_LAYERID_SELFILLUM	= 7,	// self illumination layer
	LMA_LAYERID_ENVIRONMENT = 8, 	// environment map layer
	LMA_LAYERID_SHINE		= 9,	// shininess (for specular)
	LMA_LAYERID_SHINESTRENGTH = 10,	// shine strength (for specular)
	LMA_LAYERID_FILTERCOLOR = 11	// filter color layer
};


// a chunk
struct LMA_Chunk
{
	int		mChunkID;				// the chunk ID
	int		mSizeInBytes;			// the size in bytes of this chunk (excluding this chunk struct)
	int		mVersion;				// the version of the chunk
};


// the old chunk header
struct LMA_ChunkOld
{
	char	mChunkID;				// the chunk ID
	int		mSizeInBytes;			// the size in bytes of this chunk (excluding this chunk struct)
};


// color
struct LMA_ColorRGB
{
	float mR, mG, mB;				// rgb
};


// a 3D vector
struct LMA_Vector3
{
	float	mX;						// x+ = to the right
	float	mY;						// y+ = up
	float	mZ;						// z+ = forwards (into the depth)
};


// a compressed 3D vector
struct LMA_16BitVector3
{
	unsigned short	mX;						// x+ = to the right
	unsigned short	mY;						// y+ = up
	unsigned short	mZ;						// z+ = forwards (into the depth)
};


// a compressed 3D vector
struct LMA_8BitVector3
{
	unsigned char	mX;						// x+ = to the right
	unsigned char	mY;						// y+ = up
	unsigned char	mZ;						// z+ = forwards (into the depth)
};



// a quaternion
struct LMA_Quaternion
{
	float	mX;						// Quaternion values
	float	mY;						//
	float	mZ;						//
	float	mW;						//
};


// a 4x3 matrix
struct LMA_Matrix
{
	float	m[16];					// 4x4 matrix data (m[column + row*4] = value)
};


// the main header
struct LMA_Header
{
	char	mFourcc[4];				// 'LMA '
	char	mHiVersion;				// high version (2  in case of v2.34)
	char	mLoVersion;				// low version  (34 in case of v2.34)
	bool	mActor;					// is this an actor? (if false, it's a motion)

	LMA_Header()	{ mFourcc[0]='L'; mFourcc[1]='M'; mFourcc[2]='A'; mFourcc[3]=' '; }
};


// the facial animation header
struct LMF_Header
{
	char	mFourcc[4];				// 'LMF '
	char	mHiVersion;				// high version (2  in case of v2.34)
	char	mLoVersion;				// low version  (34 in case of v2.34)

	LMF_Header()	{ mFourcc[0]='L'; mFourcc[1]='M'; mFourcc[2]='F'; mFourcc[3]=' '; }
};


// a node header
struct LMA_Node
{
	char			mName[40];		// the unique name of the node
	char			mParent[40];	// the name of the parent '\0' in case of a root node (node without parent)
	LMA_Vector3		mLocalPos;		// the local translation (before hierarchy)
	LMA_Quaternion	mLocalQuat;		// the local rotation (before hierarchy)
	LMA_Vector3		mLocalScale;	// the local scale (before hierarchy)
	LMA_Matrix		mInvBoneTM;		// inverse bone tm (after hierarchy) only used for nodes which are bones
};


// a motion node
struct LMA_MotionPart
{
	char			mName[40];		// the name of this motion part
	LMA_Vector3		mPosePos;		// initial pose position
	LMA_Quaternion	mPoseRot;		// initial pose rotation
	LMA_Vector3		mPoseScale;		// initial pose scale
};


// a facial expression motion part
struct LMA_ExpressionMotionPart
{
	char			mName[40];		// the name of this motion part
	int				mNrKeys;		// number of keys to follow
};


// a facial phoneme motion data
struct LMA_PhonemeMotionData
{
	int				mNumPhonemes;	// number of phonemes
	int				mNrKeys;		// number of keys to follow
};


// a float key
struct LMA_FloatKey
{
	float			mTime;			// the time in seconds
	float			mValue;			// the value			
};


// a unsigned short key
struct LMA_UnsignedShortKey
{
	float			mTime;			// the time in seconds
	unsigned short	mValue;			// the value			
};


// a 3D vector key
struct LMA_Vector3Key
{
	float			mTime;			// the time in seconds
	LMA_Vector3		mValue;			// the value
};


// a quaternion key
struct LMA_QuaternionKey
{
	float			mTime;			// the time in seconds
	LMA_Quaternion	mValue;			// the value
};


// TCB key
struct LMA_Vector3KeyTCB
{
	LMA_Vector3		mValue;
	float			mTime;
	float			mTension;
	float			mContinuity;
	float			mBias;
	float			mEaseIn;
	float			mEaseOut;
};


// a phoneme key
struct LMA_PhonemeKey
{
	float			mTime;			// the time in seconds
	char			mPhonemeNumber;	// the number of the phoneme as they are stored in the facial motion
	unsigned char	mPowerValue;	// the power value of the phoneme
};


// animation header
struct LMA_Anim
{
	int				mNrKeys;		// number of keys to follow
	char			mIPType;		// interpolation type		('L'=Linear     'B'=Bezier      'T'=TCB)
	char			mAnimType;		// animation type to follow ('P'=Position   'R'=Rotation    'S'=Scaling)
};


// uv (texture) coordinate
struct LMA_UV
{
	float	mU;						// u
	float	mV;						// v
};


// a mesh
struct LMA_Mesh
{
	int	mNodeNumber;	// the node number this mesh belongs to (0 means the first node in the file, 1 means the second, etc.)
	int mNumOrgVerts;	// number of original vertices
	int mTotalVerts;	// total number of vertices (of all submeshes)
	int mTotalIndices;	// total number of indices (of all submeshes)
	int	mNumSubMeshes;	// number of submeshes to follow
};


// a mesh
struct LMA_Mesh2
{
	int		mNodeNumber;		// the node number this mesh belongs to (0 means the first node in the file, 1 means the second, etc.)
	int		mNumOrgVerts;		// number of original vertices
	int		mTotalVerts;		// total number of vertices (of all submeshes)
	int		mTotalIndices;		// total number of indices (of all submeshes)
	int		mNumSubMeshes;		// number of submeshes to follow
	bool	mIsCollisionMesh;	// is this mesh a collision mesh or a normal mesh?
};


// a mesh
struct LMA_Mesh3
{
	int		mNodeNumber;		// the node number this mesh belongs to (0 means the first node in the file, 1 means the second, etc.)
	int		mNumOrgVerts;		// number of original vertices
	int		mTotalVerts;		// total number of vertices (of all submeshes)
	int		mTotalIndices;		// total number of indices (of all submeshes)
	int		mNumSubMeshes;		// number of submeshes to follow
	int		mNumUVSets;			// the number of UV sets (of all vertices inside this mesh)
	bool	mIsCollisionMesh;	// is this mesh a collision mesh or a normal mesh?
};



// an influence
struct LMA_SkinInfluence
{
	unsigned short	mNodeNr;
	float			mWeight;
};


struct LMA_CollisionMesh
{
	int		mNumVertices;
	int		mNumFaces;

	// followed by:
	// mNumVertices * sizeof(LMA_MCore::Vector3)
	// mNumFaces * sizeof(int) * 3
};


// material
struct LMA_Material
{
	char			mName[40];			// material name
	LMA_ColorRGB	mAmbient;			// ambient color
	LMA_ColorRGB	mDiffuse;			// diffuse color
	LMA_ColorRGB	mSpecular;			// specular color
	LMA_ColorRGB	mEmissive;			// self illumination color
	float			mShine;				// shine
	float			mShineStrength;		// shine strength
	float			mOpacity;			// the opacity amount [1.0=full opac, 0.0=full transparent]
	float			mIOR;				// index of refraction
	bool			mDoubleSided;		// double sided?
	bool			mWireFrame;			// render in wireframe?
	char			mTransparencyType;	// F=filter / S=substractive / A=additive / U=unknown
	float			mUOffset;			// u offset (horizontal texture shift)
	float			mVOffset;			// v offset (vertical texture shift)
	float			mUTiling;			// horizontal tiling factor
	float			mVTiling;			// vertical tiling factor
	float			mRotationRadians;	// texture rotation in radians
};


// a material layer
struct LMA_MaterialLayer
{
	char			mTexture[40];		// the texture filename
	char			mMapType;			// the map type (see enum in somewhere near the top of file)
	unsigned short	mMaterialNumber;	// the parent material number (as read from the file, where 0 means the first material)
	float			mAmount;			// the amount, between 0 and 1
};


// material
struct LMA_Material2
{
	char			mName[40];			// material name
	LMA_ColorRGB	mAmbient;			// ambient color
	LMA_ColorRGB	mDiffuse;			// diffuse color
	LMA_ColorRGB	mSpecular;			// specular color
	LMA_ColorRGB	mEmissive;			// self illumination color
	float			mShine;				// shine
	float			mShineStrength;		// shine strength
	float			mOpacity;			// the opacity amount [1.0=full opac, 0.0=full transparent]
	float			mIOR;				// index of refraction
	bool			mDoubleSided;		// double sided?
	bool			mWireFrame;			// render in wireframe?
	char			mTransparencyType;	// F=filter / S=substractive / A=additive / U=unknown
};


// a material layer
struct LMA_MaterialLayer2
{
	char			mTexture[40];		// the texture filename
	char			mMapType;			// the map type (see enum in somewhere near the top of file)
	unsigned short	mMaterialNumber;	// the parent material number (as read from the file, where 0 means the first material)
	float			mAmount;			// the amount, between 0 and 1
	float			mUOffset;			// u offset (horizontal texture shift)
	float			mVOffset;			// v offset (vertical texture shift)
	float			mUTiling;			// horizontal tiling factor
	float			mVTiling;			// vertical tiling factor
	float			mRotationRadians;	// texture rotation in radians
};


// material (shader supported)
struct LMA_Material3
{
	char			mName[40];			// material name
	char			mShaderFile[40];	// shader filename
	LMA_ColorRGB	mAmbient;			// ambient color
	LMA_ColorRGB	mDiffuse;			// diffuse color
	LMA_ColorRGB	mSpecular;			// specular color
	LMA_ColorRGB	mEmissive;			// self illumination color
	float			mShine;				// shine
	float			mShineStrength;		// shine strength
	float			mOpacity;			// the opacity amount [1.0=full opac, 0.0=full transparent]
	float			mIOR;				// index of refraction
	bool			mDoubleSided;		// double sided?
	bool			mWireFrame;			// render in wireframe?
	char			mTransparencyType;	// F=filter / S=substractive / A=additive / U=unknown
};


// a sub mesh vertex
struct LMA_SubMeshVertex
{
	LMA_Vector3		mPos;				// the position
	LMA_Vector3		mNormal;			// the normal
	LMA_UV			mUV;				// the uv
	int				mOrgVtx;			// the original vertex (points into the mesh)
};



// a sub mesh vertex
struct LMA_SubMeshVertex2
{
	int				mOrgVtx;			// the original vertex (points into the mesh)
	LMA_Vector3		mPos;				// the position
	LMA_Vector3		mNormal;			// the normal

	// followed by:
	// LMA_UV[parentMesh.mNumUVSets]
};



// a sub mesh
struct LMA_SubMesh
{
	char			mMatID;				// material number, indexes into the file, so 0 means the first read material
	char			mNumUVSets;			// number of UV sets
	int				mNumIndices;		// number of indices
	int				mNumVerts;			// number of vertices
};


// node limit information
struct LMA_Limit
{
	int				mNodeNumber;		// the node number where this info belongs to
	LMA_Vector3		mTranslationMin;	// the minimum translation values
	LMA_Vector3		mTranslationMax;	// the maximum translation value.
	LMA_Vector3		mRotationMin;		// the minimum rotation values
	LMA_Vector3		mRotationMax;		// the maximum rotation values
	LMA_Vector3		mScaleMin;			// the minimum scale values
	LMA_Vector3		mScaleMax;			// the maximum scale values
	bool			mLimitFlags[9];		// the limit type activation flags
};


// node physics information
struct LMA_PhysicsInfo
{
	char			mName[40];				// the unique name of the expression part
	float			mSphereRadius;		// radius of the sphere
	float			mCylinderRadius;	// radius of the cylinder
	float			mCylinderHeight;	// height of the cylinder
	float			mBoxDepth;			// depth of the box
	float			mBoxHeight;			// height of the box
	float			mBoxWidth;			// width of the box
	float			mMass;				// the mass of the node
	int				mPhysicsObjectType; // 0=box, 1=sphere, 2=cylinder
};


// a facial transformation used by bones expression parts
struct LMA_FacialTransformation
{
	char			mNodeName[40];			// the name of the node to which this transformation belongs to
	LMA_Vector3		mPosition;				// the node delta position
	LMA_Vector3		mScale;					// the node delta scale
	LMA_Quaternion	mRotation;				// the node rotation
};


// a mesh expression part
struct LMA_MeshExpressionPart
{
	char			mName[40];				// the unique name of the expression part
	float			mRangeMin;				// the slider min
	float			mRangeMax;				// the slider max
	int				mLOD;					// the level of detail to which this expression part belongs to
	char			mPhonemeChar;			// the char to which the expression belongs to, if this expression part isn't a phoneme set it to 0
	int				mNumDeformDatas;		// deform data for the facial mesh expression parts
	int				mNumTransformations;	// the number of transformations
	int				mNumPhonemeSets;		// the number of phoneme sets to follow

	// followed by:
	// LMA_FacialDeformData[ mNumDeformDatas ]
	// LMA_FacialTransformations[ mNumTransformations ]
	// int [ mNumPhonemeSets ]				(the phoneme sets array)
};



// a mesh expression part
struct LMA_MeshExpressionPart2
{
	char			mName[40];				// the unique name of the expression part
	float			mRangeMin;				// the slider min
	float			mRangeMax;				// the slider max
	int				mLOD;					// the level of detail to which this expression part belongs to
	char			mPhonemeChar;			// the char to which the expression belongs to, if this expression part isn't a phoneme set it to 0
	int				mNumDeformDatas;		// deform data for the facial mesh expression parts
	int				mNumTransformations;	// the number of transformations
	int				mNumPhonemeSets;		// the number of phoneme sets to follow

	// followed by:
	// LMA_FacialDeformData2[ mNumDeformDatas ]
	// LMA_FacialTransformations[ mNumTransformations ]
	// int [ mNumPhonemeSets ]				(the phoneme sets array)
};



// facial deformation data
struct LMA_FacialDeformData
{
	char			mNodeName[40];			// the node name, which is the node which has the mesh we will deform
	int				mNumVertices;			// the number of vertices

	// followed by:
	// LMA_Vector3[ mNumVertices ]   (delta position values)
	// LMA_Vector3[ mNumVertices ]	 (delta normal values)
	// int[ mNumVertices ]			 (vertex numbers)
};

///////////////////////////////////////////////////////////////////////////////
// a sub mesh vertex

// imc 확장 버전을 위한 구조체 (버전 10000부터 시작한다.)
// a mesh

// Desc : 모체는 LMA_Mesh3
struct LMA_Mesh10000
{
	int		mNodeNumber;		// the node number this mesh belongs to (0 means the first node in the file, 1 means the second, etc.)
	int		mNumOrgVerts;		// number of original vertices
	int		mTotalVerts;		// total number of vertices (of all submeshes)
	int		mTotalIndices;		// total number of indices (of all submeshes)
	int		mNumSubMeshes;		// number of submeshes to follow
	bool	mIsCollisionMesh;	// is this mesh a collision mesh or a normal mesh?
	char    mShdMapName[40];
	BYTE    mReserved[217];  
};

// Desc : 모체는 LMA_SubMeshVertex
struct LMA_SubMeshVertex10000
{
	LMA_Vector3		mPos;				// the position
	LMA_Vector3		mNormal;			// the normal
	LMA_UV			mUV0;				// the uv
	LMA_UV			mUV1;				// the uv
	int				mOrgVtx;			// the original vertex (points into the mesh)
};

// material
struct LMA_Material10000
{
	char			mName[40];			// material name
	LMA_ColorRGB	mAmbient;			// ambient color
	LMA_ColorRGB	mDiffuse;			// diffuse color
	LMA_ColorRGB	mSpecular;			// specular color
	LMA_ColorRGB	mEmissive;			// self illumination color
	float			mShine;				// shine
	float			mShineStrength;		// shine strength
	float			mOpacity;			// the opacity amount [1.0=full opac, 0.0=full transparent]
	float			mIOR;				// index of refraction
	bool			mDoubleSided;		// double sided?
	bool			mWireFrame;			// render in wireframe?
	char			mTransparencyType;	// F=filter / S=substractive / A=additive / U=unknown

	DWORD           mShaderMask;
	INT             mAlphaRef;
	INT             mSrcBlend;
	INT             mDstBlend;
	DWORD           mTexFxn;
	FLOAT           mTexFxnUParm0, mTexFxnVParm0, mTexFxnSub0Parm0;
	BYTE            mReserved[240];
};

// facial deformation data
struct LMA_FacialDeformData2
{
	char			mNodeName[40];			// the node name, which is the node which has the mesh we will deform
	int				mNumVertices;			// the number of vertices
	float			mMinValue;				// minimum range value for x y and z components of the compressed position vectors
	float			mMaxValue;				// maximum range value for x y and z components of the compressed position vectors

	// followed by:
	// LMA_16BitVector3[ mNumVertices ]  (delta position values)
	// LMA_8BitVector3[ mNumVertices ]	 (delta normal values)
	// int[ mNumVertices ]				 (vertex numbers)
};


struct LMA_FXMaterial
{
	char	mName[40];
	char	mEffectFile[40];	// path excluded, extension included
	int		mNumIntParams;
	int		mNumFloatParams;
	int		mNumColorParams;
	int		mNumBitmapParams;

	// followed by:
	// LMA_FXIntParameter   [ mNumIntParams ]
	// LMA_FXFloatParameter [ mNumFloatParams ]
	// LMA_FXColorParameter [ mNumColorParams ]
	// LMA_FXBitmapParameter[ mNumBitmapParams ]
};


struct LMA_FXIntParameter
{
	char	mName[40];
	int		mValue;
};


struct LMA_FXFloatParameter
{
	char	mName[40];
	float	mValue;
};


struct LMA_FXColorParameter
{
	char	mName[40];
	float	mR;
	float	mG;
	float	mB;
	float	mA;
};

struct LMA_FXBitmapParameter
{
	char	mName[40];
	char	mFilename[40];	// path excluded, extension included
};





#endif