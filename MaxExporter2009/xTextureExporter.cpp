#include "stdafx.h"

#include "xTextureExporter.h"
#include "xExportConfig.h"

namespace MaxPlugin {

xTextureExporter gTextureExp;

IMP_SLN(xTextureExporter);

xTextureExporter::xTextureExporter()
{
	INIT_SLN;
}

xTextureExporter::~xTextureExporter()
{
	SHUT_SLN;
}

void xTextureExporter::Clear()
{
	mTextures.Clear();
}

void xTextureExporter::Push(const char * texture)
{
	TString128 sTex = texture;

	if (mTextures.Find(sTex) == mTextures.End())
	{
		mTextures.Insert(sTex, 1);
	}
}

void xTextureExporter::Export()
{
	Map<TString128, int>::Iterator whr = mTextures.Begin();
	Map<TString128, int>::Iterator end = mTextures.End();

	TString128 expPath = xExportConfig::Instance()->GetExportPath();

	while (whr != end)
	{
		TString128 filename = whr->first;

		TString128 basename = File::GetBaseName(filename);

		TString128 newFilename = expPath + "\\" + basename;

		CopyFile(filename.c_str(), newFilename.c_str(), FALSE);

		++whr;
	}
}

}
