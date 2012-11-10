#pragma once

#include "MWShaderCodeGenerter.h"


namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Code_Factory : public Singleton<ST_Code_Factory>
{
public:
    ST_Code_Factory();
    ~ST_Code_Factory();

    void SetCodeGenerter(SHADER_PROGRAM_LANGUAGE lauguage);
    void GenerteShader(ST_Instance * inst);

    String GenerteCode(ST_Node * node);
    String GenerteUniformParamDefine(ST_Node * node);

protected:
    ST_Code_Generter *          m_generter;
};

}

}