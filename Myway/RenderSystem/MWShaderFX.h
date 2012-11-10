#pragma once

#include "MWShaderProgram.h"
#include "MWShaderProvider.h"

namespace Myway
{

class ShaderLib;

class MW_ENTRY Technique
{
    DECLARE_ALLOC();

public:
    Technique(const TString128 & name, ShaderLib * fx);
    ~Technique();

    const TString128 & GetName();
    ShaderLib * GetShaderLib();

    void SetVertexShader(const TString128 & name);
    void SetPixelShader(const TString128 & name);

    ShaderProgram * GetVertexShader();
    ShaderProgram * GetPixelShader();

    ShaderParamTable * GetVertexShaderParamTable();
    ShaderParamTable * GetPixelShaderParamTable();

    void SetSamplerState(int index, const SamplerState & state);
    SamplerState & GetSamplerState(int index);

protected:
    ShaderLib * mShaderLib;

    TString128 mName;
    ShaderParamTable mVertexShaderParamTable;
    ShaderParamTable mPixelShaderParamTable;
    ShaderProgram * mVertexShader;
    ShaderProgram * mPixelShader;
    SamplerState mSamplerStates[MAX_SAMPLER_STATE];
};





class MW_ENTRY ShaderLib
{
    DECLARE_ALLOC();

public:
    ShaderLib(const TString128 & name);
    ~ShaderLib();

    const TString128 & GetName();

    ShaderProgram * AddShader(const TString128 & name,
                              SHADER_LANGUAGE language,
                              SHADER_PROFILE profile,
                              const TString128 & source, 
                              const TString128 & entry,
                              const List<ShaderParamDesc> * params);
    void RemoveShader(const TString128 & name);
    ShaderProgram * GetShader(const TString128 & name);
    ShaderProgram * GetShader(int index);
    int GetShaderCount();

    Technique * AddTechnique(const TString128 & name);
    void RemoveTechnique(const TString128 & name);
    void RemoveTechnique(int index);

    int GetTechniqueCount() const;
    Technique * GetTechnique(int index);

    Technique * GetTechnique(const TString128 & tech);

protected:
    TString128 mName;
    Array<ShaderProgram*> mShaders;
    Array<Technique*>  mTechniques;
};

class ShaderLibManager
{
    DECLARE_ALLOC();
    DECLARE_SINGLETON(ShaderLibManager);

public:
    ShaderLibManager();
    ~ShaderLibManager();

    ShaderLib * LoadShaderLib(const TString128 & name, const TString128 & source, const TString128 & group);
    ShaderLib * CreateShaderLib(const TString128 & name);
    ShaderLib * GetShaderLib(const TString128 & name);
    ShaderLib * GetShaderLib(int index);
    int GetShaderLibCount();

protected:
    Array<ShaderLib *> mShaderLibs;
};

}