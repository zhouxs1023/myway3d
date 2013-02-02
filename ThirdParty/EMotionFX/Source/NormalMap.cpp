/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NormalMap.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
NormalMap::NormalMap(const int width, const int height, MCore::String& name)
{ 
	// reset members
	mPixels		= NULL;
	mWidth		= 0;
	mHeight		= 0;
	mName		= name;

	// allocate the data
	Alloc(width, height); 
}


// destructor
NormalMap::~NormalMap() 
{ 
	if (mPixels)
		delete[] mPixels;
}


// allocate pixel data for the normal map
void NormalMap::Alloc(const int width, const int height)
{
	if (mPixels)
	{
		delete[] mPixels;
		mPixels = NULL;
	}

	// allocate the new pixel data
	mPixels		= new Pixel[width * height];
	mWidth		= width;
	mHeight		= height;

	// reset pixel values
	for (int y=0; y<height; y++)
	for (int x=0; x<width; x++)
	{
		Pixel* pixel = &mPixels[x+y*width];
		pixel->x	= x;
		pixel->y	= y;
		pixel->r	= 0;
		pixel->g	= 0;
		pixel->b	= 0;
		pixel->dist	= 0.0f;
		pixel->isFree = true;

		// init on the maximum distance etc
		pixel->x	= mWidth  * mWidth;
		pixel->y	= mHeight * mHeight;
		pixel->dist = float(mWidth*mWidth + mHeight*mHeight);
	}
}


// convert a normal into a colored pixel at a given position and set used flag
void NormalMap::WriteNormal(const int x, const int y, MCore::DVector3& normal, bool setUsed)
{
	if (!InRange(x, y)) return;

	Pixel* pixel = &mPixels[x+y*mWidth];
	pixel->r = MCore::Clamp<int>(((normal.x + 1.0) * 0.5) * 255, 0, 255);
	pixel->g = MCore::Clamp<int>(((normal.y + 1.0) * 0.5) * 255, 0, 255);
	pixel->b = MCore::Clamp<int>(((normal.z + 1.0) * 0.5) * 255, 0, 255);
	pixel->x = x;
	pixel->y = y;
	pixel->dist = 0.0f;

	if (setUsed)
		pixel->isFree = false;
}


// convert a normal and it's neighborhood into colored pixels at a given positions and set used flags
void NormalMap::EncodeNormal(const int x, const int y, MCore::DVector3& normal)
{
	// write the normal
	WriteNormal(x, y, normal, true);
}


// save the normal map as bitmap file
bool NormalMap::SaveAsBMP(const MCore::String& filename)
{
	// try to create the file
	MCore::DiskFile file;
	if (!file.Open(filename, MCore::DiskFile::WRITE, true))
	{
		MCore::LOG("Failed to create file '%s'...", filename.GetReadPtr());
		return false;
	}

	// write the header
	char id[2] = {'B', 'M'};
	file.Write(id, 2);

	int filesize = 54 + (mWidth * mHeight * 3);
	file.Write(&filesize, 4);

	int reserved = 0;
	file.Write(&reserved, 4);

	int pixelDataOffset = 54;	// TODO: calculate
	file.Write(&pixelDataOffset, 4);

	int headerSize = 40;	// (40 bytes)
	file.Write(&headerSize, 4);

	file.Write(&mWidth, 4);
	file.Write(&mHeight, 4);

	unsigned short planes = 1;
	file.Write(&planes, 2);

	unsigned short bitsPerPixel = 24;
	file.Write(&bitsPerPixel, 2);

	int compression = 0; // also known as BI_RGB
	file.Write(&compression, 4);

	// data size, rounded up to 4 bytes boundaries
	int dataSize = 0;//mWidth * mHeight * 3;
	int extra = (mWidth * mHeight * 3) % 4;
	//dataSize += extra;
	file.Write(&dataSize, 4);

	int hRes = 0;
	file.Write(&hRes, 4);

	int vRes = 0;
	file.Write(&vRes, 4);

	int colors = 0;//1 << bitsPerPixel;
	file.Write(&colors, 4);

	int importantColors = 0;
	file.Write(&importantColors, 4);

	// write bitmap data
	char temp = 0;

	// flip y
	for (int y=mHeight-1; y>=0; y--)
	{
		for (int x=0; x<mWidth; x++)
		{
			Pixel* pixel = &mPixels[x+y*mWidth];
			file.Write(&pixel->b, 1);	// bgr instead of rgb
			file.Write(&pixel->g, 1);
			file.Write(&pixel->r, 1);
		}
	}

	for (int i=0; i<extra; i++)
		file.Write(&temp, 1);

	// close the file
	file.Close();

	MCore::LOG("Saved normal map '%s'...", filename.GetReadPtr());

	// success!
	return true;
}



// fix errors in the normal map
void NormalMap::FixErrors()
{
	// fix isolated single pixels that are bugs
	// with isolated we mean pixels surrounded by other pixels, while this pixel has very high color difference
	// compared to the neighbors
	for (int y=1; y<mHeight-1; y++)
	{
		for (int x=1; x<mWidth-1; x++)
		{
			Pixel* pixel = &mPixels[x+y*mWidth];

			bool isolated = true;
			for (int sy=-1; sy<=1 && isolated; sy++)
			for (int sx=-1; sx<=1 && isolated; sx++)
			{
				if (sx==0 && sy==0) continue;

				Pixel* sPixel = &mPixels[(x+sx) + ((y+sy)*mWidth)];

				Vector3 dif;
				dif.x = sPixel->r - pixel->r;
				dif.y = sPixel->g - pixel->g;
				dif.z = sPixel->b - pixel->b;

				float length = dif.Length();
				if (length < 20)
					isolated = false;
			}


			// if the pixel was an isolated pixel with high color difference
			// let the EDT algorithm overwrite it
			if (isolated)
			{
				pixel->dist = mWidth*mWidth + mHeight*mHeight;
				pixel->x = x*x;
				pixel->y = y*y;
				pixel->isFree = true;
			}
		}
	}
}



// blur the normalmap
void NormalMap::Blur()
{
	// make a copy of the pixel data in a temp buffer
	Pixel* tempBuffer = new Pixel[mWidth*mHeight];
	memcpy(tempBuffer, mPixels, mWidth*mHeight*sizeof(Pixel));

	// for all pixels in the normalmap (except borders)
	for (int y=0; y<mHeight-1; y++)
	{
		for (int x=0; x<mWidth-1; x++)
		{
			Pixel* pixel = &mPixels[x+y*mWidth];

			// sample all neighbors
			int numSamples = 0;
			int totalR = 0;
			int totalG = 0;
			int totalB = 0;
			for (int sy=0; sy<=1; sy++)
			for (int sx=0; sx<=1; sx++)
			{
				Pixel* sPixel = &tempBuffer[(x+sx) + ((y+sy)*mWidth)];

				totalR += sPixel->r;
				totalG += sPixel->g;
				totalB += sPixel->b;

				numSamples++;
			}

			// average the sample
			if (numSamples > 0)
			{
				pixel->r = MCore::Clamp<int>(totalR / numSamples, 0, 255);
				pixel->g = MCore::Clamp<int>(totalG / numSamples, 0, 255);
				pixel->b = MCore::Clamp<int>(totalB / numSamples, 0, 255);
			}
		}
	}

	// get rid of the temp buffer
	delete[] tempBuffer;
}


// renormalize the normalmap
void NormalMap::Normalize()
{
	for (int y=0; y<mHeight; y++)
	{
		for (int x=0; x<mWidth; x++)
		{
			Pixel* pixel = &mPixels[x+y*mWidth];

			// skip non-used pixels
			if (pixel->isFree)
				continue;

			// get the normal
			DVector3 normal = GetNormal(x, y);

			// normalize it
			normal.Normalize();

			// convert the normal to pixels again and write it in the normal map
			WriteNormal(x, y, normal, true);
		}
	}
}



DVector3 NormalMap::GetNormal(const int x, const int y) const
{
	Pixel* pixel = &mPixels[x+y*mWidth];

	// convert the pixel to a 3Dnormal
	float nx = (((float)pixel->r / 255.0f) * 2.0f) - 1.0f;
	float ny = (((float)pixel->g / 255.0f) * 2.0f) - 1.0f;
	float nz = (((float)pixel->b / 255.0f) * 2.0f) - 1.0f;

	return DVector3(nx, ny, nz);
}


//--------------------------------------------------------------------


void NormalMap::CheckNeighbor(int x, int y, Pixel* pixel, Pixel* neighbor) 
{
	// calculate distance
	float dx = float(neighbor->x - x);
	float dy = float(neighbor->y - y);
	float dist = dx*dx + dy*dy;

	// if it is closer to the minimum distance
	if (dist < pixel->dist) 
	{
		pixel->x = neighbor->x;
		pixel->y = neighbor->y;
		pixel->dist = dist;
	}
}


void NormalMap::FillEmptyPixels()
{
	int x, y;

	// perform the EDT-8 algorithm
	for (y=0; y<mHeight; y++)
	{
		for (x=0; x<mWidth; x++)
		{
			int x0 = MCore::Clamp<int>( x-1, 0, mWidth-1 );
			int x1 = MCore::Clamp<int>( x+1, 0, mWidth-1 );
			int y0 = MCore::Clamp<int>( y-1, 0, mHeight-1 );

			Pixel* p = &mPixels[x + y*mWidth];
			CheckNeighbor( x, y, p, &mPixels[y0*mWidth+x0] );
			CheckNeighbor( x, y, p, &mPixels[y0*mWidth+x] );
			CheckNeighbor( x, y, p, &mPixels[y0*mWidth+x1] );
			CheckNeighbor( x, y, p, &mPixels[y*mWidth+x0] );
		}

		for (x=mWidth-1; x>=0; x--) 
		{
			int x1 = MCore::Clamp<int>( x+1, 0, mWidth-1 );
			Pixel* p = &mPixels[y*mWidth+x];
			CheckNeighbor( x, y, p, &mPixels[y*mWidth+x1] );
		}
	}


	for (y=mHeight-1; y>=0; y--) 
	{
		for (x=mWidth-1; x>=0; x--) 
		{
			int x0 = MCore::Clamp<int>( x-1, 0, mWidth-1 );
			int x1 = MCore::Clamp<int>( x+1, 0, mWidth-1 );
			int y1 = MCore::Clamp<int>( y+1, 0, mHeight-1 );

			Pixel* p = &mPixels[y*mWidth+x];

			CheckNeighbor( x, y, p, &mPixels[y*mWidth+x1] );
			CheckNeighbor( x, y, p, &mPixels[y1*mWidth+x0] );
			CheckNeighbor( x, y, p, &mPixels[y1*mWidth+x] );
			CheckNeighbor( x, y, p, &mPixels[y1*mWidth+x1] );
		}

		for (x=0; x<mWidth; x++)
		{
			int x0 = MCore::Clamp<int>( x-1, 0, mWidth-1 );
			Pixel* p = &mPixels[y*mWidth+x];
			CheckNeighbor(x, y, p, &mPixels[y*mWidth+x0]);
		}
	}

	//------------------------------------------------

	// fill the empty pixels with the nearest neighbor
	for (y=0; y<mHeight; y++)
	{
		for (x=0; x<mWidth; x++)
		{
			Pixel* p = &mPixels[y*mWidth+x];

			int sx = p->x;
			int sy = p->y;

			if (sx>=0 && sx<mWidth && sy>=0 && sy<mHeight) 
			{
				p->r = mPixels[sy*mWidth + sx].r;
				p->g = mPixels[sy*mWidth + sx].g;
				p->b = mPixels[sy*mWidth + sx].b;
				p->isFree = false;
			}
		}
	}
}



void NormalMap::FixBump()
{
	// find the minimum and maximum bump value
	int minVal = 999999;
	int maxVal =-999999;
	int y;
	for (y=0; y<mHeight; y++)
	{
		for (int x=0; x<mWidth; x++)
		{
			Pixel* pixel = &mPixels[y*mWidth+x];

			if (pixel->r < minVal)
				minVal = pixel->r;

			if (pixel->r > maxVal)
				maxVal = pixel->r;
		}
	}

	// the min value must become -127 and the max value +128
	// float scale = (maxVal - minVal);
	for (y=0; y<mHeight; y++)
	{
		for (int x=0; x<mWidth; x++)
		{
			Pixel* pixel = &mPixels[y*mWidth+x];

			float value = pixel->r;

			if (value >= 0)
			{
				value /= (float)maxVal;
				value *= 128;
				value += 128;
				int newVal = MCore::Clamp<int>(value, 128, 255);
				pixel->r = newVal;
				pixel->g = newVal;
				pixel->b = newVal;
			}
			else
			{
				value = Math::Abs(value);
				value /= (float)Math::Abs(minVal);
				value *= 128;
				int newVal = MCore::Clamp<int>(value, 0, 127);
				pixel->r = newVal;
				pixel->g = newVal;
				pixel->b = newVal;
			}
		}
	}
}


} // namespace EMotionFX