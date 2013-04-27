#include "MNavPath.h"

namespace Myway {

	NavPath::NavPath()
		: mSize(0, 0)
		, mData(NULL)
		, mHeight(NULL)
	{
	}

	NavPath::~NavPath()
	{
		safe_delete_array(mData);
		safe_delete_array(mHeight);
	}

	void NavPath::Resize(int w, int h)
	{
		safe_delete_array(mData);
		safe_delete_array(mHeight);

		mSize = Size2(w, h);

		if (w > 0 && h > 0)
		{
			mData = new char[w * h];
			mHeight = new float[w * h];

			memset(mData, 0, sizeof(char) * w * h);
			memset(mHeight, 0, sizeof(float) * w * h);
		}
	}

	void NavPath::Set(int x, int y)
	{
		d_assert (x < mSize.w && y < mSize.h);

		mData[y * mSize.w + x] = 1;
	}

	void NavPath::Clear(int x, int y)
	{
		d_assert (x < mSize.w && y < mSize.h);

		mData[y * mSize.w + x] = 0;
	}

	char NavPath::Get(int x, int y) const
	{
		d_assert (x < mSize.w && y < mSize.h);

		return mData[y * mSize.w + x];
	}

	void NavPath::SetHeight(int x, int y, float h)
	{
		d_assert (x < mSize.w && y < mSize.h);

		mHeight[y * mSize.w + x] = h;;
	}

	float NavPath::GetHeight(int x, int y) const
	{
		d_assert (x < mSize.w && y < mSize.h);

		return mHeight[y * mSize.w + x];
	}

	const Size2 & NavPath::GetSize() const
	{
		return mSize;
	}

	void NavPath::Serialize(ISerializer & Serializer)
	{
		const int K_Magic = 'Path';
		const int K_Version = 0;

		if (Serializer.IsSave())
		{
			Serializer << K_Magic;
			Serializer << K_Version;

			Serializer << mSize.w;
			Serializer << mSize.h;

			Serializer.Write(mData, mSize.w * mSize.h * sizeof(char));
			Serializer.Write(mHeight, mSize.w * mSize.h * sizeof(float));
		}
		else
		{
			int magic = 0, version = 0;

			Serializer >> magic;
			Serializer >> version;

			d_assert (magic == K_Magic && version == K_Version);

			int w = 0, h = 0;

			Serializer >> w;
			Serializer >> h;

			Resize(w, h);

			Serializer.Read(mData, mSize.w * mSize.h * sizeof(char));
			Serializer.Read(mHeight, mSize.w * mSize.h * sizeof(float));
		}
	}

}

