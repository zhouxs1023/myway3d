#include "MWShaderInstance.h"
#include "MWShaderNodeSampler2D.h"
#include "MWShaderCodeFactory.h"
#include "MWShaderLibrary.h"
#include "MWShaderProgramManager.h"

using namespace Myway;
using namespace Shader;

ST_Instance::ST_Instance(const String & name)
: m_name(name),
  m_diffuse(NULL),
  m_diffuse_power(NULL),
  m_specular(NULL),
  m_specular_power(NULL),
  m_normal(NULL),
  m_transparency(NULL),
  mogmode(FOG_LINEAR),
  m_lightingmode(LM_NONE),
  m_shadow(FALSE)
{
}

ST_Instance::~ST_Instance()
{
    DestroyAllNode();
}

void ST_Instance::DeleteSelf()
{
    ST_Library::Instance().DestroySTInstance(this);
}

const String & ST_Instance::GetName() const
{
    return m_name;
}

void ST_Instance::GenerteShader()
{
    ST_Code_Factory::Instance().GenerteShader(this);
}

ST_Node * ST_Instance::CreateNode(const String & name, ST_NODE_TYPE type)
{
    if (GetNode(name))
        return NULL;

    ST_Node * node = NULL;
    switch (type)
    {
    case STNODE_SAMPLER2D:
        node = new ST_Node_Sampler2D(this, name);
        break;

    case STNODE_ADD:
        node = new ST_Node_Add(this, name);
        break;

    case STNODE_SUB:
        node = new ST_Node_Sub(this, name);
        break;

    case STNODE_MULTIPLY:
        node = new ST_Node_Multiply(this, name);
        break;

    case STNODE_CONST_FLOAT1:
        node = new ST_Node_ConstFloat1(this, name);
        break;

    case STNODE_CONST_FLOAT2:
        node = new ST_Node_ConstFloat2(this, name);
        break;

    case STNODE_CONST_FLOAT3:
        node = new ST_Node_ConstFloat3(this, name);
        break;

    case STNODE_CONST_FLOAT4:
        node = new ST_Node_ConstFloat4(this, name);
        break;

    case STNODE_PARAM_FLOAT4:
        node = new ST_Node_ParamFloat4(this, name);
        break;
    }

    assert (node);

    if (node)
    {
        m_nodes.Insert(name, node);
    }

    return node;
}

ST_Node * ST_Instance::GetNode(const String & name)
{
    NodeMap::Iterator iter = m_nodes.Find(name);
    NodeMap::Iterator end = m_nodes.End();

    return iter != end ? iter->second : NULL;
}

void ST_Instance::DestroyNode(const String & name)
{
    NodeMap::Iterator iter = m_nodes.Find(name);
    NodeMap::Iterator end = m_nodes.End();

    if (iter == end)
    {
        String msg = "Error: Node '" + name + "' + don't exist in Instance '" + m_name + "'";
        EXCEPTION(msg);
    }

    NotifyDestroy(iter->second);

    delete iter->second;
    m_nodes.Erase(iter);
}

void ST_Instance::DestroyNode(ST_Node * node)
{
    DestroyNode(node->GetName());
}

void ST_Instance::DestroyAllNode()
{
    NodeMap::Iterator iter = m_nodes.Begin();
    NodeMap::Iterator end = m_nodes.End();

    while (iter != end)
    {
        delete iter->second;
        ++iter;
    }

    m_nodes.Clear();
    m_diffuse = NULL;
    m_diffuse_power = NULL;
    m_specular = NULL;
    m_specular_power = NULL;
    m_normal = NULL;
    m_transparency = NULL;
}

void ST_Instance::SetDiffuse(ST_Node * node)
{
    m_diffuse = node;
}

void ST_Instance::SetDiffusePower(ST_Node * node)
{
    assert (!node || node->GetParent() == this);

    m_diffuse_power = node;
}

void ST_Instance::SetSpecular(ST_Node * node)
{
    assert (!node || node->GetParent() == this);

    m_specular = node;
}

void ST_Instance::SetSpecularPower(ST_Node * node)
{
    assert (!node || node->GetParent() == this);

    m_specular_power = node;
}

void ST_Instance::SetNormal(ST_Node * node)
{
    assert (!node || node->GetParent() == this);

    m_normal = node;
}

void ST_Instance::SetTransparency(ST_Node * node)
{
    assert (!node || node->GetParent() == this);

    m_transparency = node;
}

void ST_Instance::SetDiffuse(const String & name)
{
    m_diffuse = GetNode(name);
}

void ST_Instance::SetDiffusePower(const String & name)
{
    m_diffuse_power = GetNode(name);
}

void ST_Instance::SetSpecular(const String & name)
{
    m_specular = GetNode(name);
}

void ST_Instance::SetSpecularPower(const String & name)
{
    m_specular_power = GetNode(name);
}

void ST_Instance::SetNormal(const String & name)
{
    m_normal = GetNode(name);
}

void ST_Instance::SetTransparency(const String & name)
{
    m_transparency = GetNode(name);
}

ST_Node * ST_Instance::GetDiffuse()
{
    return m_diffuse;
}

ST_Node * ST_Instance::GetDiffusePower()
{
    return m_diffuse_power;
}

ST_Node * ST_Instance::GetSpecular()
{
    return m_specular;
}

ST_Node * ST_Instance::GetSpecularPower()
{
    return m_specular_power;
}

ST_Node * ST_Instance::GetNormal()
{
    return m_normal;
}

ST_Node * ST_Instance::GetTransparency()
{
    return m_transparency;
}

void ST_Instance::SetLightingMode(LIGHTING_MODE mode)
{
    m_lightingmode = mode;
}

LIGHTING_MODE ST_Instance::GetLightingMode() const
{
    return m_lightingmode;
}

void ST_Instance::SetFogMode(FOG_MODE mode)
{
    mogmode = mode;
}

FOG_MODE ST_Instance::GetFogMode() const
{
    return mogmode;
}

void ST_Instance::NotifyDestroy(ST_Node * node)
{
    NodeMap::Iterator iter = m_nodes.Begin();
    NodeMap::Iterator end = m_nodes.End();

    if (iter != end)
    {
        iter->second->NotifyDestroy(node);
        ++iter;
    }

    if (m_diffuse == node)
        node = NULL;

    if (m_specular == node)
        m_specular = NULL;

    if (m_diffuse_power == node)
        m_diffuse_power = NULL;

    if (m_specular_power == node)
        m_specular_power = NULL;

    if (m_normal == node)
        m_normal = NULL;

    if (m_transparency == node)
        m_transparency = NULL;
}

ST_Instance::NodeVisitor ST_Instance::GetAllNodes()
{
    return NodeVisitor(m_nodes.Begin(), m_nodes.End());
}

void ST_Instance::_SetVertexShader(ShaderProgramPtr shader)
{
    m_vshader = shader;
}

void ST_Instance::_SetPixelShader(ShaderProgramPtr shader)
{
    mshader = shader;
}

ShaderProgramPtr ST_Instance::GetVertexShader() const
{
    return m_vshader;
}

ShaderProgramPtr ST_Instance::GetPixelShader() const
{
    return mshader;
}

void ST_Instance::SetSkeletonAnimation(bool skel)
{
    m_skeleton = skel;
}

bool ST_Instance::GetSkeletonAnimation() const
{
    return m_skeleton;
}

void ST_Instance::SetShadowEnable(bool shadow)
{
    m_shadow = shadow;
}

bool ST_Instance::GetShadowEnable() const
{
    return m_shadow;
}

void ST_Instance::SetVertexShaderProfile(SHADER_PROGRAM_PROFILE profile)
{
    assert (ShaderProgramManager::IsVertexShader(profile));
    m_vsprofile = profile;
}

void ST_Instance::SetPixelShaderProfile(SHADER_PROGRAM_PROFILE profile)
{
    assert (ShaderProgramManager::IsPixelShader(profile));
    msprofile = profile;
}

SHADER_PROGRAM_PROFILE ST_Instance::GetVertexShaderProfile() const
{
    return m_vsprofile;
}

SHADER_PROGRAM_PROFILE ST_Instance::GetPixelShaderProfile() const
{
    return msprofile;
}