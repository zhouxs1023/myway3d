#include "MWShaderCodeFactory.h"
#include "MWShaderCodeGenerterCg.h"

using namespace Myway;
using namespace Shader;


ST_Code_Factory::ST_Code_Factory()
{
    m_generter = new ST_Code_Generter_Cg();
}

ST_Code_Factory::~ST_Code_Factory()
{
    safe_delete(m_generter);
}

void ST_Code_Factory::SetCodeGenerter(SHADER_PROGRAM_LANGUAGE lauguage)
{
    safe_delete(m_generter);

    if (lauguage == SHADER_LANGUAGE_CG)
    {
        m_generter = new ST_Code_Generter_Cg();
    }
    else
    {
        assert (0);
    }
}

String ST_Code_Factory::GenerteCode(ST_Node * node)
{
    return m_generter->GenerteCode(node);
}

void  ST_Code_Factory::GenerteShader(ST_Instance * inst)
{
    m_generter->GenerteShader(inst);
}

String ST_Code_Factory::GenerteUniformParamDefine(ST_Node * node)
{
    return m_generter->GenerteUniformParamDefine(node);
}