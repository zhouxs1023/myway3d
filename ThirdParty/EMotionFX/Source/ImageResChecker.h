/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __IMAGERESCHECKER_H
#define __IMAGERESCHECKER_H

#include "../Core/Source/MCore.h"


namespace EMotionFX
{

/**
 * The image resolution checker class extracts the width and height from an image file.
 * Simply call GetResoltuion(), it will automatically take the correct function
 * for the given image file.
 */
class ImageResChecker
{
	DECLARE_CLASS(ImageResChecker)

	public:
		/**
		 * Default constructor.
		 */
		ImageResChecker();

		/**
		 * Destructor.
		 */
		virtual ~ImageResChecker();

		/**
		 * Extract the resolution from an image file.
		 * @param filename The file name of the image file.
		 * @param outWidth Pointer to an integer to which the function will write the image's width.
		 * @param outHeight Pointer to an integer to which the function will write the image's height.
		 * @return True if the function has extracted the resolution successfully, false if not.
		 */
		virtual bool GetResolution(const MCore::String& filename, int* outWidth, int* outHeight);


	protected:
		/**
		 * Extract resolution from a BMP file.
		 * @param filename The filename of the image file.
		 * @param outWidth Pointer to an integer to which the function will write the image's width.
		 * @param outHeight Pointer to an integer to which the function will write the image's height.
		 * @return True if the function has extracted the resolution successfully, false if not.
		 */
		bool GetBMPRes(MCore::DiskFile& file, int* outWidth, int* outHeight);

		/**
		 * Extract resolution from a TGA file.
		 * @param filename The filename of the image file.
		 * @param outWidth Pointer to an integer to which the function will write the image's width.
		 * @param outHeight Pointer to an integer to which the function will write the image's height.
		 * @return True if the function has extracted the resolution successfully, false if not.
		 */
		bool GetTGARes(MCore::DiskFile& file, int* outWidth, int* outHeight);

		/**
		 * Extract resolution from a JPG file.
		 * @param filename The filename of the image file.
		 * @param outWidth Pointer to an integer to which the function will write the image's width.
		 * @param outHeight Pointer to an integer to which the function will write the image's height.
		 * @return True if the function has extracted the resolution successfully, false if not.
		 */
		bool GetJPGRes(MCore::DiskFile& file, int* outWidth, int* outHeight);

		/**
		 * Extract resolution from a PNG file.
		 * @param filename The filename of the image file.
		 * @param outWidth Pointer to an integer to which the function will write the image's width.
		 * @param outHeight Pointer to an integer to which the function will write the image's height.
		 * @return True if the function has extracted the resolution successfully, false if not.
		 */
		bool GetPNGRes(MCore::DiskFile& file, int* outWidth, int* outHeight);

		/**
		 * Extract resolution from a Photoshop PSD file.
		 * @param filename The filename of the image file.
		 * @param outWidth Pointer to an integer to which the function will write the image's width.
		 * @param outHeight Pointer to an integer to which the function will write the image's height.
		 * @return True if the function has extracted the resolution successfully, false if not.
		 */
		bool GetPSDRes(MCore::File& file, int* outWidth, int* outHeight);
};

} // namespace EMotionFX


#endif