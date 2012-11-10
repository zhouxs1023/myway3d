#pragma once

#include "MWShaderNode.h"
#include "MWShaderProgram.h"

namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Instance : public RefObj
{
    DeclareRefObj

protected:
    typedef Map<String, ST_Node*>       NodeMap;

public:
    typedef Visitor<NodeMap::Iterator>  NodeVisitor;

public:
    ST_Instance(const String & name);
    ~ST_Instance();

    const String & GetName() const;

    void GenerteShader();

    ST_Node * CreateNode(const String & name, ST_NODE_TYPE type);
    ST_Node * GetNode(const String & name);
    void DestroyNode(const String & name);
    void DestroyNode(ST_Node * node);
    void DestroyAllNode();

    void SetDiffuse(ST_Node * node);
    void SetDiffusePower(ST_Node * node);
    void SetSpecular(ST_Node * node);
    void SetSpecularPower(ST_Node * node);
    void SetNormal(ST_Node * node);
    void SetTransparency(ST_Node * node);

    void SetDiffuse(const String & name);
    void SetDiffusePower(const String & name);
    void SetSpecular(const String & name);
    void SetSpecularPower(const String & name);
    void SetNormal(const String & name);
    void SetTransparency(const String & name);

    ST_Node * GetDiffuse();
    ST_Node * GetDiffusePower();
    ST_Node * GetSpecular();
    ST_Node * GetSpecularPower();
    ST_Node * GetNormal();
    ST_Node * GetTransparency();

    void SetLightingMode(LIGHTING_MODE mode);
    LIGHTING_MODE GetLightingMode() const;

    void SetFogMode(FOG_MODE mode);
    FOG_MODE GetFogMode() const;

    NodeVisitor GetAllNodes();

    void _SetVertexShader(ShaderProgramPtr shader);
    void _SetPixelShader(ShaderProgramPtr shader);

    ShaderProgramPtr GetVertexShader() const;
    ShaderProgramPtr GetPixelShader() const;

    void SetVertexShaderProfile(SHADER_PROGRAM_PROFILE profile);
    void SetPixelShaderProfile(SHADER_PROGRAM_PROFILE profile);
    SHADER_PROGRAM_PROFILE GetVertexShaderProfile() const;
    SHADER_PROGRAM_PROFILE GetPixelShaderProfile() const;

    void SetSkeletonAnimation(bool skel);
    bool GetSkeletonAnimation() const;

    void SetShadowEnable(bool shadow);
    bool GetShadowEnable() const;

protected:
    void NotifyDestroy(ST_Node * node);

protected:
    String              m_name;
    ST_Node *           m_diffuse;
    ST_Node *           m_diffuse_power;
    ST_Node *           m_specular;
    ST_Node *           m_specular_power;
    ST_Node *           m_normal;
    ST_Node *           m_transparency;
    NodeMap             m_nodes;
    LIGHTING_MODE       m_lightingmode;

    FOG_MODE            mogmode;
    bool                m_skeleton;
    bool                m_shadow;
    ShaderProgramPtr    m_vshader;
    ShaderProgramPtr    mshader;

    SHADER_PROGRAM_PROFILE m_vsprofile;
    SHADER_PROGRAM_PROFILE msprofile;
};

DeclareSmartPtr(ST_Instance);

}

}