#include "stdafx.h"
#include "xExportConfig.h"

xExportConfig gConfig;

IMP_SLN(xExportConfig);

xExportConfig::xExportConfig()
{
	INIT_SLN;

	mExportSelected = false;

	mExportNormal = true;
	mExportTangent = false;
	mExportColor = false;
	mExportTexcoord = true;
	mExportLightmapUV = false;
}

xExportConfig::~xExportConfig()
{
	SHUT_SLN;
}

void xExportConfig::Load()
{
	const char * filename = "./xMaxExporter.ini";

	xml_doc doc;
	char * data = XmlHelper::LoadXmlFromFile(doc, filename);

	if (data)
	{
		xml_node * root = doc.first_node("Config");

		xml_node * ndExportSelected = root->first_node("ExportSelected");
		xml_node * ndExportNormal = root->first_node("ExportNormal");
		xml_node * ndExportTangent = root->first_node("ExportTangent");
		xml_node * ndExportColor = root->first_node("ExportColor");
		xml_node * ndExportTexcoord = root->first_node("ExportTexcoord");
		xml_node * ndExportLightmapUV = root->first_node("ExportLightmapUV");

		if (ndExportSelected)
		{
			TString128 val = ndExportSelected->first_attribute("Value")->value();

			if (val.LowerR() == "true")
				mExportSelected = true;
		}

		if (ndExportNormal)
		{
			TString128 val = ndExportNormal->first_attribute("Value")->value();

			if (val.LowerR() == "true")
				mExportNormal = true;
		}

		if (ndExportTangent)
		{
			TString128 val = ndExportTangent->first_attribute("Value")->value();

			if (val.LowerR() == "true")
				mExportTangent = true;
		}

		if (ndExportColor)
		{
			TString128 val = ndExportColor->first_attribute("Value")->value();

			if (val.LowerR() == "true")
				mExportColor = true;
		}

		if (ndExportTexcoord)
		{
			TString128 val = ndExportTexcoord->first_attribute("Value")->value();

			if (val.LowerR() == "true")
				mExportTexcoord = true;
		}

		if (ndExportLightmapUV)
		{
			TString128 val = ndExportLightmapUV->first_attribute("Value")->value();

			if (val.LowerR() == "true")
				mExportLightmapUV = true;
		}
	}

	safe_free(data);
}

void xExportConfig::Save()
{
	const char * filename = "./xMaxExporter.ini";

	std::ofstream stream;

	stream.open(filename);

	d_assert (stream.is_open());

	stream << "<Config>" << std::endl;
	{
		stream << "    " << "<ExportSelected Value = \"" << (mExportSelected ? "true" : "false") << "\"/>" << std::endl;
		stream << "    " << "<ExportNormal Value = \"" << (mExportNormal ? "true" : "false") << "\"/>" << std::endl;
		stream << "    " << "<ExportTangent Value = \"" << (mExportTangent ? "true" : "false") << "\"/>" << std::endl;
		stream << "    " << "<ExportColor Value = \"" << (mExportColor ? "true" : "false") << "\"/>" << std::endl;
		stream << "    " << "<ExportTexcoord Value = \"" << (mExportTexcoord ? "true" : "false") << "\"/>" << std::endl;
		stream << "    " << "<ExportLightmapUV Value = \"" << (mExportLightmapUV ? "true" : "false") << "\"/>" << std::endl;
	}
	stream << "</Config>" << std::endl;
}

void xExportConfig::SetExportFilename(const TString128 & filename)
{
	mExportFilename = filename;

	mExportFilename.SplitFileNameR(mExportBaseName, mExportPath);
}