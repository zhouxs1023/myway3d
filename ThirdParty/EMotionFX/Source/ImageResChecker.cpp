/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "ImageResChecker.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
ImageResChecker::ImageResChecker()
{
}


// destructor
ImageResChecker::~ImageResChecker()
{
}


// extract the resolution
bool ImageResChecker::GetResolution(const MCore::String& filename, int* outWidth, int* outHeight)
{
	MCore::DiskFile file;

	// extract the resolutions
	if (file.Open(filename + ".bmp", MCore::DiskFile::READ, true))
	{
		if (!GetBMPRes(file, outWidth, outHeight)) { file.Close(); return false; }
		file.Close();
		return true;
	}
	else if (file.Open(filename + ".jpg", MCore::DiskFile::READ, true))
	{
		if (!GetJPGRes(file, outWidth, outHeight)) { file.Close(); return false; }
		file.Close();
		return true;
	}
	else if (file.Open(filename + ".png", MCore::DiskFile::READ, true))
	{
		if (!GetPNGRes(file, outWidth, outHeight)) { file.Close(); return false; }
		file.Close();
		return true;
	}
	else if (file.Open(filename + ".tga", MCore::DiskFile::READ, true))
	{
		if (!GetTGARes(file, outWidth, outHeight)) { file.Close(); return false; }
		file.Close();
		return true;
	}
	else if (file.Open(filename + ".psd", MCore::DiskFile::READ, true))
	{
		if (!GetPSDRes(file, outWidth, outHeight)) { file.Close(); return false; }
		file.Close();
		return true;
	}

	// no texture found
	return false;
}


// extract the resolution from a bmp file
bool ImageResChecker::GetBMPRes(MCore::DiskFile& file, int* outWidth, int* outHeight)
{
	MCore::LOG("Extracting BMP resolution...");

	if (!file.Seek(18)) return false;
	if (file.Read(outWidth, 4) == 0) return false;
	if (file.Read(outHeight, 4) == 0) return false;

	MCore::LOG("Resolution : %dx%d", *outWidth, *outHeight);

	return true;
}


// extract the resolution of a tga file
bool ImageResChecker::GetTGARes(MCore::DiskFile& file, int* outWidth, int* outHeight)
{
	MCore::LOG("Extracting TGA resolution...");

	if (!file.Seek(12)) return false;
	short width, height;
	if (file.Read(&width, 2) == 0) return false;
	if (file.Read(&height, 2) == 0) return false;
	MCore::LOG("Resolution : %dx%d", width, height);

	*outWidth = width;
	*outHeight = height;

	return true;
}


// extract the resolution from a jpg file
bool ImageResChecker::GetJPGRes(MCore::DiskFile& file, int* outWidth, int* outHeight)
{
	MCore::LOG("Extracting JPG resolution...");

	unsigned char dummy[16];		
	unsigned char sig[2];
	int readLen = file.Read(&sig, 2);

	if (sig[0]!=0xFF || sig[1]!=0xD8)
		readLen = 0;

	if (readLen > 0)
	{
		unsigned char seg;
		readLen = file.Read(&seg, 1);
		while ((seg == 0xFF) && (readLen > 0))
		{
			readLen = file.Read(&seg, 1);
			if (seg != 0xFF)
			{
				if ((seg == 0xC0) || (seg == 0xC1))
				{
					readLen = file.Read(dummy, 3);
					short int width, height;
					file.Read(&height, 2);
					file.Read(&width, 2);
					MCore::ConvertWordEndian(width);
					MCore::ConvertWordEndian(height);
					*outHeight = height;
					*outWidth = width;
					MCore::LOG("Resolution : %dx%d", width, height);
					return true;
				}
				else
				{
					if (seg!=0x01 && seg!=0xD0 && seg!=0xD1 && seg!=0xD2 && seg!=0xD3 && seg!=0xD4 && seg!=0xD5 && seg!=0xD6 && seg!=0xD7)
					{
						short int len;
						file.Read(&len, 2);
						MCore::ConvertWordEndian(len);
						file.Forward(len-2);
						file.Read(&seg, 1);
					}
					else
						seg = 0xFF;
				}
			}
		}
	}

	MCore::LOG("no res found!");
	return false;
}


// extract the resolution from a png file
bool ImageResChecker::GetPNGRes(MCore::DiskFile& file, int* outWidth, int* outHeight)
{
	MCore::LOG("Extracting PNG resolution...");

	int width, height;
	if (!file.Seek(16)) return false;
	if (file.Read(&width, 4) == 0) return false;
	if (file.Read(&height, 4) == 0) return false;

	// convert from big to little endian
	// TODO: don't do this conversion on hardware which already is big endian
	MCore::ConvertIntEndian( width );
	MCore::ConvertIntEndian( height );
	*outWidth = width;
	*outHeight = height;

	MCore::LOG("Resolution : %dx%d", *outWidth, *outHeight);

	return true;
}


// extract the resolution from a psd file
bool ImageResChecker::GetPSDRes(MCore::File& file, int* outWidth, int* outHeight)
{
	MCore::LOG("Extracting PSD resolution...");

	int width, height;
	if (!file.Seek(14)) return false;
	if (file.Read(&height, 4) == 0) return false;
	if (file.Read(&width, 4) == 0) return false;

	// convert from big to little endian
	// TODO: don't do this conversion on hardware which already is big endian
	MCore::ConvertIntEndian(width);
	MCore::ConvertIntEndian(height);
	*outWidth = width;
	*outHeight = height;

	MCore::LOG("Resolution : %dx%d", *outWidth, *outHeight);

	return true;
}

} // namespace EMotionFX