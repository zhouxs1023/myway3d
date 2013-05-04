/* 
--------------------------------------------------------------------------------------
                        Myway Engine

        desc:       Shader program for render object
        author:     Myway
        email:      404793124@qq.com

--------------------------------------------------------------------------------------
*/


#pragma once

#include "MWShaderParam.h"

namespace Myway
{

/* iterface
--------------------------------------------------------------------------------------
     ShaderProgram
--------------------------------------------------------------------------------------
*/

class MW_ENTRY ShaderProgram
{
    DECLARE_ALLOC();
public:
    static bool IsVertexShader(SHADER_PROFILE profile);
    static bool IsPixelShader(SHADER_PROFILE profile);

public:
    ShaderProgram(const TString128 & name, SHADER_PROFILE profile, SHADER_LANGUAGE language);
	ShaderProgram();
    virtual ~ShaderProgram();

    void AddParam(const ShaderParamDesc & desc);
    void GetParamTable(ShaderParamTable * table);
    ShaderParamDesc * GetParam(const TString32 & sName);

    SHADER_PROFILE GetProfile() const;
    SHADER_LANGUAGE GetLanguage() const;
    const TString128 & GetName() const;

protected:
    SHADER_PROFILE mProfile;
    SHADER_LANGUAGE mLanguage;
    TString128 mName;
    Array<ShaderParamDesc> mParams;
};

}