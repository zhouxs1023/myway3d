#include "MWShaderNodeSampler2D.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;


ST_Node_Sampler2D::ST_Node_Sampler2D(ST_Instance * parent, const String & name)
: ST_Node(parent, name),
  mnput(NULL)
{
}

ST_Node_Sampler2D::~ST_Node_Sampler2D()
{
}

ST_NODE_TYPE ST_Node_Sampler2D::GetType() const
{
    return STNODE_SAMPLER2D;
}

ST_VALUE_TYPE ST_Node_Sampler2D::GetValueType() const
{
    return STVAL_FLOAT4;
}

String ST_Node_Sampler2D::Generte()
{
    assert (!IsGenerated());

    String code;

    if (mnput && !mnput->IsGenerated())
    {
        code += mnput->Generte();
    }

    m_generted = TRUE;

    return code + ST_Code_Factory::Instance().GenerteCode(this);
}

ST_Node * ST_Node_Sampler2D::GetInput()
{
    return mnput;
}

void ST_Node_Sampler2D::SetInput(ST_Node * node)
{
    assert (this == node && node->GetParent() == marent);
    mnput = node;
}

void ST_Node_Sampler2D::SetInput(const String & name)
{
    SetInput(marent->GetNode(name));
}

void ST_Node_Sampler2D::NotifyDestroy(ST_Node * node)
{
    if (mnput == node)
        mnput = NULL;
}

void ST_Node_Sampler2D::SetTexture(const String & texture)
{
    m_texture = texture;
}

const String & ST_Node_Sampler2D::GetTexture() const
{
    return m_texture;
}

String ST_Node_Sampler2D::GetUniformParamDefine()
{
    return ST_Code_Factory::Instance().GenerteUniformParamDefine(this);
}