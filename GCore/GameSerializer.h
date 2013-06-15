#pragma once

namespace game {

	class GSerializer
	{
	public:
		GSerializer(const char * filename, bool saving = true);
		~GSerializer();

		bool IsSave() { return mIsSave; }

		bool IsOpen();
		bool eof();
		void Read(void * data, int size);
		void Write(const void * data, int size);

		GSerializer & operator >>(bool & data);
		GSerializer & operator >>(char & data);
		GSerializer & operator >>(int & data);
		GSerializer & operator >>(float & data);
		GSerializer & operator >>(double & data);
		GSerializer & operator >>(TString128 & data);
		GSerializer & operator >>(TString256 & data);
		GSerializer & operator >>(Color & data);
		GSerializer & operator >>(Color4 & data);
		GSerializer & operator >>(Vec3 & data);
		GSerializer & operator >>(Vec4 & data);
		GSerializer & operator >>(Quat & data);
		GSerializer & operator >>(Mat3 & data);
		GSerializer & operator >>(Mat4 & data);
		GSerializer & operator >>(IPropertyObj * obj);

		GSerializer & operator <<(bool data);
		GSerializer & operator <<(char data);
		GSerializer & operator <<(int data);
		GSerializer & operator <<(float data);
		GSerializer & operator <<(double data);
		GSerializer & operator <<(const char * data);
		GSerializer & operator <<(const TString128 & data);
		GSerializer & operator <<(const TString256 & data);
		GSerializer & operator <<(const Color & data);
		GSerializer & operator <<(const Color4 & data);
		GSerializer & operator <<(const Vec3 & data);
		GSerializer & operator <<(const Vec4 & data);
		GSerializer & operator <<(const Quat & data);
		GSerializer & operator <<(const Mat3 & data);
		GSerializer & operator <<(const Mat4 & data);
		GSerializer & operator <<(IPropertyObj * obj);

	protected:
		bool mIsSave;
		File mFile;
	};


	// inline
	inline GSerializer & GSerializer::operator >>(bool & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(char & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(int & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(float & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(double & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(TString128 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(TString256 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Color & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Color4 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Vec3 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Vec4 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Quat & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Mat3 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator >>(Mat4 & data)
	{
		mFile.Read(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(bool data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(char data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(int data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(float data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(double data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const char * data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const TString128 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const TString256 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Color & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Color4 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Vec3 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Vec4 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Quat & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Mat3 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(const Mat4 & data)
	{
		mFile.Write(&data, sizeof (data));
		return *this;
	}

	inline GSerializer & GSerializer::operator <<(IPropertyObj * obj)
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

	inline GSerializer & GSerializer::operator >>(IPropertyObj * obj)
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