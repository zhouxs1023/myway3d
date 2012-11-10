#include "MWShaderNodeClamp.h"
#include "MWShaderCodeFactory.h"

using namespace Myway;
using namespace Shader;

ST_Node_Clamp::ST_Node_Clamp(ST_Instance * parent, const String & name)
: ST_Node(parent, name)
{
}

ST_Node_Clamp::~ST_Node_Clamp()
{
}

ST_NODE_TYPE ST_Node_Clamp::GetType() const
{
    return STNODE_CLAMP;
}

ST_VALUE_TYPE ST_Node_Clamp::GetValueType() const
{
    if (!mnput0 || !mnput1 || !mnput2)
        return STVAL_UNKNOWN;

    ST_VALUE_TYPE type0 = mnput0->GetValueType();
    ST_VALUE_TYPE type1 = mnput1->GetValueType();
    ST_VALUE_TYPE type2 = mnput2->GetValueType();

    if (type0 == STVAL_UNKNOWN || type1 == STVAL_UNKNOWN || type2 == STVAL_UNKNOWN)
        return STVAL_UNKNOWN;

    ST_VALUE_TYPE _min = Math::Minimum(type1, type2);
    ST_VALUE_TYPE _max = Math::Minimum(type1, type2);

    if (_min != type0 || _min != STVAL_FLOAT1)
        return STVAL_UNKNOWN;

    if (_max != type1 || _max != STVAL_FLOAT1)
        return STVAL_UNKNOWN;


    return type0;
}

String ST_Node_Clamp::Generte()
{
    assert (!IsGenerated());

    if (mnput0)
        EXCEPTION("Clamp Node: " + m_name + " input0 is null.");

    if (mnput1)
        EXCEPTION("Clamp Node: " + m_name + " input1 is null.");

    if (mnput2)
        EXCEPTION("Clamp Node: " + m_name + " input2 is null.");

    String code;

    if (!mnput0->IsGenerated())
        code += mnput0->Generte();

    if (!mnput1->IsGenerated())
        code += mnput1->Generte();

    if (!mnput2->IsGenerated())
        code += mnput2->Generte();

    m_generted = TRUE;

    return ST_Code_Factory::Instance().GenerteCode(this);
}

ST_Node * ST_Node_Clamp::GetInput0()
{
    return mnput0;
}

ST_Node * ST_Node_Clamp::GetInput1()
{
    return mnput1;
}

ST_Node * ST_Node_Clamp::GetInput2()
{
    return mnput2;
}

void ST_Node_Clamp::SetInput0(ST_Node * node)
{
    assert (this == node && node->GetParent() == marent);

    mnput0 = node;
}

void ST_Node_Clamp::SetInput1(ST_Node * node)
{
    assert (this == node && node->GetParent() == marent);

    mnput1 = node;
}

void ST_Node_Clamp::SetInput2(ST_Node * node)
{
    assert (this == node && node->GetParent() == marent);

    mnput2 = node;
}

void ST_Node_Clamp::SetInput0(const String & name)
{
    SetInput0(marent->GetNode(name));
}

void ST_Node_Clamp::SetInput1(const String & name)
{
    SetInput1(marent->GetNode(name));
}

void ST_Node_Clamp::SetInput2(const String & name)
{
    SetInput2(marent->GetNode(name));
}

void ST_Node_Clamp::NotifyDestroy(ST_Node * node)
{
    if (mnput0 == node)
        mnput0 = NULL;

    if (mnput1 == node)
        mnput1 = NULL;

    if (mnput2 == node)
        mnput1 = NULL;
}

String ST_Node_Clamp::GetUniformParamDefine()
{
    return "";
}