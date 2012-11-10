/* 
--------------------------------------------------------------------------------------
                        Myway Engine

        desc:       Shader program For Direct3D 9.0
        author:     Myway
        email:      404793124@qq.com

--------------------------------------------------------------------------------------
*/

#pragma once 

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWShaderProgram.h"

namespace Myway
{

/* 
------------------------------------------------------
    Vertex Shader.
------------------------------------------------------
 */

class RS_ENTRY D3D9VertexShader : public ShaderProgram
{
    DeclareRefObj();

public:
    D3D9VertexShader(IDirect3DDevice9 * device,
                     const TString128 & name,
                     SHADER_PROFILE profile,
                     SHADER_LANGUAGE language,
                     IDirect3DVertexShader9 * shader);

    ~D3D9VertexShader();

    IDirect3DVertexShader9 *        GetShader();

protected:
    IDirect3DDevice9 *              mDevice;
    IDirect3DVertexShader9 *        mShader;
};

inline IDirect3DVertexShader9 * D3D9VertexShader::GetShader()
{
    return mShader;
}


/* 
------------------------------------------------------
    Pixel Shader.
------------------------------------------------------
*/
class RS_ENTRY D3D9PixelShader : public ShaderProgram
{
    DeclareRefObj();

public:
    D3D9PixelShader(IDirect3DDevice9 * device,
                    const TString128 & name,
                    SHADER_PROFILE profile,
                    SHADER_LANGUAGE language,
                    IDirect3DPixelShader9 * shader);

    virtual ~D3D9PixelShader();

    IDirect3DPixelShader9 * GetShader() { return mShader; }

    friend class D3D9ShaderProgramManager;

protected:
    IDirect3DDevice9 *          mDevice;
    IDirect3DPixelShader9 *     mShader;
};

}
