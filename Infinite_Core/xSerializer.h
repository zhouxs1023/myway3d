//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "Infinite.h"

class INFI_ENTRY xSerializer
{
public:
	xSerializer(const char * filename, bool saving = true);
	~xSerializer();

	bool IsSave() { return mIsSave; }

	bool IsOpen();
	bool eof();
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
	xSerializer & operator >>(IPropertyObj * obj);

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
	xSerializer & operator <<(IPropertyObj * obj);

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

inline xSerializer & xSerializer::operator <<(IPropertyObj * obj)
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

inline xSerializer & xSerializer::operator >>(IPropertyObj * obj)
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