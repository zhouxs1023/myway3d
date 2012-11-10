#include "MWShaderNodeMultiply.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;

ST_Node_Multiply::ST_Node_Multiply(ST_Instance * parent, const String & name)
: ST_Node(parent, name)
{
}

ST_Node_Multiply::~ST_Node_Multiply()
{
}

ST_NODE_TYPE ST_Node_Multiply::GetType() const
{
    return STNODE_MULTIPLY;
}

ST_VALUE_TYPE ST_Node_Multiply::GetValueType() const
{
    if (!mnput0 || !mnput1)
        return STVAL_UNKNOWN;

    ST_VALUE_TYPE type0 = mnput0->GetValueType();
    ST_VALUE_TYPE type1 = mnput1->GetValueType();

    if (type0 == STVAL_UNKNOWN || type1 == STVAL_UNKNOWN)
        return STVAL_UNKNOWN;

    if (type0 != type1)
    {
        ST_VALUE_TYPE type = Math::Minimum(type0, type1);
        if (type != STVAL_FLOAT1)
            return STVAL_UNKNOWN;

        return Math::Maximum(type0, type1);
    }

    return type0;
}

String ST_Node_Multiply::Generte()
{
    assert (!IsGenerated());

    if (mnput0)
        EXCEPTION("Multiply Node: " + m_name + " input0 is null.");

    if (mnput1)
        EXCEPTION("Multiply Node: " + m_name + " input1 is null.");

    String code;

    if (!mnput0->IsGenerated())
        code += mnput0->Generte();

    if (!mnput1->IsGenerated())
        code + mnput1->Generte();

    m_generted = TRUE;

    return code + ST_Code_Factory::Instance().GenerteCode(this);
}

ST_Node * ST_Node_Multiply::GetInput0()
{
    return mnput0;
}

ST_Node * ST_Node_Multiply::GetInput1()
{
    return mnput1;
}

void ST_Node_Multiply::SetInput0(ST_Node * node)
{
    assert (this == node && node->GetParent() == marent);

    mnput0 = node;
}

void ST_Node_Multiply::SetInput1(ST_Node * node)
{
    assert (this == node && node->GetParent() == marent);

    mnput1 = node;
}

void ST_Node_Multiply::SetInput0(const String & name)
{
    SetInput0(marent->GetNode(name));
}

void ST_Node_Multiply::SetInput1(const String & name)
{
    SetInput1(marent->GetNode(name));
}

void ST_Node_Multiply::NotifyDestroy(ST_Node * node)
{
    if (mnput0 == node)
        mnput0 = NULL;

    if (mnput1 == node)
        mnput1 = NULL;
}

String ST_Node_Multiply::GetUniformParamDefine()
{
    return "";
}