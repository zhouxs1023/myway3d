#include "MWXml.h"

#pragma warning(push)
#pragma warning(disable : 4996)

#include "tinyxml.cpp"
#include "tinyxmlerror.cpp"
#include "tinyxmlparser.cpp"
#include "MWFile.h"

#pragma warning(pop)

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
}
