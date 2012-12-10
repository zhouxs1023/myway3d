#include "stdafx.h"
#include "xExporterConfig.h"

xExportConfig gConfig;

IMP_SLN(xExportConfig);

xExportConfig::xExportConfig()
{
	INIT_SLN;

	mExportSelected = false;
}

xExportConfig::~xExportConfig()
{
	SHUT_SLN;
}