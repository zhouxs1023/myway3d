#pragma once

#include <cg\cgD3D11.h>
#include "MWD3D11ShaderProgram.h"
#include "MWShaderProgramManager.h"

namespace Myway {

class RS_ENTRY D3D11ShaderProgramManager : public ShaderProgramManager
{
public:
    D3D11ShaderProgramManager();
    ~D3D11ShaderProgramManager();


    ShaderProgram * CreateShader(const TString128 & name,
                                 SHADER_LANGUAGE language,
                                 SHADER_PROFILE profile,
                                 const TString128 & source,
                                 const TString128 & entry,
                                 const List<ShaderParamDesc> * defs);

    ShaderProgram * CreateShaderFromString(const TString128 & name,
                                  SHADER_LANGUAGE language,
                                  SHADER_PROFILE profile,
                                  const String & source, 
                                  const TString128 & entry,
                                  const List<ShaderParamDesc> * defs);

    void Destroy(ShaderProgram * program);


protected:
    ShaderProgram * LoadCgProgram(const TString128 & name,
                                  SHADER_PROFILE profile,
                                  const char * source, int size,
                                  const char * entry,
                                  const List<ShaderParamDesc> * defs);

    void LoadCgProgramCode(String & code, int size);

    void LoadCgProgramParam(ShaderProgram * program, CGparameter param, int count);

    bool IsParamValid(CGparameter param);

    SHADER_PARAM_TYPE GetCgType(CGtype type);
    
    bool IsSupported(SHADER_PROFILE profile);


protected:
    CGcontext mCgContext;
};



}