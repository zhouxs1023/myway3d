#include "MWFXComposerImport.h"
#include "MWRenderSystem.h"
#include "MWResourceManager.h"

using namespace Myway;

#define render_target       "render_target"
#define name                "name"
#define format              "format"
#define zformat             "zformat"
#define width               "width"
#define height              "height"
#define msaa                "msaa"

#define full                "full"
#define half                "half"
#define quater              "quater"
#define eighth              "eighth"
#define sixteenth           "sixteenth"

#define a8r8g8b8            "A8R8G8B8"
#define x8r8g8b8            "X8R8G8B8"
#define r8g8b8              "R8G8B8"
#define a16b16g16r16        "A16B16G16R16"
#define a16b16g16r16f       "A16B16G16R16F"
#define r16f                "R16F"

#define d16                 "d16"
#define d15s1               "d15s1"
#define d24x4s4             "d24x4s4"
#define d24s8               "d24s8"
#define d32                 "d32"

#define msaa_none           "msaa_none"
#define msaa_2x             "msaa_2x"
#define msaa_3x             "msaa_3x"
#define msaa_4x             "msaa_4x"
#define msaa_5x             "msaa_5x"
#define msaa_6x             "msaa_6x"
#define msaa_7x             "msaa_7x"
#define msaa_8x             "msaa_8x"
#define msaa_9x             "msaa_9x"
#define msaa_10x            "msaa_10x"
#define msaa_11x            "msaa_11x"
#define msaa_12x            "msaa_12x"
#define msaa_13x            "msaa_13x"
#define msaa_14x            "msaa_14x"
#define msaa_15x            "msaa_15x"
#define msaa_16x            "msaa_16x"

#define step                "step"
#define render_mode         "render_mode"
#define render_mark         "render_mark"
#define render_frame        "render_frame"
#define render_target0      "render_target0"
#define render_target1      "render_target1"
#define render_target2      "render_target2"
#define render_target3      "render_target3"
#define material            "material"

#define rm_default          "default"
#define rm_rect             "rect"

#define tab                 "   "
#define space               " "
#define bracket_b           "{"
#define bracket_e           "}"

#define name_a              "name              "
#define format_a            "format            "
#define width_a             "width             "
#define height_a            "height            "
#define msaa_a              "msaa              "


#define render_mode_a       "render_mode       "
#define render_mark_a       "render_mark       "
#define render_frame_a      "render_frame      "
#define render_target0_a    "render_target0    "
#define render_target1_a    "render_target1    "
#define render_target2_a    "render_target2    "
#define render_target3_a    "render_target3    "
#define material_a          "material          "

FORMAT FXComposerMapping::GetFormat(const String & x)
{
    if (x == a8r8g8b8)
        return FMT_A8R8G8B8;
    else if (x == x8r8g8b8)
        return FMT_X8R8G8B8;
    else if (x == r8g8b8)
        return FMT_R8G8B8;
    else if (x == a16b16g16r16)
        return FMT_A16B16G16R16;
    else if (x == a16b16g16r16f)
        return FMT_A16B16G16R16F;
    else if (x == r16f)
        return FMT_R16F;
    else
    {
        debug_assert(0, "error: composer not support this format render target.");
        return FMT_UNKNOWN;
    }
}

int FXComposerMapping::GetWidth(const String & x)
{
    int _width = RenderSystem::Instance().GetPrimaryRenderWindow()->GetBackBufferWidth();

    if (x == full)
        return _width;
    else if (x == half)
        return _width >> 1;
    else if (x == quater)
        return _width >> 2;
    else if (x == eighth)
        return _width >> 3;
    else if (x == sixteenth)
        return _width >> 4;
    else
        return atoi(x.c_str());
}

int FXComposerMapping::GetHeight(const String & x)
{
    int _height = RenderSystem::Instance().GetPrimaryRenderWindow()->GetBackBufferHeight();

    if (x == full)
        return _height;
    else if (x == half)
        return _height >> 1;
    else if (x == quater)
        return _height >> 2;
    else if (x == eighth)
        return _height >> 3;
    else if (x == sixteenth)
        return _height >> 4;
    else
        return atoi(x.c_str());
}

MULTI_SAMPLE FXComposerMapping::GetMSAA(const String & x)
{
    if (x == msaa_none)
    {
        return MULTISAMPLE_NONE;
    }
    else if (x == msaa_2x)
    {
        return MULTISAMPLE_2x;
    }
    else if (x == msaa_3x)
    {
        return MULTISAMPLE_3x;
    }
    else if (x == msaa_4x)
    {
        return MULTISAMPLE_4x;
    }
    else if (x == msaa_5x)
    {
        return MULTISAMPLE_5x;
    }
    else if (x == msaa_6x)
    {
        return MULTISAMPLE_6x;
    }
    else if (x == msaa_7x)
    {
        return MULTISAMPLE_7x;
    }
    else if (x == msaa_8x)
    {
        return MULTISAMPLE_8x;
    }
    else if (x == msaa_9x)
    {
        return MULTISAMPLE_9x;
    }
    else if (x == msaa_10x)
    {
        return MULTISAMPLE_10x;
    }
    else if (x == msaa_11x)
    {
        return MULTISAMPLE_11x;
    }
    else if (x == msaa_12x)
    {
        return MULTISAMPLE_12x;
    }
    else if (x == msaa_13x)
    {
        return MULTISAMPLE_13x;
    }
    else if (x == msaa_14x)
    {
        return MULTISAMPLE_14x;
    }
    else if (x == msaa_15x)
    {
        return MULTISAMPLE_15x;
    }
    else if (x == msaa_16x)
    {
        return MULTISAMPLE_16x;
    }
    else
    {
        debug_assert(0, "error: composer msaa.");
        return MULTISAMPLE_NONE;
    }
}

FX_RENDER_MODE FXComposerMapping::GetRenderMode(const String & x)
{
    if (x == rm_default)
        return FXRM_DEFAULT;
    else if (x == rm_rect)
        return FXRM_RECT;
    else
    {
        debug_assert(0, "error: composer render mode.");
        return FXRM_UNKNOWN;
    }
}

String FXComposerMapping::GetMSAAString(MULTI_SAMPLE x)
{
    switch (x)
    {
    case MULTISAMPLE_NONE:
        return msaa_none;

    case MULTISAMPLE_2x:
        return msaa_2x;

    case MULTISAMPLE_3x:
        return msaa_3x;

    case MULTISAMPLE_4x:
        return msaa_4x;

    case MULTISAMPLE_5x:
        return msaa_5x;

    case MULTISAMPLE_6x:
        return msaa_6x;

    case MULTISAMPLE_7x:
        return msaa_7x;

    case MULTISAMPLE_8x:
        return msaa_8x;

    case MULTISAMPLE_9x:
        return msaa_9x;

    case MULTISAMPLE_10x:
        return msaa_10x;

    case MULTISAMPLE_11x:
        return msaa_11x;

    case MULTISAMPLE_12x:
        return msaa_12x;

    case MULTISAMPLE_13x:
        return msaa_13x;

    case MULTISAMPLE_14x:
        return msaa_14x;

    case MULTISAMPLE_15x:
        return msaa_15x;
            
    case MULTISAMPLE_16x:
        return msaa_16x;

    default:
        debug_assert(0, "msaa type error.");
        return msaa_none;
    }
}

String FXComposerMapping::GetFormatString(FORMAT x)
{
    switch (x)
    {
    case FMT_R8G8B8:
        return r8g8b8;

    case FMT_A8R8G8B8:
        return a8r8g8b8;

    case FMT_X8R8G8B8:
        return x8r8g8b8;

    case FMT_A16B16G16R16:
        return a16b16g16r16;

    case FMT_A16B16G16R16F:
        return a16b16g16r16f;

    case FMT_R16F:
        return r16f;

    default:
        debug_assert(0, "error: composer not support this format render target.");
        return space;
    }
}

String FXComposerMapping::GetZFormatString(FORMAT x)
{
    switch (x)
    {
    case FMT_D15S1:
        return d15s1;

    case FMT_D16:
        return d16;

    case FMT_D24X4S4:
        return d24x4s4;

    case FMT_D24S8:
        return d24s8;

    case FMT_D32:
        return d32;

    default:
        debug_assert(0, "error: composer not support this format depth buffer.");
        return space;
    }
}

String FXComposerMapping::GetRenderModeString(FX_RENDER_MODE x)
{
    switch (x)
    {
    case FXRM_DEFAULT:
        return rm_default;

    case FXRM_RECT:
        return rm_rect;

    default:
        debug_assert(0, "error: unknow fx render mode.");
        return space;
    }
}


/*
------------------------------------------------------------------------------
    FXComposerImport
------------------------------------------------------------------------------
*/
void FXComposerImport::Import(FXComposer * com, const String & sFileName)
{
    DataStreamPtr stream = ResourceManager::Instance().OpenResource(sFileName);

    if (stream.IsNull())
    {
        EXCEPTION("Stream error: " + sFileName);
    }

    CommandScript cst;
    cst.LoadStream(stream);

    String cmd;

    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();

            if (render_target == cmd)
            {
                ParseRenderTarget(com, cst);
            }
            else if (step == cmd)
            {
                ParseStep(com, cst);
            }
        }
    }

    com->Initialize();
}

void FXComposerImport::ParseRenderTarget(FXComposer * com, CommandScript & cst)
{
    String cmd;
    String param;

    String _name = "unknown";
    FORMAT _format = FMT_UNKNOWN;
    int _width = 0;
    int _height = 0;
    MULTI_SAMPLE _msaa = MULTISAMPLE_NONE;

    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (bracket_b == cmd)
            {
                continue;
            }

            else if (name == cmd)
            {
                _name = param;
            }

            else if (format == cmd)
            {
                _format = FXComposerMapping::GetFormat(param);
            }

            else if (width == cmd)
            {
                _width = FXComposerMapping::GetWidth(param);
            }

            else if (height == cmd)
            {
                _height = FXComposerMapping::GetHeight(param);
            }

            else if (msaa == cmd)
            {
                _msaa = FXComposerMapping::GetMSAA(param);
            }

            else if (bracket_e == cmd)
            {
                break;
            }
        }
    }

    RenderTargetPtr target = VideoBufferManager::Instance().CreateRenderTarget(_name, 
                                                                               _width,
                                                                               _height,
                                                                               _format,
                                                                               _msaa);
    com->AddRenderTarget(target);
}

void FXComposerImport::ParseStep(FXComposer * com, CommandScript & cst)
{
    String cmd;
    String param;

    FXStep * _step = com->CreateStep();

    while (cst.MoveToNextLine())
    {
        cmd = cst.ParseCommand();
        param = cst.ParseStringParam();

        if (!cst.IsLineComment())
        {
            if (bracket_b == cmd)
            {
                continue;
            }

            else if (cmd == render_mode)
            {
                FX_RENDER_MODE mode = FXComposerMapping::GetRenderMode(param);
                _step->SetRenderMode(mode);
            }

            else if (cmd == render_mark)
            {
                int mark = param.ToInt();
                _step->SetRenderMark(mark);
            }

            else if (cmd == render_frame)
            {
                int frame = param.ToInt();
                _step->SetRenderFrame(frame);
            }

            else if (cmd == render_target0)
            {
                RenderTarget * target = com->GetRenderTarget(param);
                EXCEPTION_DEBUG(target, "can't find render target in composer.");
                _step->SetRenderTarget(0, target);
            }

            else if (cmd == render_target1)
            {
                RenderTarget * target = com->GetRenderTarget(param);
                EXCEPTION_DEBUG(target, "can't find render target in composer.");
                _step->SetRenderTarget(1, target);
            }

            else if (cmd == render_target2)
            {
                RenderTarget * target = com->GetRenderTarget(param);
                EXCEPTION_DEBUG(target, "can't find render target in composer.");
                _step->SetRenderTarget(2, target);
            }

            else if (cmd == render_target3)
            {
                RenderTarget * target = com->GetRenderTarget(param);
                EXCEPTION_DEBUG(target, "can't find render target in composer.");
                _step->SetRenderTarget(3, target);
            }

            else if (cmd == material)
            {
                _step->SetMaterial(param);
            }
           
            else if (bracket_e == cmd)
            {
                break;
            }
        }
    }
}



/*
------------------------------------------------------------------------------
    FXComposerExport
------------------------------------------------------------------------------
*/
void FXComposerExport::Export(FXComposer * com, const String & sFileName)
{
    File file;
    
    file.Open(sFileName);

    EXCEPTION_DEBUG(file.IsOpen(), sFileName + " can't open.");

    RenderTarget * tex;
    for (int i = 0; i < com->GetRenderTargetCount(); ++i)
    {
        tex = com->GetRenderTarget(i);
        WriteRenderTarget(tex, file);
    }

    FXStep * _step;
    for (int i = 0; i < com->GetStepCount(); ++i)
    {
        _step = com->GetStep(i);
        WriteStep(_step, i, file);
    }

    file.Close();
}

void FXComposerExport::WriteRenderTarget(RenderTarget * tex, File & file)
{
    file << render_target << File::ENDL
        << name_a << tex->GetName() << File::ENDL
        << format_a << FXComposerMapping::GetFormatString(tex->GetFormat()) << File::ENDL
        << width_a << String(tex->GetWidth()) << File::ENDL
        << height_a << String(tex->GetHeight()) << File::ENDL
        << msaa_a << FXComposerMapping::GetMSAAString(tex->GetMultiSampleType()) << File::ENDL
        << File::ENDL;
}

void FXComposerExport::WriteStep(FXStep * _step, int i, File & file)
{
    file << step << space
        << String(i) << File::ENDL; 
    
    file << bracket_b << File::ENDL;

    //render mode
    file << tab
        << render_mode_a
        << FXComposerMapping::GetRenderModeString(_step->GetRenderMode())
        << File::ENDL;

    //render mark
    file << tab
        << render_mark_a
        << String(_step->GetRenderMark())
        << File::ENDL;

    //render target0
    Texture * target = 0;
    target = _step->GetRenderTarget(0);
    if (target)
    {
        file << render_target0_a
            << target->GetName()
            << File::ENDL;
    }

    //render target1
    target = _step->GetRenderTarget(1);
    if (target)
    {
        file << render_target1_a
            << target->GetName()
            << File::ENDL;
    }

    //render target2
    target = _step->GetRenderTarget(2);
    if (target)
    {
        file << render_target2_a
            << target->GetName()
            << File::ENDL;
    }

    //render target3
    target = _step->GetRenderTarget(3);
    if (target)
    {
        file << render_target3_a
            << target->GetName()
            << File::ENDL;
    }

    //material
    file << material_a
        << _step->GetMaterial()->GetName()
        << File::ENDL;

    file << bracket_e << File::ENDL
        << File::ENDL;
}