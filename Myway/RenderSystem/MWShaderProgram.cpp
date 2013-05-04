/* 
--------------------------------------------------------------------------------------
                        Myway Engine

        desc:       Shader program for render object
        author:     Myway
        email:      404793124@qq.com

--------------------------------------------------------------------------------------
*/


#include "MWShaderProgram.h"

using namespace Myway;


bool ShaderProgram::IsVertexShader(SHADER_PROFILE profile)
{
    return profile == SP_VS;
}

bool ShaderProgram::IsPixelShader(SHADER_PROFILE profile)
{
	return profile == SP_PS;
}

ShaderProgram::ShaderProgram(const TString128 & name, SHADER_PROFILE profile, SHADER_LANGUAGE language)
: mName(name),
  mProfile(profile),
  mLanguage(language)
{
}

ShaderProgram::ShaderProgram()
	: mName("")
	, mProfile(SP_UNKNOWN)
	, mLanguage(SL_UNKNOWN)
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::AddParam(const ShaderParamDesc & def)
{
    mParams.PushBack(def);
}

void ShaderProgram::GetParamTable(ShaderParamTable * table)
{
    table->Clear();
    Array<ShaderParamDesc>::Iterator whr = mParams.Begin();
    Array<ShaderParamDesc>::Iterator end = mParams.End();

    while (whr != end)
    {
        table->AddParam(whr->name, whr->type, whr->count, whr->reg, whr->btype, whr->bindex);

        ++whr;
    }

    whr = mParams.Begin();
    while (whr != end)
    {
        if (whr->btype == SPBT_UNKNOWN && whr->type == SPT_FLOAT4)
            table->GetParam(whr->name)->Set((float*)&(whr->val), 4 * sizeof(float));

        ++whr;
    }
}

ShaderParamDesc * ShaderProgram::GetParam(const TString32 & name)
{
    Array<ShaderParamDesc>::Iterator iter;
    Array<ShaderParamDesc>::Iterator end;

    iter = mParams.Begin();
    end = mParams.End();

    while (iter != end && iter->name != name)
    {
        ++iter;
    }

    return iter != end ? &(*iter) : NULL;
}

SHADER_PROFILE ShaderProgram::GetProfile() const
{
    return mProfile;
}

SHADER_LANGUAGE ShaderProgram::GetLanguage() const
{
    return mLanguage;
}

const TString128 & ShaderProgram::GetName() const
{
    return mName;
}