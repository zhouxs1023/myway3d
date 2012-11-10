#pragma once

#include "MWFXComposer.h"

namespace Myway
{

class MW_ENTRY FXComposerMapping
{
public:
    static FORMAT               GetFormat(const String & x);
    static int               GetWidth(const String & x);
    static int               GetHeight(const String & x);
    static MULTI_SAMPLE         GetMSAA(const String & x);
    static FX_RENDER_MODE       GetRenderMode(const String & x);

    static String               GetMSAAString(MULTI_SAMPLE x);
    static String               GetFormatString(FORMAT x);
    static String               GetZFormatString(FORMAT x);
    static String               GetRenderModeString(FX_RENDER_MODE x);
};

class MW_ENTRY FXComposerImport
{
public:
    static void Import(FXComposer * com, const String & sFileName);

protected:
    static void ParseRenderTarget(FXComposer * com, CommandScript & cst);
    static void ParseStep(FXComposer * com, CommandScript & cst);
};


class MW_ENTRY FXComposerExport
{
public:
    static void Export(FXComposer * com, const String & sFileName);

protected:
    static void WriteRenderTarget(RenderTarget * tex, File & file);
    static void WriteStep(FXStep * step, int i, File & file);
};

}
