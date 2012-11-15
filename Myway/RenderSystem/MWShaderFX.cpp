#include "MWShaderFX.h"
#include "MWShaderProgramManager.h"
#include "MWShaderFXLoader.h"
#include "MWResourceManager.h"

namespace Myway
{

//
// Technique
//
Technique::Technique(const TString128 & name, ShaderLib * fx)
: mName(name), mShaderLib(fx)
{
}

Technique::~Technique()
{
}

const TString128 & Technique::GetName()
{
    return mName;
}

ShaderLib * Technique::GetShaderLib()
{
    return mShaderLib;
}

void Technique::SetVertexShader(const TString128 & name)
{
    mVertexShader = mShaderLib->GetShader(name);

    d_assert (mVertexShader && ShaderProgram::IsVertexShader(mVertexShader->GetProfile()));

    mVertexShader->GetParamTable(&mVertexShaderParamTable);
}

void Technique::SetPixelShader(const TString128 & name)
{
    mPixelShader = mShaderLib->GetShader(name);

    d_assert (mPixelShader && ShaderProgram::IsPixelShader(mPixelShader->GetProfile()));

    mPixelShader->GetParamTable(&mPixelShaderParamTable);
}

ShaderProgram * Technique::GetVertexShader()
{
    return mVertexShader;
}

ShaderProgram * Technique::GetPixelShader()
{
    return mPixelShader;
}

ShaderParamTable * Technique::GetVertexShaderParamTable()
{
    return &mVertexShaderParamTable;
}

ShaderParamTable * Technique::GetPixelShaderParamTable()
{
    return &mPixelShaderParamTable;
}

void Technique::SetSamplerState(int index, const SamplerState & state)
{
    d_assert (index < MAX_SAMPLER_STATE);

    mSamplerStates[index] = state;
}

SamplerState & Technique::GetSamplerState(int index)
{
    d_assert (index < MAX_SAMPLER_STATE);

    return mSamplerStates[index];
}



















//
// ShaderLib
//
ShaderLib::ShaderLib(const TString128 & name)
: mName(name)
{
}

ShaderLib::~ShaderLib()
{
    Technique * tech = NULL;

    for (int i = 0; i < mTechniques.Size(); ++i)
    {
        tech = mTechniques[i];
        delete tech;
    }

    ShaderProgram * prog = NULL;

    for (int i = 0; i < mShaders.Size(); ++i)
    {
        prog = mShaders[i];
        delete prog;
    }
}

const TString128 & ShaderLib::GetName()
{
    return mName;
}


Technique * ShaderLib::AddTechnique(const TString128 & name)
{
    Technique * tech = new Technique(name, this);

    mTechniques.PushBack(tech);

    return tech;
}

void ShaderLib::RemoveTechnique(const TString128 & name)
{
    Technique * tech = NULL;

    for (int i = 0; i < mTechniques.Size(); ++i)
    {
        tech = mTechniques[i];

        if (tech->GetName() == name)
        {
            delete tech;
            mTechniques.Erase(i);
            break;
        }
    }
}

void ShaderLib::RemoveTechnique(int index)
{
    Technique * tech = NULL;
    tech = mTechniques[index];
    delete tech;
    mTechniques.Erase(index);
}


int ShaderLib::GetTechniqueCount() const
{
    return mTechniques.Size();
}

Technique * ShaderLib::GetTechnique(int index)
{
    return mTechniques[index];
}

Technique * ShaderLib::GetTechnique(const TString128 & name)
{
    Technique * tech = NULL;

    for (int i = 0; i < mTechniques.Size(); ++i)
    {
        tech = mTechniques[i];

        if (tech->GetName() == name)
        {
            return tech;
        }
    }

    return NULL;
}

ShaderProgram * ShaderLib::AddShader(const TString128 & name,
                                    SHADER_LANGUAGE language,
                                    SHADER_PROFILE profile,
                                    const TString128 & source, 
                                    const TString128 & entry,
                                    const List<ShaderParamDesc> * params)
{
    ShaderProgram * shader = ShaderProgramManager::Instance()->CreateShader(name, language, profile, source, entry, params);
    
    mShaders.PushBack(shader);

    return shader;
}

void ShaderLib::RemoveShader(const TString128 & name)
{
    for (int i = 0; i < mShaders.Size(); ++i)
    {
        ShaderProgram * shader = mShaders[i];

        if (shader->GetName() == name)
        {
            ShaderProgramManager::Instance()->Destroy(shader);
            mShaders.Erase(i);
            break;
        }
    }
}

ShaderProgram * ShaderLib::GetShader(const TString128 & name)
{
    for (int i = 0; i < mShaders.Size(); ++i)
    {
        ShaderProgram * shader = mShaders[i];

        if (shader->GetName() == name)
        {
            return shader;
        }
    }

    return NULL;
}

ShaderProgram * ShaderLib::GetShader(int index)
{
    return mShaders[index];
}

int ShaderLib::GetShaderCount()
{
    return mShaders.Size();
}














IMP_SLN(ShaderLibManager);
ShaderLibManager::ShaderLibManager()
{
    INIT_SLN;
}

ShaderLibManager::~ShaderLibManager()
{
    SHUT_SLN;

    for (int i = 0; i < mShaderLibs.Size(); ++i)
    {
        delete mShaderLibs[i];
    }
}

ShaderLib * ShaderLibManager::LoadShaderLib(const TString128 & name, const TString128 & source)
{
    ShaderLib * lib = GetShaderLib(name);

    if (lib == NULL)
    {
        DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source.c_str());

        if (stream == NULL)
        {
            LOG_PRINT_FORMAT ("%s can't open!", source.c_str());
            return NULL;
        }

        lib = new ShaderLib(name);
        mShaderLibs.PushBack(lib);
        ShaderLibLoader::Load(lib, stream);
    }

    return lib;
}

ShaderLib * ShaderLibManager::CreateShaderLib(const TString128 & name)
{
    ShaderLib * lib = GetShaderLib(name);

    if (lib)
        return NULL;

    lib = new ShaderLib(name);

    mShaderLibs.PushBack(lib);

    return lib;
}

ShaderLib * ShaderLibManager::GetShaderLib(const TString128 & name)
{
    for (int i = 0; i < mShaderLibs.Size(); ++i)
    {
        if (mShaderLibs[i]->GetName() == name)
            return mShaderLibs[i];
    }

    return NULL;
}

ShaderLib * ShaderLibManager::GetShaderLib(int index)
{
    return mShaderLibs[index];
}

int ShaderLibManager::GetShaderLibCount()
{
    return mShaderLibs.Size();
}

}
