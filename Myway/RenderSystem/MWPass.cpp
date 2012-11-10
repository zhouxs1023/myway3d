#include "MWPass.h"
#include "MWShaderProgramDefMgr.h"
#include "MWShaderProgramManager.h"

using namespace Myway;


/* class: Pass
----------------------------------------------------------
@Remark:
    äÖÈ¾Í¨µÀ.        
----------------------------------------------------------
*/

Pass::Pass()
: m_LightingType(RLT_NONE),
  m_LightCount(1),
  m_Ambient(Color4::White),
  m_Emissive(Color4::Black),
  m_Diffuse(Color4::White),
  m_Specular(Color4::White),
  m_Power(1.0f),
  m_CullMode(CULL_CCW),
  m_ShadeMode(SHADE_GOURAUD),
  m_FillMode(FILL_SOLID),
  m_ColorWrite(COLORWRITEENABLE_ALPHA | COLORWRITEENABLE_RED | COLORWRITEENABLE_BLUE | COLORWRITEENABLE_GREEN),
  m_DepthWrite(TRUE),
  m_DepthEnable(TRUE),
  m_DepthFunc(CMP_LESSEQUAL),
  m_AlphaBlendEnable(FALSE),
  m_AlphaBlendSrcFactory(BLEND_ONE),
  m_AlphaBlendDestFactory(BLEND_ZERO),
  m_AlphaBlendOperator(BLENDOP_ADD),
  m_AlphaTestEnable(FALSE),
  m_AlphaTestFunc(CMP_ALWAYS),
  m_AlphaTestRef(0),
  m_PointMin(1.0f),
  m_PointMax(64.0f),
  m_PointScaleA(1.0f),
  m_PointScaleB(0.0f),
  m_PointScaleC(0.0f),
  m_PointSprite(FALSE),
  m_FogStart(0.0f),
  m_FogEnd(1.0f),
  m_FogDesity(1.0f),
  m_FogColor(Color4::White),
  m_DepthBias(0),
  m_SlopeScaleDepthBias(0),
  m_pVertexShader(),
  m_pPixelShader()
{
}

Pass::~Pass()
{
    DestroyAllTextureStage();
}


void Pass::SetVertexShader(ShaderProgramPtr pVertexShader)
{
    assert (ShaderProgramManager::IsVertexShader(pVertexShader->GetProfile()));

    m_pVertexShader = pVertexShader;
    m_pVertexShaderParamTable = pVertexShader->GetParamTable();
}

void Pass::SetPixelShader(ShaderProgramPtr pPixelShader)
{
    assert (ShaderProgramManager::IsPixelShader(pPixelShader->GetProfile()));

    m_pPixelShader = pPixelShader;
    m_pPixelShaderParamTable = m_pPixelShader->GetParamTable();
}


void Pass::SetVertexShader(const String & sShaderName)
{
    ShaderProgramPtr pVertexShader = ShaderProgramManager::Instance().Find(sShaderName);

    if (pVertexShader.IsNull())
    {
        const ShaderProgramDefine * pSpd = ShaderProgramDefMgr::Instance().GetShaderProgramDefine(sShaderName);
        debug_assert(pSpd, "vertex shader is not exist.");

        pVertexShader = ShaderProgramManager::Instance().CreateShader(pSpd->name,
                                                                      pSpd->language,
                                                                      pSpd->profile,
                                                                      pSpd->source,
                                                                      pSpd->entry,
                                                                      &pSpd->params);
    }

    SetVertexShader(pVertexShader);
}

void Pass::SetPixelShader(const String & sShaderName)
{
    ShaderProgramPtr pPixelShader = ShaderProgramManager::Instance().Find(sShaderName);

    if (pPixelShader.IsNull())
    {
        const ShaderProgramDefine * pSpd = ShaderProgramDefMgr::Instance().GetShaderProgramDefine(sShaderName);
        debug_assert(pSpd, "pixel shader is not exist.");

        pPixelShader = ShaderProgramManager::Instance().CreateShader(pSpd->name,
                                                                     pSpd->language,
                                                                     pSpd->profile,
                                                                     pSpd->source,
                                                                     pSpd->entry,
                                                                     &pSpd->params);
    }

    SetPixelShader(pPixelShader);
}

void Pass::DestroyTextureStage(int stage)
{
    assert(stage < m_TextureStages.Size());

    delete m_TextureStages[stage];
    m_TextureStages.Erase(m_TextureStages.Begin() + stage);
}

void Pass::DestroyAllTextureStage()
{
    Vector<TextureStage*>::Iterator iter;
    Vector<TextureStage*>::Iterator end;

    iter = m_TextureStages.Begin();
    end = m_TextureStages.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    m_TextureStages.Clear();
}


ShaderParamTablePtr Pass::GetVertexShaderParamTable()
{
    return m_pVertexShaderParamTable;
}

ShaderParamTablePtr Pass::GetPixelShaderParamTable()
{
    return m_pPixelShaderParamTable;
}

void Pass::Clone(Pass * p)
{
    DestroyAllTextureStage();

    m_Ambient               = p->m_Ambient;
    m_Emissive              = p->m_Emissive;
    m_Diffuse               = p->m_Diffuse;
    m_Specular              = p->m_Specular;
    m_Power                 = p->m_Power;
    m_CullMode              = p->m_CullMode;
    m_ShadeMode             = p->m_ShadeMode;
    m_FillMode              = p->m_FillMode;
    m_ColorWrite            = p->m_ColorWrite;
    m_DepthWrite            = p->m_DepthWrite;
    m_DepthEnable           = p->m_DepthWrite;
    m_DepthFunc             = p->m_DepthFunc;
    m_AlphaBlendSrcFactory  = p->m_AlphaBlendSrcFactory;
    m_AlphaBlendDestFactory = p->m_AlphaBlendDestFactory;
    m_AlphaBlendOperator    = p->m_AlphaBlendOperator;
    m_AlphaTestEnable       = p->m_AlphaTestEnable;
    m_AlphaTestFunc         = p->m_AlphaTestFunc;
    m_AlphaTestRef          = p->m_AlphaTestRef;
    m_PointMin              = p->m_PointMin;
    m_PointMax              = p->m_PointMax;
    m_PointScaleA           = p->m_PointScaleA;
    m_PointScaleB           = p->m_PointScaleB;
    m_PointScaleC           = p->m_PointScaleC;
    m_PointSprite           = p->m_PointSprite;
    m_FogStart              = p->m_FogStart;
    m_FogEnd                = p->m_FogEnd;
    m_FogDesity             = p->m_FogDesity;
    m_FogColor              = p->m_FogColor;
    m_DepthBias             = p->m_DepthBias;
    m_SlopeScaleDepthBias   = p->m_SlopeScaleDepthBias;
    m_pSTInstance           = p->m_pSTInstance;

    SetVertexShader(p->m_pVertexShader);
    SetPixelShader(p->m_pPixelShader);

    Vector<TextureStage*>::Iterator iter;
    Vector<TextureStage*>::Iterator end;
    TextureStage * stage;

    iter = p->m_TextureStages.Begin();
    end = p->m_TextureStages.End();

    while (iter != end)
    {
        stage = new TextureStage("");

        stage->Clone(*iter);

        ++iter;
    }
}

Pass::Pass(const Pass & p)
{
    assert(0 && "Pass::Pass(const Pass &) is invalid.");
}

Pass & Pass::operator =(const Pass & p)
{
    assert(0 && "Pass::operator =(const Pass) is invalid.");
    return *this;
}

void Pass::SetSTInstance(const String & name)
{
    Shader::ST_InstancePtr inst = Shader::ST_Library::Instance().GetSTInstance(name);

    if (inst.IsNull())
    {
        inst = Shader::ST_Library::Instance().Load(name, name);
    }

    SetSTInstance(inst);
}

void Pass::SetSTInstance(Shader::ST_InstancePtr inst)
{
    assert (inst.NotNull());

    m_pSTInstance = inst;
    SetVertexShader(inst->GetVertexShader());
    SetPixelShader(inst->GetPixelShader());
}

Shader::ST_InstancePtr Pass::GetSTInstance()
{
    return m_pSTInstance;
}