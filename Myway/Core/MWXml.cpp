#include "MWXml.h"

#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4996)

#include "tinyxml.cpp"
#include "tinyxmlerror.cpp"
#include "tinyxmlparser.cpp"
#include "MWFile.h"

namespace Myway
{
    char * XmlHelper::LoadXmlFromFile(xml_doc & doc, const char * filename)
    {
        File file;

        if (file.Open(filename, OM_READ_BINARY))
        {
            int size = file.Size();

            char * data = (char *)alloc_m(size + 1);

            file.Read(data, sizeof(char), size);
            data[size] = 0;
            LoadXmlFromMem(doc, data);

            return data;
        }

        return NULL;
    }

    void XmlHelper::LoadXmlFromMem(xml_doc & doc, char * data)
    {
        doc.parse<0>(data);
    }

	bool XmlHelper::GetAttribute(xml_attri * attri, int & x)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%d", &x);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, int & x, int & y)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%d %d", &x, &y);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, int & x, int & y, int & z)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%d %d %d", &x, &y, &z);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, int & x, int & y, int & z, int & w)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%d %d %d %d", &x, &y, &z, &w);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, float & x)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%f", &x);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, float & x, float & y)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%f %f", &x, &y);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, float & x, float & y, float & z)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%f %f %f", &x, &y, &z);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, float & x, float & y, float & z, float & w)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%f %f %f %f", &x, &y, &z, &w);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, char * x)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%s", x);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, char * x, char * y)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%s %s", x, y);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, char * x, char * y, char * z)
	{
		if (!attri)
			 return false;

		const char * val = attri->value();

		sscanf(val, "%s %s %s", x, y, z);

		return true;
	}

	bool XmlHelper::GetAttribute(xml_attri * attri, char * x, char * y, char * z, char * w)
	{
		if (!attri)
			return false;

		const char * val = attri->value();

		sscanf(val, "%s %s %s %s", x, y, z, w);

		return true;
	}
}

#pragma warning(pop)
