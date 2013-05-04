#include "MWD3D11ShaderProgramManager.h"
#include "MWResourceManager.h"
#include "MWD3D11Driver.h"
#include "MWD3D11ShaderProgram.h"


#define _CgException(func, program)                 \
    if (cgGetError() != CG_NO_ERROR)                \
        EXCEPTION(Myway::TString128("Cg error: ") + \
                  "program \"" + program + "\", func \"" + #func + Myway::TString128("\" failed, desc: ") + cgGetErrorString(cgGetError()))

#define _StreamException(stream, name)              \
    if (stream.IsNull())                            \
        EXCEPTION(Myway::TString128("Stream error: ") + name + " can't open.")

namespace Myway {

static TString128 gLoadCurrentPath;


/* class: D3D11ShaderProgramManager
--------------------------------------------------------------------------------------
@Remark:
    Shader Program Manager For Direct3D9.
--------------------------------------------------------------------------------------
*/
D3D11ShaderProgramManager::D3D11ShaderProgramManager()
{
    mCgContext = cgCreateContext();
	cgD3D11SetDevice(mCgContext, D3D11Driver::Instance()->Device());
}

D3D11ShaderProgramManager::~D3D11ShaderProgramManager()
{
	cgD3D11SetDevice(mCgContext, NULL);
    cgDestroyContext(mCgContext);
}


ShaderProgram * D3D11ShaderProgramManager::CreateShader(const TString128 & name,
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

	gLoadCurrentPath = File::GetFileDir(source) + "\\";

    d_assert (IsSupported(profile));

	if (language == SL_CG)
    {
        program = LoadCgProgram(name, profile, (const char*)stream->GetData(), stream->Size(), entry.c_str(), defs);
    }
    else
	{
		d_assert (0 && "not support!");
	}

    d_assert(program);

    return program;
}

ShaderProgram * D3D11ShaderProgramManager::CreateShaderFromString(const TString128 & name,
                                                                  SHADER_LANGUAGE language,
                                                                  SHADER_PROFILE profile,
                                                                  const String & source, 
                                                                  const TString128 & entry,
                                                                  const List<ShaderParamDesc> * defs)
{
    ShaderProgram * program = NULL;

	d_assert (IsSupported(profile));


    if (language == SL_CG)
    {
        program = LoadCgProgram(name, profile, source.c_str(), source.Length(), entry.c_str(), defs);
    }
	else
	{
		d_assert (0 && "not support!");
	}

    d_assert(program);

    return program;
}

void D3D11ShaderProgramManager::Destroy(ShaderProgram * pProgram)
{
    delete pProgram;
}

ShaderProgram * D3D11ShaderProgramManager::LoadCgProgram(const TString128 & name,
                                                        SHADER_PROFILE profile,
                                                        const char * source, int size,
                                                        const char * entry,
                                                        const List<ShaderParamDesc> * defs)
{
	ID3D11Device * device = D3D11Driver::Instance()->Device();

    ShaderProgram * program = NULL;

    String func = source;

    //load program code
     LoadCgProgramCode(func, func.Length());

    //create program
    CGprofile cgprofile = D3D11Helper::GetCgProfile(profile);
    CGprogram pro = cgCreateProgram(mCgContext, CG_SOURCE, func.c_str(), cgprofile, entry, NULL);

    _CgException(cgCreateProgram, name);

	HRESULT hr = S_OK;

    //create shader
	hr = cgD3D11LoadProgram(pro, 0);

	ID3D10Blob * pMicroCode = cgD3D11GetCompiledProgram(pro);
	ID3D10Blob * pError = cgD3D11GetProgramErrors(pro);

    if (FAILED(hr))
    {
        TString128 message = "Assemble shader " + name + " error: " + (LPCSTR)pError->GetBufferPointer();
        safe_release (pError);
        EXCEPTION(message);
    }

    safe_release (pError);

    //vertex shader
    if (ShaderProgram::IsVertexShader(profile))
    {
        ID3D11VertexShader * shader = NULL;

        hr = device->CreateVertexShader(
			pMicroCode->GetBufferPointer(),
			pMicroCode->GetBufferSize(),
			NULL, &shader);

        d_assert (SUCCEEDED(hr));

		D3D11_VertexShader * pVertexShader = new D3D11_VertexShader;

		pVertexShader->mName = name;
		pVertexShader->mProfile = profile;
		pVertexShader->mLanguage = SL_CG;
		pVertexShader->mD3D11VertexShader = shader;
		pVertexShader->mD3D11MicroCode = pMicroCode;

		pMicroCode->AddRef();

		program = pVertexShader;

    }

    //pixel shader
    else if (ShaderProgram::IsPixelShader(profile))
    {
		ID3D11PixelShader * shader = NULL;

		hr = device->CreatePixelShader(
			pMicroCode->GetBufferPointer(),
			pMicroCode->GetBufferSize(),
			NULL, &shader);

        d_assert (SUCCEEDED(hr));

		D3D11_PixelShader * pPixelShader = new D3D11_PixelShader;

		pPixelShader->mName = name;
		pPixelShader->mProfile = profile;
		pPixelShader->mLanguage = SL_CG;
		pPixelShader->mD3D11PixelShader = shader;
		pPixelShader->mD3D11MicroCode = pMicroCode;

		pMicroCode->AddRef();

		program = pPixelShader;
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

void D3D11ShaderProgramManager::LoadCgProgramCode(String & code, int size)
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

		head = gLoadCurrentPath.c_str() + head;
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

void D3D11ShaderProgramManager::LoadCgProgramParam(ShaderProgram * program, CGparameter param, int count)
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

bool D3D11ShaderProgramManager::IsParamValid(CGparameter param)
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

SHADER_PARAM_TYPE D3D11ShaderProgramManager::GetCgType(CGtype type)
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

bool D3D11ShaderProgramManager::IsSupported(SHADER_PROFILE profile)
{
    return true;
}

}
