#pragma once

#include "MWMaterial.h"


namespace Myway
{

//class MW_ENTRY MaterialLoader : public AllocObj
//{
//public:
//    MaterialLoader() {}
//    virtual ~MaterialLoader() {}
//
//    virtual MaterialPtr Load(const String & sName, const String & filename) = 0;
//    virtual void Save(MaterialPtr mtl, const String & filename) = 0;
//};
//
//class MW_ENTRY DefaultMaterialLoader: public MaterialLoader
//{
//public:
//    DefaultMaterialLoader();
//    virtual ~DefaultMaterialLoader();
//
//    virtual MaterialPtr Load(const String & sName, const String & filename);
//    virtual void Save(MaterialPtr mtl, const String & filename);
//
//protected:
//    void ReadTechnique(Technique * tech, CommandScript & cst);
//    void ReadPass(Pass * pass, CommandScript & cst);
//    void ReadTextureStage(TextureStage * stage, CommandScript & cst);
//    void ReadVertexShader(Pass * pass, CommandScript & cst);
//    void ReadPixelShader(Pass * pass, CommandScript & cst);
//    void ReadShaderParam(ShaderParam * spp, CommandScript & cst, const String & type);
//
//    void WriteTechinque(Technique * tech, File & file);
//    void WritePass(Pass * pass, File & file);
//    void WriteVertexShader(ShaderProgram * vs, File & file);
//    void WritePixelShader(ShaderProgram * ps, File & file);
//    void WriteTextureStage(TextureStage * stage, File & file);
//
//    SHADER_PARAMETER_TYPE GetType(const String & type);
//    AUTO_SHADER_PARAMETER_TYPE GetAutoType(const String & type);
//    bool IsCustom(const String & type);
//};

}