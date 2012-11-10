#pragma once

#include "MWShaderNode.h"


namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Node_ConstFloat1: public ST_Node
{
public:
    ST_Node_ConstFloat1(ST_Instance * parent, const String & name);
    virtual ~ST_Node_ConstFloat1();

    ST_NODE_TYPE    GetType() const;
    ST_VALUE_TYPE   GetValueType() const;
    String          Generte();
    void            NotifyDestroy(ST_Node * node);
    String          GetUniformParamDefine();

    void            SetValue(float v);
    float           GetValue() const;

protected:
    float           m_value;
};

}

}