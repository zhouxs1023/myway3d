/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NORMALMAP_H
#define __NORMALMAP_H

#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class NormalMapGenerator;

/**
 * The normal map class.
 * Normal maps are maps containing normal information. Basically they are the same as texture maps, however
 * instead of encoding color information, they contain normal information. These normal maps can be used in per pixel
 * lighting techniques. A normal map is generated from a high and low detail version of the model.
 * The high detail version can have a huge amount of triangles and vertices. The low detail model will be the ingame model.
 * Now using some techniques, the the normals on the high detail model are sampled, and stored as normal maps.
 * Using per pixel lighting this allows us to make the low detail model look like the high detailed model.
 * This can increase realism a lot.
 */
class NormalMap
{
	MEMORYOBJECTCATEGORY(NormalMap, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NORMALMAPGENERATOR);
	friend class NormalMapGenerator;

	public:
		/**
		 * A pixel inside the normal map.
		 * It contains more than just an RGB value, for the reason that it needs some extra information for the 
		 * EDT algorithm which is responsible for filling empty pixels to prevent bilinear filtering problems.
		 */
		struct Pixel
		{
			int		x;		/**< The x position of the pixel color to use for this pixel (used by EDT algorithm). */
			int		y;		/**< The y position of the pixel color to use for this pixel (used by EDT algorithm). */
			float	dist;	/**< The distance to the closest neighbor pixel (used by EDT algorithm). */
			int		r;		/**< Red. */
			int		g;		/**< Green. */
			int		b;		/**< Blue. */
			bool	isFree;	/**< Is this pixel free? (or empty) */
		};

		/**
		 * The constructor.
		 * @param width The width in pixels (x-resolution of the normal map).
		 * @param height The height in pixels (y-resolution of the normal map).
		 * @param name The name of the normal map.
		 */
		NormalMap(const int width, const int height, MCore::String& name);

		/**
		 * Destructor.
		 * Automatically gets rid of the allocated data.
		 */
		~NormalMap();

		/**
		* Allocate pixel data for the normal map.
		* @param width The width in pixels.
		* @param height The height in pixels.
		*/
		void Alloc(const int width, const int height);

		/**
		 * Get access to the pixel data.
		 * @result Returns a pointer to the pixel data.
		 */
		inline Pixel* GetPixelData();

		/**
		 * Get the width of the normal map image in pixels.
		 * @result The width, expressed in number of pixels.
		 */
		inline int GetWidth() const;

		/**
		 * Get the height of the normal map image in pixels.
		 * @result The height, expressed in number of pixels.
		 */
		inline int GetHeight() const;

		/**
		 * Check if x and y are in the range of the image's resolution.
		 * @param x The value to check with the image's width.
		 * @param y The value to check with the image's height.
		 * @return True if x and y are in range of the resolution, otherwise false.
		 */
		inline bool InRange(const int x, const int y);

		/**
		 * Check if the given pixel is already being processed or not.
		 * @param x The x position of the pixel.
		 * @param y The y position of the pixel.
		 * @param True if the pixel is free, so not used, false if not.
		 */
		inline bool IsFree(const int x, const int y);

		/**
		 * Get the name of the normal map.
		 * @return The name of the normal map.
		 */
		inline const MCore::String& GetName() const;

		/**
		 * Convert a normal into a colored pixel at a given position.
		 * @param x The x position of the pixel, in the normal map.
		 * @param y The y position of the pixel, in the normal map.
		 * @param normal The normal to write into the normal map.
		 * @param setUsed When set to true, the pixel at (x, y) will be set to be used. This means that the function
		 *                IsFree(x, y) will return false when this parameter is set to true.
	  	 */
		void WriteNormal(const int x, const int y, MCore::DVector3& normal, bool setUsed);

		/**
		 * Convert a given pixel in the normalmap to a normal.
		 * @param x The x coordinate in the normal map image.
		 * @param y The y coordinate in the normal map image.
		 * @result The converted normal.
		 */
		MCore::DVector3 GetNormal(const int x, const int y) const;

		/**
		 * Encode a given normal into the normal map. The difference between WriteNormal and EncodeNormal is that
		 * this method (EncodeNormal) will write 'thick' pixels in order to prevent bilinear filtering problems.
		 * @param x X Coordinate of the pixel.
		 * @param y Y Coordinate of the pixel.
		 * @param normal The normal to convert.
		 */
		void EncodeNormal(const int x, const int y, MCore::DVector3& normal);

		/**
		 * Save the normal map as an uncompressed 24 bits BMP file.
		 * @param filename The file name of the bitmap file.
		 * @return True if the bitmap has been written successfully, false if not.
		 */
		bool SaveAsBMP(const MCore::String& filename);

		/**
		 * Fix errors in the normal map.
		 * This method will be executed when the postFilter parameter in the NormalMapGenerator::Generate(...) method is set to true.
		 */
		void FixErrors();

		/**
		 * Fill the empty pixels to prevent bilinear filtering problems.
		 * The algorithm used is called EDT-8.
		 */	 
		void FillEmptyPixels();

		/**
		 * Apply a simple blur, by averaging all the neighboring used pixels.
		 * Please note that after blurring you should re-normalize the image again.
		 * This can be done using the Normalize() method.
		 */
		void Blur();

		/**
		 * Normalize the normals in the image.
		 */
		void Normalize();

		/**
		 * Convert the values in the normal map to a valid bumpmap, by making the values in valid ranges [0..255].
		 */
		void FixBump();


	private:
		MCore::String	mName;		/**< The name of the normal map, used to construct the filename. */
		Pixel*			mPixels;	/**< The pixel data. */
		int				mWidth;		/**< The width of the normal map image, in pixels. */
		int				mHeight;	/**< The height of the normal map image, in pixels. */

		/// neighbor check used by the EDT algorithm.
		void CheckNeighbor(int x, int y, Pixel* pixel, Pixel* neighbor);
};

// include inline code
#include "NormalMap.inl"

} // namespace EMotionFX


#endif