#include "MWShaderNodeConstFloat2.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;

ST_Node_ConstFloat2::ST_Node_ConstFloat2(ST_Instance * parent, const String & name)
: ST_Node(parent, name),
  m_value(0)
{
}

ST_Node_ConstFloat2::~ST_Node_ConstFloat2()
{
}

ST_NODE_TYPE ST_Node_ConstFloat2::GetType() const
{
    return STNODE_CONST_FLOAT2;
}

ST_VALUE_TYPE ST_Node_ConstFloat2::GetValueType() const
{
    return STVAL_FLOAT2;
}

String ST_Node_ConstFloat2::Generte()
{
    assert (!IsGenerated());

    m_generted = TRUE;
    return ST_Code_Factory::Instance().GenerteCode(this);
}

void ST_Node_ConstFloat2::NotifyDestroy(ST_Node * node)
{
}

String ST_Node_ConstFloat2::GetUniformParamDefine()
{
    return "";
}

void ST_Node_ConstFloat2::SetValue(const Vec2 & v)
{
    m_value = v;
}

const Vec2 & ST_Node_ConstFloat2::GetValue() const
{
    return m_value;
}