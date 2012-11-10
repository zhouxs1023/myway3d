#pragma once

#include "MWShaderCodeGenerter.h"

namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Code_Generter_Cg : public ST_Code_Generter
{
public:
    ST_Code_Generter_Cg();
    virtual ~ST_Code_Generter_Cg();

    virtual void GenerteShader(ST_Instance * inst);
    virtual String GenerteCode(ST_Node * node);
    virtual String GenerteUniformParamDefine(ST_Node * node);

protected:
    void GenPixelShader(ST_Instance * inst);
    void GenPixelShaderParam(ST_Instance * inst, List<ShaderParamDefine> & params);

    void GenVertexShader(ST_Instance * inst);
    void GenVertexShaderParam(ST_Instance * inst, List<ShaderParamDefine> & params);

    String GenSampler2DCode(ST_Node * node);
    String GenAddCode(ST_Node * node);
    String GenSubCode(ST_Node * node);
    String GenMultiplyCode(ST_Node * node);
    String GenClampCode(ST_Node * node);
    String GenConstFloat1Code(ST_Node * node);
    String GenConstFloat2Code(ST_Node * node);
    String GenConstFloat3Code(ST_Node * node);
    String GenConstFloat4Code(ST_Node * node);
    String GenParamFloat4Code(ST_Node * node);


    String GenSampler2DUniform(ST_Node * node);
    String GenParamFloat4Uniform(ST_Node * node);
};

}

}