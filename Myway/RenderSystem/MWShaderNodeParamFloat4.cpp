#include "MWShaderNodeParamFloat4.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;

ST_Node_ParamFloat4::ST_Node_ParamFloat4(ST_Instance * parent, const String & name)
: ST_Node(parent, name),
  m_value(0)
{
}

ST_Node_ParamFloat4::~ST_Node_ParamFloat4()
{
}

const String & ST_Node_ParamFloat4::GetVarName() const
{
    return m_name;
}

ST_NODE_TYPE ST_Node_ParamFloat4::GetType() const
{
    return STNODE_PARAM_FLOAT4;
}

ST_VALUE_TYPE ST_Node_ParamFloat4::GetValueType() const
{
    return STVAL_FLOAT4;
}

String ST_Node_ParamFloat4::Generte()
{
    assert (!IsGenerated());

    m_generted = TRUE;

    return ST_Code_Factory::Instance().GenerteCode(this);
}

void ST_Node_ParamFloat4::NotifyDestroy(ST_Node * node)
{
}

String ST_Node_ParamFloat4::GetUniformParamDefine()
{
    return "";
}

void ST_Node_ParamFloat4::SetValue(const Vec4 & v)
{
    m_value = v;
}

const Vec4 & ST_Node_ParamFloat4::GetValue() const
{
    return m_value;
}