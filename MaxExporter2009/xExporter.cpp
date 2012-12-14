#include "stdafx.h"

#include "xExporter.h"
#include "resource.h"

//------------------------------------------------------
static xExportClassDesc gExportDesc;

int xExportClassDesc::IsPublic()
{
	return 1; 
}

void * xExportClassDesc::Create(BOOL loading)
{ 
	return new xMaxExport();
}

const TCHAR * xExportClassDesc::ClassName() 
{ 
	return "Myway Exporter"; 
}

SClass_ID xExportClassDesc::SuperClassID() 
{ 
	return SCENE_EXPORT_CLASS_ID; 
}

Class_ID xExportClassDesc::ClassID() 
{ 
	return Class_ID('Mywa', 'y3D0'); 
}

const TCHAR* xExportClassDesc::Category()
{ 
	return _T("Myway Exporter"); 
}

extern "C" {

__declspec( dllexport ) const TCHAR *
LibDescription()
{ 
	return _T("Myway Exporter"); 
}

__declspec( dllexport ) int
LibNumberClasses()
{ 
	return 1; 
}

__declspec( dllexport ) ClassDesc *
LibClassDesc(int i)
{
	switch(i)
	{
	case 0: 
		return &gExportDesc; 
		break;
	}

	return NULL;
}

__declspec( dllexport ) ULONG 
LibVersion()
{ 
	return VERSION_3DSMAX; 
}

__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}

}










#include "xExportConfig.h"
#include "xMeshExporter.h"
#include "xExportSettingDlg.h"
#include "xTextureExporter.h"

using namespace MaxPlugin;

xMaxExport::xMaxExport()
{
}

xMaxExport::~xMaxExport()
{
}

int	xMaxExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	xExportConfig::Instance()->SetExportFilename(name);

	xExportSettingDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		xTextureExporter::Instance()->Clear();

		xMeshExporter meshExp(ei, i);
		meshExp.Export();

		xTextureExporter::Instance()->Export();

		return IMPEXP_SUCCESS;
	}
	
	return IMPEXP_FAIL;
}

int xMaxExport::ExtCount()
{
	return 1;
}

const TCHAR * xMaxExport::Ext(int n)
{
	switch (n)
	{
	case 0:
		return _T("mesh");
		break;

	default:
		return 0;
		break;
	}
}

const TCHAR * xMaxExport::LongDesc()
{ 
	return _T("Myway Exporter");
}

const TCHAR * xMaxExport::ShortDesc()
{
	return _T("Myway Exporter");
}

const TCHAR * xMaxExport::AuthorName()
{ 
	return _T("Myway");
}

const TCHAR * xMaxExport::CopyrightMessage()
{ 
	return _T("The Infinate Team (c) 2012");
}

const TCHAR * xMaxExport::OtherMessage1()
{ 
	return 0;
}

const TCHAR * xMaxExport::OtherMessage2()
{ 
	return 0;
}

unsigned int xMaxExport::Version()
{ 
	return 1;
}

void xMaxExport::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, "Myway Exporter", "About", 0);
}

BOOL xMaxExport::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}

