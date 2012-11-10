#include "MWShaderNodeConstFloat1.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;

ST_Node_ConstFloat1::ST_Node_ConstFloat1(ST_Instance * parent, const String & name)
: ST_Node(parent, name),
  m_value(0)
{
}

ST_Node_ConstFloat1::~ST_Node_ConstFloat1()
{
}

ST_NODE_TYPE ST_Node_ConstFloat1::GetType() const
{
    return STNODE_CONST_FLOAT1;
}

ST_VALUE_TYPE ST_Node_ConstFloat1::GetValueType() const
{
    return STVAL_FLOAT1;
}

String ST_Node_ConstFloat1::Generte()
{
    assert (!IsGenerated());

    m_generted = TRUE;
    return ST_Code_Factory::Instance().GenerteCode(this);
}

void ST_Node_ConstFloat1::NotifyDestroy(ST_Node * node)
{
}

String ST_Node_ConstFloat1::GetUniformParamDefine()
{
    return "";
}

void ST_Node_ConstFloat1::SetValue(float v)
{
    m_value = v;
}

float ST_Node_ConstFloat1::GetValue() const
{
    return m_value;
}