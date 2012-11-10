#pragma once

#include "MWShaderNode.h"


namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Node_ParamFloat4: public ST_Node
{
public:
    ST_Node_ParamFloat4(ST_Instance * parent, const String & name);
    virtual ~ST_Node_ParamFloat4();

    const String &  GetVarName() const;

    ST_NODE_TYPE    GetType() const;
    ST_VALUE_TYPE   GetValueType() const;
    String          Generte();
    void            NotifyDestroy(ST_Node * node);
    String          GetUniformParamDefine();

    void            SetValue(const Vec4 & v);
    const Vec4 &    GetValue() const;

protected:
    String          m_code;
    Vec4            m_value;
};

}

}