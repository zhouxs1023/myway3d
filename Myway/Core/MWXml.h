#pragma once

#include "tinyxml.h"
#include "rapidxml.hpp"

#include "MWDefine.h"

namespace Myway
{

typedef rapidxml::xml_document<char> xml_doc;
typedef rapidxml::xml_node<char> xml_node;
typedef rapidxml::xml_attribute<char> xml_attri;

class MW_ENTRY XmlHelper
{
public:
    static char * LoadXmlFromFile(xml_doc & doc, const char * filename);
    static void LoadXmlFromMem(xml_doc & doc, char * data);
};

}