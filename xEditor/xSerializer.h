#pragma once

#include "xEditor.h"

class X_ENTRY xSerializer
{
public:
	xSerializer(const char * filename, bool saving = true);
	~xSerializer();

	bool IsSave() { return mIsSave; }

	void Read(void * data, int size);
	void Write(const void * data, int size);

	xSerializer & operator >>(bool & data);
	xSerializer & operator >>(char & data);
	xSerializer & operator >>(int & data);
	xSerializer & operator >>(float & data);
	xSerializer & operator >>(double & data);
	xSerializer & operator >>(TString128 & data);
	xSerializer & operator >>(TString256 & data);
	xSerializer & operator >>(Color & data);
	xSerializer & operator >>(Color4 & data);
	xSerializer & operator >>(Vec3 & data);
	xSerializer & operator >>(Vec4 & data);
	xSerializer & operator >>(Quat & data);
	xSerializer & operator >>(Mat3 & data);
	xSerializer & operator >>(Mat4 & data);

	xSerializer & operator <<(bool data);
	xSerializer & operator <<(char data);
	xSerializer & operator <<(int data);
	xSerializer & operator <<(float data);
	xSerializer & operator <<(double data);
	xSerializer & operator <<(const char * data);
	xSerializer & operator <<(const TString128 & data);
	xSerializer & operator <<(const TString256 & data);
	xSerializer & operator <<(const Color & data);
	xSerializer & operator <<(const Color4 & data);
	xSerializer & operator <<(const Vec3 & data);
	xSerializer & operator <<(const Vec4 & data);
	xSerializer & operator <<(const Quat & data);
	xSerializer & operator <<(const Mat3 & data);
	xSerializer & operator <<(const Mat4 & data);

protected:
	bool mIsSave;
	File mFile;
};


// inline
inline xSerializer & xSerializer::operator >>(bool & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(char & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(int & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(float & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(double & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(TString128 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(TString256 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Color & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Color4 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Vec3 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Vec4 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Quat & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Mat3 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator >>(Mat4 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(bool data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(char data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(int data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(float data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(double data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const char * data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const TString128 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const TString256 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Color & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Color4 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Vec3 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Vec4 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Quat & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Mat3 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline xSerializer & xSerializer::operator <<(const Mat4 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}