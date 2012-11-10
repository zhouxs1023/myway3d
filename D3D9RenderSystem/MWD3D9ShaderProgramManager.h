#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9ShaderProgram.h"
#include "MWShaderProgramManager.h"

namespace Myway
{

/* class: D3D9ShaderProgramManager
--------------------------------------------------------------------------------------
@Remark:
    Shader Program Manager For Direct3D9.
--------------------------------------------------------------------------------------
*/

class RS_ENTRY D3D9ShaderProgramManager : public ShaderProgramManager
{
public:
    D3D9ShaderProgramManager(IDirect3DDevice9 * device);
    ~D3D9ShaderProgramManager();


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
    
    
    ShaderProgram * LoadHLSLProgram(const TString128 & name,
                                    SHADER_PROFILE profile,
                                    const char * source, int size,
                                    const char * entry,
                                    const List<ShaderParamDesc> * defs);

    void LoadHLSLShaderParam(ShaderProgram * program,
                             LPD3DXCONSTANTTABLE table,
                             D3DXHANDLE parent,
                             const TString128 & prefix,
                             int index);

    ShaderProgram * LoadAsmProgram(const TString128 & name,
                                   SHADER_PROFILE profile,
                                   const char * source, int size,
                                   const List<ShaderParamDesc> * defs);

    ShaderProgram * LoadPreCompiledProgram(const TString128 & name,
                                           SHADER_PROFILE profile,
                                           const char * source, int size,
                                           const List<ShaderParamDesc> * defs);

    bool IsSupported(SHADER_PROFILE profile);


protected:
    IDirect3DDevice9 *          mdevice;
    CGcontext                   mcontext;
};



}