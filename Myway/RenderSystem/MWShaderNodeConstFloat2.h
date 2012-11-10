#pragma once

#include "MWShaderNode.h"


namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Node_ConstFloat2: public ST_Node
{
public:
    ST_Node_ConstFloat2(ST_Instance * parent, const String & name);
    virtual ~ST_Node_ConstFloat2();

    ST_NODE_TYPE    GetType() const;
    ST_VALUE_TYPE   GetValueType() const;
    String          Generte();
    void            NotifyDestroy(ST_Node * node);
    String          GetUniformParamDefine();

    void            SetValue(const Vec2 & v);
    const Vec2 &    GetValue() const;

protected:
    Vec2            m_value;
};

}

}