#pragma once

#include "MWShaderParam.h"

namespace Myway
{

struct ShaderProgramDefine : public AllocObj
{
    String                      name;
    String                      source;
    String                      entry;
    SHADER_PROGRAM_PROFILE      profile;
    SHADER_PROGRAM_LANGUAGE     language;
    List<ShaderParamDefine>     params;
};

class MW_ENTRY ShaderProgramDefMgr : public Singleton<ShaderProgramDefMgr>
{
public:
    ShaderProgramDefMgr();
    ~ShaderProgramDefMgr();

    void Load(const String & sFile, bool bAppend = FALSE);
    void Unload();
    const ShaderProgramDefine * GetShaderProgramDefine(const String & name);

protected:
    void ParseDefine(CommandScript & cst);
    void ParseParam(ShaderParamDefine & def, CommandScript & cst);
    SHADER_PROGRAM_PROFILE GetProfile(const String & s);
    SHADER_PROGRAM_LANGUAGE GetLanguage(const String & s);

protected:
    List<ShaderProgramDefine*> m_PromgramDefines;
};

}