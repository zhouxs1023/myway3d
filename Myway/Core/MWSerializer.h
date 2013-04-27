#pragma once

#include "MWCore.h"
#include "MWMath.h"
#include "MWProperty.h"

namespace Myway {

class MW_ENTRY ISerializer
{
public:
	ISerializer() {}
	virtual ~ISerializer() {}

	virtual bool IsSave() = 0;
	virtual bool IsOpen() = 0;
	virtual bool eof() = 0;
	virtual void Read(void * data, int size) = 0;
	virtual void Write(const void * data, int size) = 0;

	virtual ISerializer & operator >>(bool & data) = 0;
	virtual ISerializer & operator >>(char & data) = 0;
	virtual ISerializer & operator >>(int & data) = 0;
	virtual ISerializer & operator >>(float & data) = 0;
	virtual ISerializer & operator >>(double & data) = 0;
	virtual ISerializer & operator >>(TString128 & data) = 0;
	virtual ISerializer & operator >>(TString256 & data) = 0;
	virtual ISerializer & operator >>(Color & data) = 0;
	virtual ISerializer & operator >>(Color4 & data) = 0;
	virtual ISerializer & operator >>(Vec3 & data) = 0;
	virtual ISerializer & operator >>(Vec4 & data) = 0;
	virtual ISerializer & operator >>(Quat & data) = 0;
	virtual ISerializer & operator >>(Mat3 & data) = 0;
	virtual ISerializer & operator >>(Mat4 & data) = 0;
	virtual ISerializer & operator >>(IPropertyObj * obj) = 0;

	virtual ISerializer & operator <<(bool data) = 0;
	virtual ISerializer & operator <<(char data) = 0;
	virtual ISerializer & operator <<(int data) = 0;
	virtual ISerializer & operator <<(float data) = 0;
	virtual ISerializer & operator <<(double data) = 0;
	virtual ISerializer & operator <<(const char * data) = 0;
	virtual ISerializer & operator <<(const TString128 & data) = 0;
	virtual ISerializer & operator <<(const TString256 & data) = 0;
	virtual ISerializer & operator <<(const Color & data) = 0;
	virtual ISerializer & operator <<(const Color4 & data) = 0;
	virtual ISerializer & operator <<(const Vec3 & data) = 0;
	virtual ISerializer & operator <<(const Vec4 & data) = 0;
	virtual ISerializer & operator <<(const Quat & data) = 0;
	virtual ISerializer & operator <<(const Mat3 & data) = 0;
	virtual ISerializer & operator <<(const Mat4 & data) = 0;
	virtual ISerializer & operator <<(IPropertyObj * obj) = 0;
};





//////////////////////////////////////////////////////////////////////////
//
// FileSerializer
//
class MW_ENTRY FileSerializer : public ISerializer
{
	DECLARE_ALLOC();

public:
	FileSerializer(const char * filename, bool saving = true);
	~FileSerializer();

	bool IsSave() { return mIsSave; }

	bool IsOpen();
	bool eof();
	void Read(void * data, int size);
	void Write(const void * data, int size);

	FileSerializer & operator >>(bool & data);
	FileSerializer & operator >>(char & data);
	FileSerializer & operator >>(int & data);
	FileSerializer & operator >>(float & data);
	FileSerializer & operator >>(double & data);
	FileSerializer & operator >>(TString128 & data);
	FileSerializer & operator >>(TString256 & data);
	FileSerializer & operator >>(Color & data);
	FileSerializer & operator >>(Color4 & data);
	FileSerializer & operator >>(Vec3 & data);
	FileSerializer & operator >>(Vec4 & data);
	FileSerializer & operator >>(Quat & data);
	FileSerializer & operator >>(Mat3 & data);
	FileSerializer & operator >>(Mat4 & data);
	FileSerializer & operator >>(IPropertyObj * obj);

	FileSerializer & operator <<(bool data);
	FileSerializer & operator <<(char data);
	FileSerializer & operator <<(int data);
	FileSerializer & operator <<(float data);
	FileSerializer & operator <<(double data);
	FileSerializer & operator <<(const char * data);
	FileSerializer & operator <<(const TString128 & data);
	FileSerializer & operator <<(const TString256 & data);
	FileSerializer & operator <<(const Color & data);
	FileSerializer & operator <<(const Color4 & data);
	FileSerializer & operator <<(const Vec3 & data);
	FileSerializer & operator <<(const Vec4 & data);
	FileSerializer & operator <<(const Quat & data);
	FileSerializer & operator <<(const Mat3 & data);
	FileSerializer & operator <<(const Mat4 & data);
	FileSerializer & operator <<(IPropertyObj * obj);

protected:
	bool mIsSave;
	File mFile;
};


// inline
inline FileSerializer & FileSerializer::operator >>(bool & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(char & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(int & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(float & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(double & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(TString128 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(TString256 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Color & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Color4 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Vec3 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Vec4 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Quat & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Mat3 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator >>(Mat4 & data)
{
	mFile.Read(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(bool data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(char data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(int data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(float data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(double data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const char * data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const TString128 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const TString256 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Color & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Color4 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Vec3 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Vec4 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Quat & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Mat3 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(const Mat4 & data)
{
	mFile.Write(&data, sizeof (data));
	return *this;
}

inline FileSerializer & FileSerializer::operator <<(IPropertyObj * obj)
{
	int iPropertySize = obj->GetPropertySize();

	for (int j = 0; j < iPropertySize; ++j)
	{
		const Property * p = obj->GetProperty(j);
		const void * data = obj->GetPropertyData(p);

		*this << p->name;
		*this << p->size;
		Write(data, p->size);
	}

	*this << TString128("_eof");

	return *this;
}

inline FileSerializer & FileSerializer::operator >>(IPropertyObj * obj)
{
	char buffer[1024];

	while (1)
	{
		TString128 name;
		int size;

		*this >> name;

		if (name == "_eof")
			break;

		*this >> size;

		d_assert (size < 1024);

		Read(buffer, size);

		const Property * p = obj->GetProperty(name.c_str());

		if (p)
			obj->SetPropertyData(p, buffer);
	}

	return *this;
}



//////////////////////////////////////////////////////////////////////////
//
// StreamSerializer
//
class MW_ENTRY StreamSerializer : public ISerializer
{
	DECLARE_ALLOC();

public:
	StreamSerializer(DataStreamPtr stream) { mStream = stream; }
	virtual ~StreamSerializer() {}

	bool IsSave() { return false; }

	bool IsOpen() { return mStream != NULL; }
	bool eof() { return mStream->Eof(); }

	void Read(void * data, int size) { d_assert (mStream != NULL); mStream->Read(data, size); } 
	void Write(const void * data, int size) { d_assert(0); }

	StreamSerializer & operator >>(bool & data);
	StreamSerializer & operator >>(char & data);
	StreamSerializer & operator >>(int & data);
	StreamSerializer & operator >>(float & data);
	StreamSerializer & operator >>(double & data);
	StreamSerializer & operator >>(TString128 & data);
	StreamSerializer & operator >>(TString256 & data);
	StreamSerializer & operator >>(Color & data);
	StreamSerializer & operator >>(Color4 & data);
	StreamSerializer & operator >>(Vec3 & data);
	StreamSerializer & operator >>(Vec4 & data);
	StreamSerializer & operator >>(Quat & data);
	StreamSerializer & operator >>(Mat3 & data);
	StreamSerializer & operator >>(Mat4 & data);
	StreamSerializer & operator >>(IPropertyObj * obj);

	StreamSerializer & operator <<(bool data);
	StreamSerializer & operator <<(char data);
	StreamSerializer & operator <<(int data);
	StreamSerializer & operator <<(float data);
	StreamSerializer & operator <<(double data);
	StreamSerializer & operator <<(const char * data);
	StreamSerializer & operator <<(const TString128 & data);
	StreamSerializer & operator <<(const TString256 & data);
	StreamSerializer & operator <<(const Color & data);
	StreamSerializer & operator <<(const Color4 & data);
	StreamSerializer & operator <<(const Vec3 & data);
	StreamSerializer & operator <<(const Vec4 & data);
	StreamSerializer & operator <<(const Quat & data);
	StreamSerializer & operator <<(const Mat3 & data);
	StreamSerializer & operator <<(const Mat4 & data);
	StreamSerializer & operator <<(IPropertyObj * obj);

protected:
	DataStreamPtr mStream;
};


// inline
inline StreamSerializer & StreamSerializer::operator >>(bool & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(char & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(int & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(float & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(double & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(TString128 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(TString256 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Color & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Color4 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Vec3 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Vec4 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Quat & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Mat3 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(Mat4 & data)
{
	mStream->Read(&data, sizeof (data));
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(bool data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(char data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(int data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(float data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(double data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const char * data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const TString128 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const TString256 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Color & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Color4 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Vec3 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Vec4 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Quat & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Mat3 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(const Mat4 & data)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator <<(IPropertyObj * obj)
{
	d_assert(0);
	return *this;
}

inline StreamSerializer & StreamSerializer::operator >>(IPropertyObj * obj)
{
	char buffer[1024];

	while (1)
	{
		TString128 name;
		int size;

		*this >> name;

		if (name == "_eof")
			break;

		*this >> size;

		d_assert (size < 1024);

		Read(buffer, size);

		const Property * p = obj->GetProperty(name.c_str());

		if (p)
			obj->SetPropertyData(p, buffer);
	}

	return *this;
}


}