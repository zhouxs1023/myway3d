#include "MWFontLoader.h"
#include "MWFontManager.h"
#include "MWDynamicTTFont.h"
#include "MWResourceManager.h"

using namespace Myway;

#define _dynamic_ttf        "dynamic_ttf"
#define _texture_size       "texture_size"
#define _char_size          "char_size"
#define _resolution         "resolution"
#define _file               "file"
#define _maxwidth           "max_width"
#define _maxheight          "max_height"
#define _maxhoriBearingY    "max_horiBearingY"
#define _auto_compueInfo    "auto_compute"
#define _range              "range"
#define _begin              "{"
#define _end                "}"


DefaultFontLoader::DefaultFontLoader()
{
}

DefaultFontLoader::~DefaultFontLoader()
{
}

Font * DefaultFontLoader::Load(const String & filename)
{
    DataStreamPtr stream = ResourceManager::Instance().OpenResource(filename);

    assert(stream.NotNull());

    CommandScript cst;

    cst.LoadStream(stream);

    String cmd;
    String param;

    Font * font = NULL;

    LOG_PRINT(String("************* Parse Font: ") + filename + "*************\n");

    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (_dynamic_ttf == cmd)
            {
                assert(!font);
                font = FontManager::Instance().CreateFont(param, FONT_DYNAMIC_TTF);
                ParseDynamicTTFont((DynamicTTFont*)font, cst);
                font->Load();
            }
            else if ("" == cmd)
            {
            }
            else
            {
                LOG_PRINT("Unknow command: " + cmd + "\n");
            }
        }
    }

    LOG_PRINT("************* Parse Font end *************\n");

    return font;
}

void DefaultFontLoader::ParseDynamicTTFont(DynamicTTFont * font, CommandScript & cst)
{
    String cmd;
    String param1;
    String param2;

    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param1 = cst.ParseStringParam();
            param2 = cst.ParseStringParam();

            if (_begin == cmd)
            {
            } 

            else if (_end == cmd)
            {
                break;
            }

            else if (_texture_size == cmd)
            {
                font->SetTextureSize(param1.ToInt());
            }

            else if (_char_size == cmd)
            {
                font->SetCharSize(param1.ToInt());
            }

            else if (_resolution == cmd)
            {
                font->SetResolution(param1.ToInt());
            }

            else if (_file == cmd)
            {
                font->SetSource(param1);
            }

            else if (_maxwidth == cmd)
            {
                font->SetMaxWidth(param1.ToInt());
            }

            else if (_maxheight == cmd)
            {
                font->SetMaxHeight(param1.ToInt());
            }

            else if (_maxhoriBearingY == cmd)
            {
                font->SetMaxHoriBearingY(param1.ToInt());
            }

            else if (_auto_compueInfo == cmd)
            {
                font->SetAutoComputerInfo(param1.Tobool());
            }

            else if (_range == cmd)
            {
                font->AddCharRange(param1.ToInt(), param2.ToInt());
            }

            else if ("" == cmd)
            {

            }

            else
            {
                LOG_PRINT("Unknow command: " + cmd + "\n");
            }
            
        } // if (!cst.IsLineComment())

    } // while (cst.MoveToNextLine())
}