#include "MWShaderNodeConstFloat3.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;

ST_Node_ConstFloat3::ST_Node_ConstFloat3(ST_Instance * parent, const String & name)
: ST_Node(parent, name),
  m_value(0)
{
}

ST_Node_ConstFloat3::~ST_Node_ConstFloat3()
{
}

ST_NODE_TYPE ST_Node_ConstFloat3::GetType() const
{
    return STNODE_CONST_FLOAT3;
}

ST_VALUE_TYPE ST_Node_ConstFloat3::GetValueType() const
{
    return STVAL_FLOAT3;
}

String ST_Node_ConstFloat3::Generte()
{
    assert (!IsGenerated());

    m_generted = TRUE;
    return ST_Code_Factory::Instance().GenerteCode(this);
}

void ST_Node_ConstFloat3::NotifyDestroy(ST_Node * node)
{
}

String ST_Node_ConstFloat3::GetUniformParamDefine()
{
    return "";
}

void ST_Node_ConstFloat3::SetValue(const Vec3 & v)
{
    m_value = v;
}

const Vec3 & ST_Node_ConstFloat3::GetValue() const
{
    return m_value;
}