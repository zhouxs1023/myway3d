/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __BASICNORMALMAPGENERATOR_H
#define __BASICNORMALMAPGENERATOR_H

// include Core
#include "../Core/Source/MCore.h"

// include the base class header
#include "NormalMapGenerator.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class SubMesh;

/**
 * The basic normal map generator.
 * This normal map generator works like 99.9% of all other normal map generators.
 * It uses the initial texture mapping uv channel to convert each texel into world space.
 * Then it calculates the normal at that world space position, and shoots a ray from the world space position
 * into the direction of the normal, until it hits a triangle inside the high detail model.
 * After that it calculates the normal at the intersection point, which is then encoded inside the normal map image.
 * The restriction however is that all texture coordinates must be in range of 0..1. So no wrapping can be used.
 * The reason for this is that otherwise the same texel can have multiple world space coordinates, which makes it
 * impossible to use this technique.
 */
class BasicNormalMapGenerator : public NormalMapGenerator
{
	MEMORYOBJECTCATEGORY(BasicNormalMapGenerator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NORMALMAPGENERATOR);

	public:
		// the basic normal map generator type ID number
		enum { TYPE_ID = 0x00000001 };

		/**
		 * The constructor.
		 */
		BasicNormalMapGenerator();

		/**
		 * The destructor.
		 */
		~BasicNormalMapGenerator();

		/**
		 * Get the unique type identification number of this generator.
		 * @result The unique type ID.
		 */
		int GetType() const;

		/**
		 * Get the description of this generator.
		 * @result A string containing the description.
		 */
		const char* GetTypeString();

		/**
		 * Get the progress state, which means how far the generation process is completed.
		 * This is represented in a percentage ranging from 0 to 100.
		 * @result The progress state, as a value ranging from 0 to 100.
		 */
		float GetProgress() const;

		/**
		 * Get the progress state for the current normal map part. A normal map can be generated in several steps.
		 * It might be that a normal map will be generated in 4 steps. That means the normal map is basically divided into
		 * four different parts. This method returns the progress state of the current part generation process.
		 * This is expressed in a percentage, in range of [0..100].
		 * @return Part progress state, as a percentage, ranging from 0 to 100.
		 */
		float GetPartProgress() const;

		/**
		 * The main method, which generates a set of normalmaps from a low and high detail model.
		 * Both actors will not be deleted from memory after execution of this method.
		 * @param lowDetail The low detail version of the actor.
		 * @param highDetail The high detail version of the actor.
		 * @param tangentSpace When set to true, the normal map will be generated in tangent space, otherwise in object space.
		 * @param postFilter When set to true, a post filter process will be applied after the normal map has been generated.
		 *                   This post filter will try to increase the quality of the normal map by removing potential noticable errors and
		 *                   applying some filtering in areas where the algorithm thinks it is needed.
		 */
		void Generate(Actor* lowDetail, Actor* highDetail, bool tangentSpace, bool postFilter);

		/**
		 * Set a pointer to the image resolution checker to use.
		 * On default this is the ImageResChecker base class itself, which supports resolution extraction of BMP, TGA, PNG, JPG and PSD files.
		 * @param checker The resolution checker to use. When setting a new one, the existing one will automatically be deleted.
		 */
		void SetResChecker(ImageResChecker* checker);


	private:
		ImageResChecker*	mResChecker;		/**< The image resolution checker, used to extract original texture resolutions, which are used to calc the normal map resolutions. */
		float				mPartProgress;		/**< The current progress state of the current generated normal map part. */
		float				mTotalProgress;		/**< The total progress state, of all normal maps. */
		int					mTotalFaces;		/**< The total number of faces being processed. */

		/**
		 * A part of the normal map which needs to be generated.
		 */
		struct NormalMapPart
		{
			MCore::String	mName;
			MCore::String	mDiffuseName;
			Node*			mNode;
			SubMesh*		mSubMesh;
			Mesh*			mMesh;
			NormalMap*		mNormalMap;
			int				mMaterialNr;
		};

		/**
		 * Process a given face.
		 * @param pos The three vertex positions of the triangle.
		 * @param uvs The three texture coordinates of the triangle.
		 * @param normals The three vertex normals of the triangle.
		 * @param tangents The three vertex tangents of the triangle.
		 * @param normalMap The normalmap to write the results in.
		 * @param highDetail The high detail actor.
		 * @param node The node where the face belongs to.
		 * @param tangentSpace When set to true, the result will be in tangent space, otherwise in object space.
		 * @param postFilter When set to true, post-filtering will be applied.
		 */
		void ProcessFace(MCore::DVector3 pos[3], MCore::DVector2 uv[3], MCore::DVector3 normals[3], MCore::DVector4 tangents[3], NormalMap* normalMap, Actor* highDetail, Node* node, bool tangentSpace, bool postFilter);

		/**
		 * Make a given sample, for a given pixel in the normal map.
		 * @param ray The ray being traced, from the low detail model, through the high detail model.
		 * @param highDetail The high detail actor.
		 * @param normalMap The normal map to write the resulting normal in.
		 * @param x The x coordinate in the normal map to write the result in.
		 * @param y The y coordinate in the normal map to write the result in.
		 * @param node The node to perform the intersection tests on.
		 * @param tangent The interpolated tangent on the low detail model, used to calculate the tangent space matrix, to convert the result into tangent space.
		 * @param tangentSpace When set to true, the result will be in tangent space, otherwise in object space.
		 * @param postFilter When set to true, post-filtering will be applied.
		 */
		void Sample(const MCore::DRay& ray, Actor* highDetail, NormalMap* normalMap, const int x, const int y, Node* node, MCore::DVector3& normal, MCore::DVector4& tangent, bool tangentSpace, bool postFilter);

		/**
		 * Calculates the closest intersection between a ray and a node, if there is any.
		 * @param ray The ray to trace.
		 * @param node The node to perform the intersection tests on.
		 * @param outPos A pointer to a vector to which the function will write the intersection point, in case there is an intersection.
		 * @param outBaryU A pointer to a double to which the function will write the barycentric U coordinate.
		 * @param outBaryV A pointer to a double to which the function will write the barycentric V coordinate.
		 * @param outStartIndex A pointer to an integer to which the function will write the start index number in the index array of the node's mesh.
		 * @result Returns true in case there is an intersection, or false when there is not.
		 */
		bool CalcIntersection(const MCore::DRay& ray, Node* node, MCore::DVector3* outPos, double* outBaryU, double* outBaryV, int* outStartIndex);
};


} // namespace EMotionFX


#endif