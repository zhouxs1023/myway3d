/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LMA_CHUNKPROCESSORS_H
#define __LMA_CHUNKPROCESSORS_H

// include the Core system
#include "../../Core/Source/MCore.h"


namespace EMotionFX
{

// forward declarations
class Node;
class Actor;
class Motion;
class FacialMotion;
class MotionPart;
class Importer;

// include the file format structs
#include "FileFormatStructs.inl"


/**
 * The chunk processor base class.
 * Chunk processors read in a specific chunk, convert them into EMotion FX objects and apply them to either actors
 * or motions. Chunk processors have got a version number, so that there is the possibility of having several
 * processor implementations with different version numbers for one type of chunk. This gives us backward compatibility.
 * Logging can be actived or deactived by functions provided by the base class.
 */
class ChunkProcessor
{
	DECLARE_CLASS(ChunkProcessor)

	public:
		/**
		 * The constructor.
		 * @param lmaImporter A pointer to the LMA importer.
		 * @param chunkID The ID of the chunk processor.
		 * @param version The version number of the chunk processor, so what version number of the given chunk
		 *                this processor can read and process.
		 */
		ChunkProcessor(Importer* lmaImporter, const int chunkID, const int version);

		/**
		 * Destructor.
		 */
		virtual ~ChunkProcessor();

		/**
		 * Read and process a chunk. This is the main method.
		 * It will return false when we have reached the end of the file or something bad has happened while reading.
		 * This is pure virtual method, which has to be overloaded by the processors inherited from this base class.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to edit using the data out of the chunk.
		 * @param motion The motion to edit using the data out of the chunk.
		 * @return False when we reached the end of the file, true if not.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting) = 0;

		/**
		 * Set the unique ID of the chunk processor.
		 * @param The id of the chunk processor.
		 */
		void SetChunkID(const int chunkID);

		/**
		 * Set the version number of the chunk processor.
		 * This represents the version number of the chunk we can read and process with this processor.
		 * @param The version number of the chunk processor.
		 */
		void SetVersion(const int versionNumber);

		/**
		 * Return the id of the chunk processor.
		 * @return The id of the chunk processor.
		 */
		int GetChunkID() const;

		/**
		 * Return the version number of the chunk processor.
		 * @return The version number of the chunk processor.
		 */
		int GetVersion() const;

		/**
		 * Set the log status.
		 * @param The log status. True if the chunk shall log events, false if not.
		 */
		void SetLogging(const bool loggingActive);

		/**
		 * Return the log status.
		 * @return The log status. True if the chunk shall log events, false if not.
		 */
		bool GetLogging() const;


	protected:
		int						mChunkID;		/**< The id of the chunk processor. */
		int						mVersion;		/**< The version number of the chunk processor, to provide backward compatibility. */
		bool					mLoggingActive;	/**< When set to true the processor chunk will log events, otherwise no logging will be performed. */
		Importer*				mLMAImporter;	/**< A pointer to the lma importer which is currently in use. */
};

//-------------------------------------------------------------------------------------------------

/**
 * The node processor reads in node chunks and convertes them into Node objects.
 */
class NodeChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(NodeChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		NodeChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~NodeChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);


	private:
		/**
		 * Create a node with parent link
		 */
		void AddLink(Node* node, char* parent);

		LMA_Node mNodeHeader;	/**< Precreated node which holds info about a node. */
};

//-------------------------------------------------------------------------------------------------

/**
 * The mesh processor reads in mesh chunks and converts them into Mesh objects.
 */
class MeshChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(MeshChunkProcessor1)

	public:
		/**
		 * Constructor.
		 * @param lmaImporter A pointer to the lma importer.
		 */
		MeshChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MeshChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};


//-------------------------------------------------------------------------------------------------

/**
 * The mesh processor reads in mesh chunks and converts them into Mesh objects.
 */
class MeshChunkProcessor2 : public ChunkProcessor
{
	DECLARE_CLASS(MeshChunkProcessor2)

	public:
		/**
		 * Constructor.
		 * @param lmaImporter A pointer to the lma importer.
		 */
		MeshChunkProcessor2(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MeshChunkProcessor2();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The mesh processor reads in mesh chunks and converts them into Mesh objects.
 */
class MeshChunkProcessor3 : public ChunkProcessor
{
	DECLARE_CLASS(MeshChunkProcessor3)

	public:
		/**
		 * Constructor.
		 * @param lmaImporter A pointer to the lma importer.
		 */
		MeshChunkProcessor3(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MeshChunkProcessor3();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};



//-------------------------------------------------------------------------------------------------

/**
 * The mesh processor reads in mesh chunks and converts them into Mesh objects.
 */

// Desc : imc chunk processor
class MeshChunkProcessor10000 : public ChunkProcessor
{
	DECLARE_CLASS(MeshChunkProcessor10000)

	public:
		/**
		 * Constructor.
		 * @param lmaImporter A pointer to the lma importer.
		 */
		MeshChunkProcessor10000(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MeshChunkProcessor10000();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The skinning info processor reads in skinning info.
 */
class SkinningInfoChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(SkinningInfoChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		SkinningInfoChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~SkinningInfoChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The collision mesh processor reads in collision meshes.
 */
class CollisionMeshChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(CollisionMeshChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		CollisionMeshChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~CollisionMeshChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The material processor reads in materials.
 */
class MaterialChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(MaterialChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MaterialChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MaterialChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The material layer processor reads in material layers.
 */
class MaterialLayerChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(MaterialLayerChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MaterialLayerChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MaterialLayerChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The motion part processor reads in motion parts.
 */
class MotionPartChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(MotionPartChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MotionPartChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MotionPartChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);

	private:
		LMA_MotionPart mMotionPart; /**< Precreated motion part which holds animation data. */
};

//-------------------------------------------------------------------------------------------------

/**
 * The animation processor reads in motions.
 */
class AnimationChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(AnimationChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		AnimationChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~AnimationChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);

	private:
		LMA_Anim mAnim; /**< Precreated animation which contains animation data. */
};

//-------------------------------------------------------------------------------------------------

/**
 * The material processor reads in materials.
 */
class MaterialChunkProcessor2 : public ChunkProcessor
{
	DECLARE_CLASS(MaterialChunkProcessor2)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MaterialChunkProcessor2(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MaterialChunkProcessor2();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The material layer processor reads in material layers.
 */
class MaterialLayerChunkProcessor2 : public ChunkProcessor
{
	DECLARE_CLASS(MaterialLayerChunkProcessor2)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MaterialLayerChunkProcessor2(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MaterialLayerChunkProcessor2();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The material processor reads in materials.
 */
class MaterialChunkProcessor3 : public ChunkProcessor
{
	DECLARE_CLASS(MaterialChunkProcessor3)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MaterialChunkProcessor3(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MaterialChunkProcessor3();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};


//-------------------------------------------------------------------------------------------------
/**
 * The material processor reads in materials.
 */
class MaterialChunkProcessor10000 : public ChunkProcessor
{
	DECLARE_CLASS(MaterialChunkProcessor10000)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		MaterialChunkProcessor10000(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~MaterialChunkProcessor10000();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The limit processor reads in node limits.
 */
class LimitChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(LimitChunkProcessor1)

public:
	/**
	 * Constructor.
	 * @param lmaImporter A pointer to the lma importer.
	 */
	LimitChunkProcessor1(Importer* lmaImporter);

	/**
	 * Destructor.
	 */
	virtual ~LimitChunkProcessor1();

	/**
	 * Reads and processes the chunk.
	 * @param file The file from which the processor reads out the chunk.
	 * @param actor The actor to modify, when importing an actor.
	 * @param motion The motion to modify, when importing a motion.
	 * @result Returns true when successfully completed, or false when some read error has occured.
	 */
	virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The physics info processor reads in node physics information.
 */
class PhysicsInfoChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(PhysicsInfoChunkProcessor1)

public:
	/**
	 * Constructor.
	 * @param lmaImporter A pointer to the lma importer.
	 */
	PhysicsInfoChunkProcessor1(Importer* lmaImporter);

	/**
	 * Destructor.
	 */
	virtual ~PhysicsInfoChunkProcessor1();

	/**
	 * Reads and processes the chunk.
	 * @param file The file from which the processor reads out the chunk.
	 * @param actor The actor to modify, when importing an actor.
	 * @param motion The motion to modify, when importing a motion.
	 * @result Returns true when successfully completed, or false when some read error has occured.
	 */
	virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The mesh expression part reader (MeshExpressionPart).
 */
class MeshExpressionPartChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(MeshExpressionPartChunkProcessor1)

public:
	/**
	 * Constructor.
	 * @param lmaImporter A pointer to the lma importer.
	 */
	MeshExpressionPartChunkProcessor1(Importer* lmaImporter);

	/**
	 * Destructor.
	 */
	virtual ~MeshExpressionPartChunkProcessor1();

	/**
	 * Reads and processes the chunk.
	 * @param file The file from which the processor reads out the chunk.
 	 * @param actor The actor to modify, when importing an actor.
	 * @param motion The motion to modify, when importing a motion.
	 * @result Returns true when successfully completed, or false when some read error has occured.
	 */
	virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};


//-------------------------------------------------------------------------------------------------

/**
 * The mesh expression part reader (MeshExpressionPart).
 */
class MeshExpressionPartChunkProcessor2 : public ChunkProcessor
{
	DECLARE_CLASS(MeshExpressionPartChunkProcessor2)

public:
	/**
	 * Constructor.
	 * @param lmaImporter A pointer to the lma importer.
	 */
	MeshExpressionPartChunkProcessor2(Importer* lmaImporter);

	/**
	 * Destructor.
	 */
	virtual ~MeshExpressionPartChunkProcessor2();

	/**
	 * Reads and processes the chunk.
	 * @param file The file from which the processor reads out the chunk.
 	 * @param actor The actor to modify, when importing an actor.
	 * @param motion The motion to modify, when importing a motion.
	 * @result Returns true when successfully completed, or false when some read error has occured.
	 */
	virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The facial expression motion part reader (ExpressionMotionPart).
 */
class ExpressionMotionPartChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(ExpressionMotionPartChunkProcessor1)

public:
	/**
	 * Constructor.
	 * @param lmaImporter A pointer to the lma importer.
	 */
	ExpressionMotionPartChunkProcessor1(Importer* lmaImporter);

	/**
	 * Destructor.
	 */
	virtual ~ExpressionMotionPartChunkProcessor1();

	/**
	 * Reads and processes the chunk.
	 * @param file The file from which the processor reads out the chunk.
 	 * @param actor The actor to modify, when importing an actor.
	 * @param motion The motion to modify, when importing a motion.
	 * @result Returns true when successfully completed, or false when some read error has occured.
	 */
	virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

//-------------------------------------------------------------------------------------------------

/**
 * The facial phoneme motion data reader (PhonemeMotionData).
 */
class PhonemeMotionDataChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(PhonemeMotionDataChunkProcessor1)

public:
	/**
	 * Constructor.
	 * @param lmaImporter A pointer to the lma importer.
	 */
	PhonemeMotionDataChunkProcessor1(Importer* lmaImporter);

	/**
	 * Destructor.
	 */
	virtual ~PhonemeMotionDataChunkProcessor1();

	/**
	 * Reads and processes the chunk.
	 * @param file The file from which the processor reads out the chunk.
 	 * @param actor The actor to modify, when importing an actor.
	 * @param motion The motion to modify, when importing a motion.
	 * @result Returns true when successfully completed, or false when some read error has occured.
	 */
	virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);

	/**
	 * Finds the maxtime and syncs all keytracks of the facial animation. Each keytrack which stops
	 * before maxtime gets an ending key with time=maxTime. So that all keytracks end at the same time.
	 * @param facialMotion The facial motion to sync.
	 */
	static void SyncMotionTrackEnds(FacialMotion* facialMotion);

	/**
	 * Finds the maximum time of the given facial animation.
	 * @param facialMotion The facial motion to check.
	 * @return The maximum time of the given facial animation.
	 */
	static float FindMaxTime(FacialMotion* facialMotion);
};


/**
 * The FX material processor reads in FX materials.
 */
class FXMaterialChunkProcessor1 : public ChunkProcessor
{
	DECLARE_CLASS(FXMaterialChunkProcessor1)

	public:
		/**
		 * Constructor.
 		 * @param lmaImporter A pointer to the lma importer.
		 */
		FXMaterialChunkProcessor1(Importer* lmaImporter);

		/**
		 * Destructor.
		 */
		virtual ~FXMaterialChunkProcessor1();

		/**
		 * Reads and processes the chunk.
		 * @param file The file from which the processor reads out the chunk.
		 * @param actor The actor to modify, when importing an actor.
		 * @param motion The motion to modify, when importing a motion.
		 * @result Returns true when successfully completed, or false when some read error has occured.
		 */
		virtual bool Process(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};


} // namespace EMotionFX


#endif