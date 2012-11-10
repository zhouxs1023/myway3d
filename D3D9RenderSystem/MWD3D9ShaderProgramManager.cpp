#include "MWD3D9DeviceCaps.h"
#include "MWD3D9ShaderProgramManager.h"
#include "MWResourceManager.h"

#define _CgException(func, program)                 \
    if (cgGetError() != CG_NO_ERROR)                \
        EXCEPTION(Myway::TString128("Cg error: ") + \
                  "program \"" + program + "\", func \"" + #func + Myway::TString128("\" failed, desc: ") + cgGetErrorString(cgGetError()))

#define _StreamException(stream, name)              \
    if (stream.IsNull())                            \
        EXCEPTION(Myway::TString128("Stream error: ") + name + " can't open.")

namespace Myway
{


class D3D9Include : public ID3DXInclude
{
public:
    D3D9Include() {}
    ~D3D9Include() {}

    STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
    {
        DataStreamPtr stream = ResourceManager::Instance()->OpenResource(pFileName);

        if (stream.NotNull())
        {
            void * data = pool_alloc(stream->Size());

            Memcpy(data, stream->GetData(), stream->Size());

            *ppData = data;
            *pBytes = stream->Size();

            return D3D_OK;
        }

        return S_FALSE;
    }

    STDMETHOD(Close)(LPCVOID pData)
    {
        pool_free((void*)pData);

        return D3D_OK;
    }
};


/* class: D3D9ShaderProgramManager
--------------------------------------------------------------------------------------
@Remark:
    Shader Program Manager For Direct3D9.
--------------------------------------------------------------------------------------
*/
D3D9ShaderProgramManager::D3D9ShaderProgramManager(IDirect3DDevice9 * device)
: mdevice(device)
{
    mcontext = cgCreateContext();
}

D3D9ShaderProgramManager::~D3D9ShaderProgramManager()
{
    cgDestroyContext(mcontext);
}


ShaderProgram * D3D9ShaderProgramManager::CreateShader(const TString128 & name,
                                                        SHADER_LANGUAGE language,
                                                        SHADER_PROFILE profile,
                                                        const TString128 & source, 
                                                        const TString128 & entry,
                                                        const List<ShaderParamDesc> * defs)
{
    ShaderProgram * program = NULL;

    DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source.c_str());
    d_assert (stream != NULL);
    _StreamException(stream, source);

    if (!IsSupported(profile))
    {
        TString128 message = "Create shader failed, desc: your device isn't supported profile " + 
            D3D9Mapping::GetShaderProfileString(profile);
        EXCEPTION(message);
    }

    if (language == SL_HLSL)
    {
        program = LoadHLSLProgram(name, profile, (const char*)stream->GetData(), stream->Size(), entry.c_str(), defs);
    }
    else if (language == SL_CG)
    {
        program = LoadCgProgram(name, profile, (const char*)stream->GetData(), stream->Size(), entry.c_str(), defs);
    }
    else if (language == SL_ASM)
    {
        program = LoadAsmProgram(name, profile, (const char*)stream->GetData(), stream->Size(), defs);
    }
    else if (language == SL_PRECOMPILED)
    {
        program = LoadAsmProgram(name, profile, (const char*)stream->GetData(), stream->Size(), defs);
    }

    d_assert(program);

    return program;
}

ShaderProgram * D3D9ShaderProgramManager::CreateShaderFromString(const TString128 & name,
                                                                  SHADER_LANGUAGE language,
                                                                  SHADER_PROFILE profile,
                                                                  const String & source, 
                                                                  const TString128 & entry,
                                                                  const List<ShaderParamDesc> * defs)
{
    ShaderProgram * program = NULL;

    if (!IsSupported(profile))
    {
        TString128 message = "Create shader failed, desc: your device isn't supported profile " + 
            D3D9Mapping::GetShaderProfileString(profile);
        EXCEPTION(message);
    }

    if (language == SL_HLSL)
    {
        program = LoadHLSLProgram(name, profile, source.c_str(), source.Length(), entry.c_str(), defs);
    }
    else if (language == SL_CG)
    {
        program = LoadCgProgram(name, profile, source.c_str(), source.Length(), entry.c_str(), defs);
    }
    else if (language == SL_ASM)
    {
        program = LoadAsmProgram(name, profile, source.c_str(), source.Length(), defs);
    }
    else if (language == SL_PRECOMPILED)
    {
        program = LoadAsmProgram(name, profile, source.c_str(), source.Length(), defs);
    }

    d_assert(program);

    return program;
}

void D3D9ShaderProgramManager::Destroy(ShaderProgram * pProgram)
{
    delete pProgram;
}

ShaderProgram * D3D9ShaderProgramManager::LoadCgProgram(const TString128 & name,
                                                        SHADER_PROFILE profile,
                                                        const char * source, int size,
                                                        const char * entry,
                                                        const List<ShaderParamDesc> * defs)
{
    ShaderProgram * program = NULL;

    String func = source;

    //load program code
     LoadCgProgramCode(func, func.Length());

    //create program
    CGprofile cgprofile = D3D9Mapping::GetCgProfile(profile);
    CGprogram pro = cgCreateProgram(mcontext, CG_SOURCE, func.c_str(), cgprofile, entry, NULL);

    _CgException(cgCreateProgram, name);

    //create shader
    LPD3DXBUFFER code = NULL;
    LPD3DXBUFFER error = NULL;
    LPD3DXCONSTANTTABLE table = NULL;
    const char * str = cgGetProgramString(pro, CG_COMPILED_PROGRAM);

    HRESULT hr = D3DXAssembleShader(str, Strlen(str), NULL, NULL, 0, &code, &error);

    if (FAILED(hr))
    {
        TString128 message = "Assemble shader " + name + " error: " + (LPCSTR)error->GetBufferPointer();
        safe_release_com(error);
        EXCEPTION(message);
    }

    safe_release_com(error);

    //vertex shader
    if (ShaderProgram::IsVertexShader(profile))
    {
        LPDIRECT3DVERTEXSHADER9 shader = NULL;

        hr = mdevice->CreateVertexShader((const DWORD *)code->GetBufferPointer(), &shader);

        safe_release_com(code);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create vertex shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            safe_release_com(table);
            EXCEPTION(message);
        }

        program = new D3D9VertexShader(mdevice, name, profile, SL_CG, shader);
    }

    //pixel shader
    else if (ShaderProgram::IsPixelShader(profile))
    {
        LPDIRECT3DPIXELSHADER9 shader = NULL;

        hr = mdevice->CreatePixelShader((const DWORD *)code->GetBufferPointer(), &shader);

        safe_release_com(code);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create pixel shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            safe_release_com(table);
            EXCEPTION(message);
        }

        program = new D3D9PixelShader(mdevice, name, profile, SL_CG, shader);
    }

    assert (program);

    //shader const
    LoadCgProgramParam(program, cgGetFirstParameter(pro, CG_PROGRAM), 1);
    LoadCgProgramParam(program, cgGetFirstParameter(pro, CG_GLOBAL), 1);

    cgDestroyProgram(pro);

    //setup param
    if (defs)
    {
        List<ShaderParamDesc>::ConstIterator whr = defs->Begin();
        List<ShaderParamDesc>::ConstIterator end = defs->End();

        while (whr != end)
        {
            ShaderParamDesc * def = program->GetParam(whr->name);

            if (def)
            {
                def->btype = whr->btype;
                def->bindex = whr->bindex;
                def->val = whr->val;
            }
            else
            {
                LOG_PRINT_FORMAT("Shader error: param %s not exist in %s.\n", whr->name.c_str(), name.c_str());
            }

            ++whr;
        }
    }

    return program;
}

void D3D9ShaderProgramManager::LoadCgProgramCode(String & code, int size)
{
    do
    {
        int pos = 0, i = 0, j = 0;
        bool flag = true;
        String head;

        pos = code.Find("#include", pos);

        if (pos == NPOS)
            break;

        i = code.Find("\"", pos + 8);
        if (i == NPOS)
        {
            i = code.Find("<", pos + 8);
            flag = false;
        }

        if (i == NPOS)
            break;

        if (flag)
        {
            j = code.Find("\"", i + 1);
        }
        else
        {
            j = code.Find(">", i + i);
        }

        if (j == NPOS)
            break;

        head = code.SubStr(i + 1, j - i - 1);

        DataStreamPtr stream = ResourceManager::Instance()->OpenResource(head.c_str());

        if (stream.IsNull())
        {
            LOG_PRINT_FORMAT("Cg head file: '%s' can't open", head.c_str());
            break;
        }

        code.Erase(pos, j - pos + 1);

        code = code._Append(pos, (const char *)stream->GetData());

    } while (1);
}

void D3D9ShaderProgramManager::LoadCgProgramParam(ShaderProgram * program, CGparameter param, int count)
{
    while (param)
    {
        if (IsParamValid(param))
        {
            CGtype type = cgGetParameterType(param);

            if (type == CG_STRUCT)
            {
                LoadCgProgramParam(program, cgGetFirstStructParameter(param), 1);
            }
            else if (type == CG_ARRAY)
            {
                //1 - dimensional.
                LoadCgProgramParam(program, cgGetArrayParameter(param, 0), cgGetArraySize(param, 0));
            }
            else
            {
                TString32 name = cgGetParameterName(param);
                int reg = cgGetParameterResourceIndex(param);
                int len = name.Length();

                if (name[len - 3] == '[' &&
                    name[len - 2] == '0' &&
                    name[len - 1] == ']')
                {
                    name.Erase(len - 3, 3);
                }

                ShaderParamDesc def;
                def.name = name;
                def.reg = reg;
                def.count = count;
                def.type = GetCgType(type);

                assert (type != SPT_UNKNOWN);

                program->AddParam(def);
            }
        }

        param = cgGetNextParameter(param);
    }
}

bool D3D9ShaderProgramManager::IsParamValid(CGparameter param)
{
    CGtype type = cgGetParameterType(param);

#ifdef _DEBUG
    CGenum _isOut = cgGetParameterDirection(param);
    CGbool _isRef = cgIsParameterReferenced(param);
    const char * _name = cgGetParameterName(param);
    int bk = 0;
#endif

    return cgGetParameterVariability(param) == CG_UNIFORM &&
           type != CG_SAMPLER1D && type != CG_SAMPLER2D &&
           type != CG_SAMPLER3D && type != CG_SAMPLERCUBE &&
           type != CG_SAMPLERRECT &&
           cgGetParameterDirection(param) != CG_OUT && 
           cgIsParameterReferenced(param);
}

SHADER_PARAM_TYPE D3D9ShaderProgramManager::GetCgType(CGtype type)
{
    switch(type)
    {
    case CG_FLOAT:
    case CG_FLOAT1:
    case CG_HALF:
    case CG_HALF1:
    case CG_FLOAT2:
    case CG_HALF2:
    case CG_FLOAT3:
    case CG_HALF3:
    case CG_FLOAT4:
    case CG_HALF4:
        return SPT_FLOAT4;
        break;

    case CG_FLOAT3x2:
    case CG_HALF3x2:
    case CG_FLOAT3x3:
    case CG_HALF3x3:
    case CG_FLOAT3x4:
    case CG_HALF3x4:
        return SPT_MATRIX3x4;

    case CG_FLOAT4x4:
    case CG_HALF4x4:
        return SPT_MATRIX4x4;

    case CG_INT:
    case CG_INT1:
    case CG_INT2:
    case CG_INT3:
    case CG_INT4:
        return SPT_INT4;

    default:
        return SPT_UNKNOWN;
    }
}

ShaderProgram * D3D9ShaderProgramManager::LoadHLSLProgram(const TString128 & name,
                                                          SHADER_PROFILE profile,
                                                          const char * source, int size,
                                                          const char * entry,
                                                          const List<ShaderParamDesc> * defs)
{
    ShaderProgram * program = NULL;

    D3D9Include incl;
    HRESULT hr = D3D_OK;
    LPD3DXBUFFER code = NULL;
    LPD3DXBUFFER error = NULL;
    LPD3DXCONSTANTTABLE table = NULL;

    TString128 d3dprofile = D3D9Mapping::GetHLSLProfile(profile);

    hr = D3DXCompileShader(source,                      // source
                           size,                        // size
                           NULL,                        // defines
                           &incl,                       // include proc
                           entry,                       // entry
                           d3dprofile.c_str(),          // profile
                           0,                           // flag
                           &code,                       // shader code
                           &error,                      // error
                           &table);                     // const table

    //check error
    if (FAILED(hr))
    {
        TString128 message = "Compile shader " + name + " error: " + (const char *)error->GetBufferPointer();
        safe_release_com(error);
        safe_release_com(table);
        safe_release_com(code);
        EXCEPTION(message);
    }

    safe_release_com(error);

    //vertex shader
    if (ShaderProgram::IsVertexShader(profile))
    {
        EXCEPTION_DEBUG((int)profile <= DeviceCaps::Instance()->PixelShaderVersion(), "vertex shader is not support.");

        LPDIRECT3DVERTEXSHADER9 shader = NULL;

        hr = mdevice->CreateVertexShader((const DWORD *)code->GetBufferPointer(), &shader);

        safe_release_com(code);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create vertex shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            safe_release_com(table);
            EXCEPTION(message);
        }

        program = new D3D9VertexShader(mdevice, name, profile, SL_HLSL, shader);
    }

    //pixel shader
    else if (ShaderProgram::IsPixelShader(profile))
    {
        LPDIRECT3DPIXELSHADER9 shader = NULL;

        hr = mdevice->CreatePixelShader((const DWORD *)code->GetBufferPointer(), &shader);

        safe_release_com(code);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create pixel shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            safe_release_com(table);
            EXCEPTION(message);
        }

        program = new D3D9PixelShader(mdevice, name, profile, SL_HLSL, shader);
    }

    D3DXCONSTANTTABLE_DESC desc;
    table->GetDesc(&desc);

    for (UINT i = 0; i < desc.Constants; ++i)
    {
        LoadHLSLShaderParam(program, table, NULL, "", i);
    }

    safe_release_com(table);

    assert (program);

    //setup param
    if (defs)
    {
        List<ShaderParamDesc>::ConstIterator iter;
        List<ShaderParamDesc>::ConstIterator end;

        iter = defs->Begin();
        end = defs->End();

        while (iter != end)
        {
            ShaderParamDesc * def = program->GetParam(iter->name);

            if (def)
            {
                def->btype = iter->btype;
                def->bindex = iter->bindex;
                def->val = iter->val;
            }
            else
            {
                LOG_PRINT_FORMAT("Shader error: param %s not exist in %s.\n", iter->name.c_str(), name.c_str());
            }

            ++iter;
        }
    }

    return program;
}

void D3D9ShaderProgramManager::LoadHLSLShaderParam(ShaderProgram * program, 
                                                   LPD3DXCONSTANTTABLE table,
                                                   D3DXHANDLE parent,
                                                   const TString128 & prefix,
                                                   int index)
{
    D3DXHANDLE handle = table->GetConstant(parent, index);

    D3DXCONSTANT_DESC desc;
    UINT num = 1;
    HRESULT hr;

    hr = table->GetConstantDesc(handle, &desc, &num);

    D3DErrorExceptionFunction(GetConstantDesc, hr);

    TString128 name = desc.Name;

    if (name[0] == '$')
    {
        name.Erase(0, 1);
    }

    int length = name.Length();

    //array
    if (name[length - 3] == '[' &&
        name[length - 2] == '0' &&
        name[length - 1] == ']')
    {
        name.Erase(length - 3, 3);
    }

    if (desc.Class == D3DXPC_STRUCT)
    {
        name = prefix + name + ".";

        for (UINT i = 0; i < desc.StructMembers; ++i)
        {
            LoadHLSLShaderParam(program, table, handle, name, i);
        }
    }
    else
    {
        if (desc.Type == D3DXPT_FLOAT || desc.Type == D3DXPT_INT)
        {
            ShaderParamDesc def;
            def.name = name.c_str();
            def.count = desc.Elements;
            def.reg = desc.RegisterIndex;

            switch(desc.Type)
            {
            case D3DXPT_FLOAT:
                if (desc.Rows == 1)
                {
                    def.type = SPT_FLOAT4;
                }
                else if (desc.Rows == 3)
                {
                    def.type = SPT_MATRIX3x4;
                }
                else if (desc.Rows == 4)
                {
                    def.type = SPT_MATRIX4x4;
                }
                break;

            default:
                d_assert (0 && "Unsupport type")
                break;
            }

            program->AddParam(def);
        }
    }
}

ShaderProgram * D3D9ShaderProgramManager::LoadAsmProgram(const TString128 & name,
                                                         SHADER_PROFILE profile,
                                                         const char * source, int size,
                                                         const List<ShaderParamDesc> * defs)
{
    ShaderProgram * program = NULL;

    LPD3DXBUFFER code = NULL;
    LPD3DXBUFFER error = NULL;

    HRESULT hr = D3DXAssembleShader(source, size, NULL, NULL, 0, &code, &error);

    if (FAILED(hr))
    {
        TString128 message = "Assemble shader " + name + " error: " + (LPCSTR)error->GetBufferPointer();
        safe_release_com(error);
        EXCEPTION(message);
    }

    safe_release_com(error);

    //vertex shader
    if (ShaderProgram::IsVertexShader(profile))
    {
        LPDIRECT3DVERTEXSHADER9 shader = NULL;

        hr = mdevice->CreateVertexShader((const DWORD *)code->GetBufferPointer(), &shader);

        safe_release_com(code);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create vertex shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            EXCEPTION(message);
        }

        program = new D3D9VertexShader(mdevice, name, profile, SL_ASM, shader);
    }

    //pixel shader
    else if (ShaderProgram::IsPixelShader(profile))
    {
        LPDIRECT3DPIXELSHADER9 shader = NULL;

        hr = mdevice->CreatePixelShader((const DWORD *)code->GetBufferPointer(), &shader);

        safe_release_com(code);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create pixel shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            EXCEPTION(message);
        }

        program = new D3D9PixelShader(mdevice, name, profile, SL_ASM, shader);
    }

    assert (program);

    //add param.
    if (defs)
    {
        List<ShaderParamDesc>::ConstIterator iter;
        List<ShaderParamDesc>::ConstIterator end;

        iter = defs->Begin();
        end = defs->End();

        while (iter != end)
        {
            program->AddParam(*iter);
            ++iter;
        }
    }
   
    return program;
}

ShaderProgram * D3D9ShaderProgramManager::LoadPreCompiledProgram(const TString128 & name,
                                                                 SHADER_PROFILE profile,
                                                                 const char * source, int size,
                                                                 const List<ShaderParamDesc> * defs)
{
    HRESULT hr = D3D_OK;
    ShaderProgram * program = NULL;

    //vertex shader
    if (ShaderProgram::IsVertexShader(profile))
    {
        LPDIRECT3DVERTEXSHADER9 shader = NULL;

        hr = mdevice->CreateVertexShader((const DWORD *)source, &shader);

        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create vertex shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            EXCEPTION(message);
        }

        program = new D3D9VertexShader(mdevice, name, profile, SL_PRECOMPILED, shader);
    }

    //pixel shader
    else if (ShaderProgram::IsPixelShader(profile))
    {
        LPDIRECT3DPIXELSHADER9 shader = NULL;

        hr = mdevice->CreatePixelShader((const DWORD *)source, &shader);


        //check error
        if (FAILED(hr))
        {
            TString128 message = "Create pixel shader " + name + " error, desc: " + D3D9Mapping::GetD3DErrorDescription(hr);
            EXCEPTION(message);
        }

        program = new D3D9PixelShader(mdevice, name, profile, SL_PRECOMPILED, shader);
    }

    assert (program);

    //add param.
    if (defs)
    {
        List<ShaderParamDesc>::ConstIterator iter;
        List<ShaderParamDesc>::ConstIterator end;

        iter = defs->Begin();
        end = defs->End();

        while (iter != end)
        {
            program->AddParam(*iter);
            ++iter;
        }
    }

    return program;
}

bool D3D9ShaderProgramManager::IsSupported(SHADER_PROFILE profile)
{
    if (ShaderProgram::IsVertexShader(profile))
    {
        return D3D9DeviceCaps::Instance()->VertexShaderVersion() >= profile;
    }
    else if (ShaderProgram::IsPixelShader(profile))
    {
        return D3D9DeviceCaps::Instance()->PixelShaderVersion() >= profile;
    }
    else
    {
        return false;
    }
}

}
