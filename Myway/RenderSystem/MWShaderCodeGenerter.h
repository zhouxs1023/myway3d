#pragma once

#include "MWShaderNode.h"
#include "MWShaderInstance.h"

namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Code_Generter : public AllocObj
{
public:
    ST_Code_Generter() {}
    virtual ~ST_Code_Generter() {}

    virtual void GenerteShader(ST_Instance * inst) = 0;
    virtual String GenerteCode(ST_Node * node) = 0;
    virtual String GenerteUniformParamDefine(ST_Node * node) = 0;
};

}


}