

inline void Pass::SetLighting(RenderLightingType type, int count)
{
    d_assert (count <= MAX_LIGHTS);

    m_LightingType = type;
    m_LightCount = count;
}

inline RenderLightingType Pass::GetLightingType() const
{
    return m_LightingType;
}

inline int Pass::GetLightingCount() const
{
    return m_LightCount;
}

inline void Pass::SetAmbient(float r, float g, float b, float a)
{
    m_Ambient.r = r;
    m_Ambient.g = g;
    m_Ambient.b = b;
    m_Ambient.a = a;
}

inline void Pass::SetAmbient(const Color4 & c)
{
    m_Ambient = c;
}

inline const Color4 & Pass::GetAmbient() const
{
    return m_Ambient;
}

inline void Pass::SetEmissive(float r, float g, float b, float a)
{
    m_Emissive.r = r;
    m_Emissive.g = g;
    m_Emissive.b = b;
    m_Emissive.a = a;
}

inline void Pass::SetEmissive(const Color4 & c)
{
    m_Emissive = c;
}

inline const Color4 & Pass::GetEmissive() const
{
    return m_Emissive;
}

inline void Pass::SetDiffuse(float r, float g, float b, float a)
{
    m_Diffuse.r = r;
    m_Diffuse.g = g;
    m_Diffuse.b = b;
    m_Diffuse.a = a;
}

inline void Pass::SetDiffuse(const Color4 & c)
{
    m_Diffuse = c;
}

inline const Color4 & Pass::GetDiffuse() const
{
    return m_Diffuse;
}

inline void Pass::SetSpecular(float r, float g, float b, float a)
{
    m_Specular.r = r;
    m_Specular.g = g;
    m_Specular.b = b;
    m_Specular.a = a;
}

inline void Pass::SetSpecular(const Color4 & c)
{
    m_Specular = c;
}

inline const Color4 & Pass::GetSpecular() const
{
    return m_Specular;
}

inline void Pass::SetPower(float power)
{
    m_Power = power;
}

inline float Pass::GetPower() const
{
    return m_Power;
}

inline void Pass::SetCullMode(CULL_MODE mode)
{
    m_CullMode = mode;
}

inline void Pass::SetShadeMode(SHADE_MODE mode)
{
    m_ShadeMode = mode;
}

inline void Pass::SetFillMode(FILL_MODE mode)
{
    m_FillMode = mode;
}

inline void Pass::SetColorWrite(int ColorChanel)
{
    m_ColorWrite = ColorChanel;
}

inline void Pass::SetDepthWrite(bool bEnabled)
{
    m_DepthWrite = bEnabled;
}

inline void Pass::SetDepthCheck(bool bEnabled)
{
    m_DepthEnable = bEnabled;
}

inline void Pass::SetDepthFunction(COMPARE_FUNCTION func)
{
    m_DepthFunc = func;
}

inline void Pass::SetAlphaBlendEnable(bool bEnabled)
{
    m_AlphaBlendEnable = bEnabled;
}

inline void Pass::SetAlphaBlendSourceFactory(BLEND_FACTORY src)
{
    m_AlphaBlendSrcFactory = src;
}

inline void Pass::SetAlphaBlendDestFactory(BLEND_FACTORY dst)
{
    m_AlphaBlendDestFactory = dst;
}

inline void Pass::SetAlphaBlendOperator(BLEND_OPERATOR op)
{
    m_AlphaBlendOperator = op;
}

inline void Pass::SetAlphaTestEnable(bool bEnabled)
{
    m_AlphaTestEnable = bEnabled;
}

inline void Pass::SetAlphaTestFunction(COMPARE_FUNCTION func)
{
    m_AlphaTestFunc = func;
}

inline void Pass::SetAlphaTestReference(int iRef)
{
    m_AlphaTestRef = iRef;
}

inline CULL_MODE Pass::GetCullMode() const
{
    return m_CullMode;
}

inline SHADE_MODE Pass::GetShadeMode() const
{
    return m_ShadeMode;
}

inline FILL_MODE Pass::GetFillMode() const
{
    return m_FillMode;
}

inline int Pass::GetColorWrite() const
{
    return m_ColorWrite;
}

inline bool Pass::GetDepthWriteEnable() const
{
    return m_DepthWrite;
}

inline bool Pass::GetDepthCheckEnable() const
{
    return m_DepthEnable;
}

inline COMPARE_FUNCTION Pass::GetDepthCheckFuncion() const
{
    return m_DepthFunc;
}

inline bool Pass::GetAlphaBlendEnable() const
{
    return m_AlphaBlendEnable;
}

inline bool Pass::IsTransparency() const
{
    return m_AlphaBlendEnable || m_DepthFunc == CMP_ALWAYS || m_DepthWrite == FALSE;
}


inline BLEND_FACTORY Pass::GetAlphaBlendSourceFactory() const
{
    return m_AlphaBlendSrcFactory;
}

inline BLEND_FACTORY Pass::GetAlphaBlendDestFactory() const
{
    return m_AlphaBlendDestFactory;
}

inline BLEND_OPERATOR Pass::GetAlphaBlendOperator() const
{
    return m_AlphaBlendOperator;
}

inline bool Pass::GetAlphaTestEnable() const
{
    return m_AlphaTestEnable;
}

inline COMPARE_FUNCTION Pass::GetAlphaTestFunction() const
{
    return m_AlphaTestFunc;
}

inline int Pass::GetAlphaTestReference() const
{
    return m_AlphaTestRef;
}

inline ShaderProgramPtr Pass::GetVertexShader() const
{
    return m_pVertexShader;
}

inline ShaderProgramPtr Pass::GetPixelShader() const
{
    return m_pPixelShader;
}

inline TextureStage * Pass::CreateTextureStage()
{
    d_assert(m_TextureStages.Size() <= MAX_TEXTURE_STAGE);

    TextureStage * stage = new TextureStage("");
    m_TextureStages.PushBack(stage);
    return stage;
}

inline TextureStage * Pass::CreateTextureStage(const String & name)
{
    d_assert(m_TextureStages.Size() <= MAX_TEXTURE_STAGE);

    TextureStage * stage = new TextureStage(name);
    m_TextureStages.PushBack(stage);
    return stage;
}

inline int Pass::GetTextureStageCount() const
{
    return m_TextureStages.Size();
}

inline TextureStage * Pass::GetTextureStage(int stage)
{
    d_assert(stage < m_TextureStages.Size());
    return m_TextureStages[stage];
}

inline TextureStage * Pass::GetTextureStage(const String & stage)
{
    for (int i = 0; i < m_TextureStages.Size(); ++i)
    {
        if (m_TextureStages[i]->GetName() == stage)
            return m_TextureStages[i];
    }

    return NULL;
}


inline void Pass::SetPointMinSize(float size)
{
    m_PointMin = size;
}

inline float Pass::GetPointMinSize() const
{
    return m_PointMin;
}

inline void Pass::SetPointMaxSize(float size)
{
    m_PointMax = size;
}

inline float Pass::GetPointMaxSize() const
{
    return m_PointMax;
}

inline void Pass::SetPointScaleA(float a)
{
    m_PointScaleA = a;
}

inline float Pass::GetPointScaleA() const
{
    return m_PointScaleA;
}

inline void Pass::SetPointScaleB(float b)
{
    m_PointScaleB = b;
}

inline float Pass::GetPointScaleB() const
{
    return m_PointScaleB;
}

inline void Pass::SetPointScaleC(float c)
{
    m_PointScaleC = c;
}

inline float Pass::GetPointScaleC() const
{
    return m_PointScaleC;
}

inline void Pass::SetPointSpriteEnable(bool bEnable)
{
    m_PointSprite = bEnable;
}

inline bool Pass::GetPointSpriteEnable() const
{
    return m_PointSprite;
}

inline void Pass::SetFogStart(float start)
{
    m_FogStart = start;
}

inline float Pass::GetFogStart() const
{
    return m_FogStart;
}

inline void Pass::SetFogEnd(float end)
{
    m_FogEnd = end;
}

inline float Pass::GetFogEnd() const
{
    return m_FogEnd;
}

inline void Pass::SetFogDensity(float density)
{
    m_FogDesity = density;
}

inline float Pass::GetFogDensity() const
{
    return m_FogDesity;
}

inline void Pass::SetFogColor(float r, float g, float b, float a)
{
    m_FogColor.r = r;
    m_FogColor.g = g;
    m_FogColor.b = b;
    m_FogColor.a = a;
}

inline void Pass::SetFogColor(const Color4 & color)
{
    m_FogColor = color;
}

inline const Color4 & Pass::GetFogColor() const
{
    return m_FogColor;
}

inline void Pass::SetDepthBias(float bias)
{
    m_DepthBias = bias;
}

inline void Pass::SetDepthBiasSlopeScale(float scale)
{
    m_SlopeScaleDepthBias = scale;
}

inline float Pass::GetDepthBias() const
{
    return m_DepthBias;
}

inline float Pass::GetDepthBiasSlopeScale() const
{
    return m_SlopeScaleDepthBias;
}