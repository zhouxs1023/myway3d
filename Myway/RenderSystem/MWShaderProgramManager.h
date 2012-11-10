#pragma once


#include "MWShaderProgram.h"

namespace Myway
{

/* iterface: ShaderProgramManager
--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
*/
class MW_ENTRY ShaderProgramManager
{
    DECLARE_ALLOC();
    DECLARE_SINGLETON(ShaderProgramManager);

public:
    ShaderProgramManager();
    virtual ~ShaderProgramManager();

    virtual ShaderProgram *  CreateShader(const TString128 & name,
                                          SHADER_LANGUAGE language,
                                          SHADER_PROFILE profile,
                                          const TString128 & source, 
                                          const TString128 & entry,
                                          const List<ShaderParamDesc> * defs) = 0;

    virtual ShaderProgram *  CreateShaderFromString(const TString128 & name,
                                          SHADER_LANGUAGE language,
                                          SHADER_PROFILE profile,
                                          const String & source,
                                          const TString128 & entry,
                                          const List<ShaderParamDesc> * defs) = 0;

    virtual void Destroy(ShaderProgram * pProgram) = 0;
};

}
