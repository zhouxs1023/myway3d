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

	static bool GetAttribute(xml_attri * attri, int & x);
	static bool GetAttribute(xml_attri * attri, int & x, int & y);
	static bool GetAttribute(xml_attri * attri, int & x, int & y, int & z);
	static bool GetAttribute(xml_attri * attri, int & x, int & y, int & z, int & w);

	static bool GetAttribute(xml_attri * attri, float & x);
	static bool GetAttribute(xml_attri * attri, float & x, float & y);
	static bool GetAttribute(xml_attri * attri, float & x, float & y, float & z);
	static bool GetAttribute(xml_attri * attri, float & x, float & y, float & z, float & w);
	
	static bool GetAttribute(xml_attri * attri, char * x);
	static bool GetAttribute(xml_attri * attri, char * x, char * y);
	static bool GetAttribute(xml_attri * attri, char * x, char * y, char * z);
	static bool GetAttribute(xml_attri * attri, char * x, char * y, char * z, char * w);
};

}