#pragma once

#include "MWShaderNode.h"


namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Node_ConstFloat3: public ST_Node
{
public:
    ST_Node_ConstFloat3(ST_Instance * parent, const String & name);
    virtual ~ST_Node_ConstFloat3();

    ST_NODE_TYPE    GetType() const;
    ST_VALUE_TYPE   GetValueType() const;
    String          Generte();
    void            NotifyDestroy(ST_Node * node);
    String          GetUniformParamDefine();

    void            SetValue(const Vec3 & v);
    const Vec3 &    GetValue() const;

protected:
    String          m_code;
    Vec3            m_value;
};

}

}