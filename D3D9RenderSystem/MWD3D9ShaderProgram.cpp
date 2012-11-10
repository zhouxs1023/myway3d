/* 
--------------------------------------------------------------------------------------
                        Myway Engine

        desc:       Shader program For Direct3D 9.0
        author:     Myway
        email:      404793124@qq.com

--------------------------------------------------------------------------------------
*/

#include "MWD3D9ShaderProgram.h"
#include "MWD3D9ShaderProgramManager.h"

namespace Myway
{


D3D9VertexShader::D3D9VertexShader(IDirect3DDevice9 * device,
                                   const TString128 & name,
                                   SHADER_PROFILE profile,
                                   SHADER_LANGUAGE language,
                                   IDirect3DVertexShader9 * shader)
: ShaderProgram(name, profile, language),
  mDevice(device),
  mShader(shader)
{
}

D3D9VertexShader::~D3D9VertexShader()
{
    safe_release_com(mShader);
}

void D3D9VertexShader::DeleteSelf()
{
    ShaderProgramManager::Instance()->Destroy(this);
}





D3D9PixelShader::D3D9PixelShader(IDirect3DDevice9 * device,
                                 const TString128 & name,
                                 SHADER_PROFILE profile,
                                 SHADER_LANGUAGE language,
                                 IDirect3DPixelShader9 * shader)
: ShaderProgram(name, profile, language),
  mDevice(device),
  mShader(shader)
{
}

D3D9PixelShader::~D3D9PixelShader()
{
    safe_release_com(mShader);
}

void D3D9PixelShader::DeleteSelf()
{
    ShaderProgramManager::Instance()->Destroy(this);
}

}
