#pragma once

#include "MWRenderDefine.h"
#include "MWShaderParam.h"
#include "MWShaderProgram.h"
#include "MWTextureStage.h"
#include "MWShaderLibrary.h"

namespace Myway
{

/* class: Pass
----------------------------------------------------------
    @Remark:
        ‰÷»æÕ®µ¿.        
----------------------------------------------------------
*/
enum RenderLightingType
{
    RLT_NONE        = 0,
    RLT_DIRECTIONAL = 1,
    RLT_POINT       = 2,
    RLT_SPOT        = 3,
    RLT_ALL         = 4,

    MW_ALIGN_ENUM(RenderLightingType)
};

class MW_ENTRY Pass : public AllocObj
{
public:
                            Pass();
                            ~Pass();

    //lighting
    void                    SetLighting(RenderLightingType type, int count);
    RenderLightingType      GetLightingType() const;
    int                  GetLightingCount() const;

    //ambinet
    void                    SetAmbient(float r, float g, float b, float a);
    void                    SetAmbient(const Color4 & c);
    const Color4 &          GetAmbient() const;

    //emissive
    void                    SetEmissive(float r, float g, float b, float a);
    void                    SetEmissive(const Color4 & c);
    const Color4 &          GetEmissive() const;

    //diffuse
    void                    SetDiffuse(float r, float g, float b, float a);
    void                    SetDiffuse(const Color4 & c);
    const Color4 &          GetDiffuse() const;

    //specular
    void                    SetSpecular(float r, float g, float b, float a);
    void                    SetSpecular(const Color4 & c);
    const Color4 &          GetSpecular() const;

    //power
    void                    SetPower(float power);
    float                   GetPower() const;

    //cull mode
    void                    SetCullMode(CULL_MODE mode);
    CULL_MODE               GetCullMode() const;

    //shade mode
    void                    SetShadeMode(SHADE_MODE mode);
    SHADE_MODE              GetShadeMode() const;

    //fill mode
    void                    SetFillMode(FILL_MODE mode);
    FILL_MODE               GetFillMode() const;

    //color write
    void                    SetColorWrite(int ColorChanel);
    int                  GetColorWrite() const;

    //depth write
    void                    SetDepthWrite(bool bEnabled);
    bool                    GetDepthWriteEnable() const;

    //depth check
    void                    SetDepthCheck(bool bEnabled);
    bool                    GetDepthCheckEnable() const;

    //depth function
    void                    SetDepthFunction(COMPARE_FUNCTION func);
    COMPARE_FUNCTION        GetDepthCheckFuncion() const;

    //alpha blend enable
    void                    SetAlphaBlendEnable(bool bEnabled);
    bool                    GetAlphaBlendEnable() const;
    bool                    IsTransparency() const;

    //alpha blend source factory
    void                    SetAlphaBlendSourceFactory(BLEND_FACTORY src);
    BLEND_FACTORY           GetAlphaBlendSourceFactory() const;

    //alpha blend dest factory
    void                    SetAlphaBlendDestFactory(BLEND_FACTORY dst);
    BLEND_FACTORY           GetAlphaBlendDestFactory() const;

    //alpha blend operator
    void                    SetAlphaBlendOperator(BLEND_OPERATOR op);
    BLEND_OPERATOR          GetAlphaBlendOperator() const;

    //alpha test enable
    void                    SetAlphaTestEnable(bool bEnable);
    bool                    GetAlphaTestEnable() const;

    //alpha test function
    void                    SetAlphaTestFunction(COMPARE_FUNCTION func);
    COMPARE_FUNCTION        GetAlphaTestFunction() const;

    //alpha test reference
    void                    SetAlphaTestReference(int iRef);
    int                  GetAlphaTestReference() const;

    //point min
    void                    SetPointMinSize(float size);
    float                   GetPointMinSize() const;

    //point max
    void                    SetPointMaxSize(float size);
    float                   GetPointMaxSize() const;

    //point scale a
    void                    SetPointScaleA(float a);
    float                   GetPointScaleA() const;

    //point scale b
    void                    SetPointScaleB(float b);
    float                   GetPointScaleB() const;

    //point scale c
    void                    SetPointScaleC(float c);
    float                   GetPointScaleC() const;

    //point sprite
    void                    SetPointSpriteEnable(bool bEnable);
    bool                    GetPointSpriteEnable() const;

    //fog start
    void                    SetFogStart(float start);
    float                   GetFogStart() const;

    //fog end
    void                    SetFogEnd(float end);
    float                   GetFogEnd() const;

    //fog density
    void                    SetFogDensity(float density);
    float                   GetFogDensity() const;

    //fog color
    void                    SetFogColor(float r, float g, float b, float a);
    void                    SetFogColor(const Color4 & color);
    const Color4 &          GetFogColor() const;

    //depth bias
    void                    SetDepthBias(float bias);
    void                    SetDepthBiasSlopeScale(float scale);
    float                   GetDepthBias() const;
    float                   GetDepthBiasSlopeScale() const;

    //vertex program
    void                    SetVertexShader(ShaderProgramPtr pVertexShader);
    void                    SetVertexShader(const String & sShaderName);
    ShaderProgramPtr        GetVertexShader() const;

    //pixel program
    void                    SetPixelShader(ShaderProgramPtr pPixelShader);
    void                    SetPixelShader(const String & sShaderName);
    ShaderProgramPtr        GetPixelShader() const;

    //texture stage
    TextureStage *          CreateTextureStage();
    TextureStage *          CreateTextureStage(const String & name);
    int                  GetTextureStageCount() const;
    TextureStage *          GetTextureStage(int stage);
    TextureStage *          GetTextureStage(const String & stage);
    void                    DestroyTextureStage(int stage);
    void                    DestroyAllTextureStage();

    ShaderParamTablePtr     GetVertexShaderParamTable();
    ShaderParamTablePtr     GetPixelShaderParamTable();

    void                    SetSTInstance(const String & name);
    void                    SetSTInstance(Shader::ST_InstancePtr inst);
    Shader::ST_InstancePtr  GetSTInstance();

    void                    Clone(Pass * pass);


                            Pass(const Pass & p);
    Pass &                  operator =(const Pass & p);

public:
    //lighting type
    RenderLightingType      m_LightingType;
    int                  m_LightCount;

    //ambiemt
    Color4                  m_Ambient;

    //emissive
    Color4                  m_Emissive;

    //diffuse
    Color4                  m_Diffuse;

    //specular
    Color4                  m_Specular;

    //power
    float                   m_Power;

    //cull mode
    CULL_MODE               m_CullMode;

    //shade mode
    SHADE_MODE              m_ShadeMode;

    //fill mode
    FILL_MODE               m_FillMode;

    //color
    int                  m_ColorWrite;

    //depth
    bool                    m_DepthWrite;
    bool                    m_DepthEnable;
    COMPARE_FUNCTION        m_DepthFunc;

    //alpha blend
    bool                    m_AlphaBlendEnable;
    BLEND_FACTORY           m_AlphaBlendSrcFactory;
    BLEND_FACTORY           m_AlphaBlendDestFactory;
    BLEND_OPERATOR          m_AlphaBlendOperator;

    //alpha test
    bool                    m_AlphaTestEnable;
    COMPARE_FUNCTION        m_AlphaTestFunc;
    int                  m_AlphaTestRef;

    //point 
    float                   m_PointMin;
    float                   m_PointMax;
    float                   m_PointScaleA;
    float                   m_PointScaleB;
    float                   m_PointScaleC;
    bool                    m_PointSprite;

    //fog
    float                   m_FogStart;
    float                   m_FogEnd;
    float                   m_FogDesity;
    Color4                  m_FogColor;

    //depth bias
    float                   m_DepthBias;
    float                   m_SlopeScaleDepthBias;


    ShaderProgramPtr        m_pVertexShader;
    ShaderProgramPtr        m_pPixelShader;
    ShaderParamTablePtr     m_pVertexShaderParamTable;
    ShaderParamTablePtr     m_pPixelShaderParamTable;

    Vector<TextureStage*>   m_TextureStages;

    Shader::ST_InstancePtr  m_pSTInstance;
};

#include "MWPass.inl"

}