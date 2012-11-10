#pragma once

#include "MWResource.h"
#include "MWTexture.h"
#include "MWShaderFx.h"

namespace Myway
{

class MW_ENTRY Material
{
public:
    static const int MAX_EX_TEXTURES = 8;
    static const int MAX_TECHNIQUES = 8; 

public:
    Material();
    ~Material();

    void SetEmissive(const Color4 & color);
    void SetAmbient(const Color4 & color);
    void SetDiffuse(const Color4 & color);
    void SetSpecular(const Color4 & color);
    void SetSpecularPower(float power);

    void SetEmissiveMap(const TString128 & tex);
    void SetDiffuseMap(const TString128 & tex);
    void SetSpecularMap(const TString128 & tex);
    void SetNormalMap(const TString128 & tex);

    void SetEmissiveMap(TexturePtr tex);
    void SetDiffuseMap(TexturePtr tex);
    void SetSpecularMap(TexturePtr tex);
    void SetNormalMap(TexturePtr tex);

    const Color4 & GetEmissive() const;
    const Color4 & GetAmbient() const;
    const Color4 & GetDiffuse() const;
    const Color4 & GetSpecular() const;
    float GetSpecularPower() const;

    TexturePtr GetEmissiveMap() const;
    TexturePtr GetDiffuseMap() const;
    TexturePtr GetSpecularMap() const;
    TexturePtr GetNormalMap() const;


    //cull mode
    void SetCullMode(CULL_MODE mode)
    { 
        mRenderState.cullMode = mode;
    }

    CULL_MODE GetCullMode() const
    { 
        return mRenderState.cullMode;
    }

    //fill mode
    void SetFillMode(FILL_MODE mode)
    {
        mRenderState.fillMode = mode;
    }

    FILL_MODE GetFillMode() const
    {
        return mRenderState.fillMode;
    }

    // blend
    void SetBlendMode(BLEND_MODE mode)
    {
        mRenderState.blendMode = mode;
    }

    BLEND_MODE GetBlendMode() const
    {
        return mRenderState.blendMode;
    }

    //color write
    void SetColorWrite(int flag)
    {
        mRenderState.colorWrite = flag;
    }

    int GetColorWrite() const
    {
        return mRenderState.colorWrite;
    }

    //depth write
    void SetDepthWrite(bool enabled)
    {
        mRenderState.depthWrite = enabled;
    }

    bool GetDepthWrite() const
    {
        return mRenderState.depthWrite;
    }

    //depth check
    void SetDepthCheck(DEPTH_CHECK_MODE m)
    {
        mRenderState.depthCheck = m;
    }

    DEPTH_CHECK_MODE GetDepthCheck() const
    {
        return mRenderState.depthCheck;
    }

    //alpha test reference
    void SetAlphaTestRef(float a)
    {
        mRenderState.alphaTestRef = a;
    }

    float GetAlphaTestRef() const
    {
        return mRenderState.alphaTestRef;
    }

    const RenderState & GetRenderState() const
    {
        return mRenderState;
    }

    bool IsTransparency() const
    {
        return mRenderState.blendMode >= BM_ALPHA_BLEND;
    }

protected:
    RenderState mRenderState;

    // material
    Color4 mEmissive;
    Color4 mAmbient;
    Color4 mDiffuse;
    Color4 mSpecular;
    float mSpecularPower;

    TexturePtr mEmissiveMap;
    TexturePtr mDiffuseMap;
    TexturePtr mSpecularMap;
    TexturePtr mNormalMap;
};

class MaterialResource : public Resource
{
    friend class MaterialLoader;

public:
    MaterialResource();
    ~MaterialResource();

    void Alloc(int num);
    Material * GetMaterial(int index);
    int GetNumMaterials();

    virtual void Load();
    virtual void Reload();
    virtual void Unload();

    virtual void LoadImp(DataStreamPtr stream);

protected:
    Material * mMaterials;
    int mNumMaterials;
};



class MaterialLoader
{
public:
    void Load(MaterialResource * mat, DataStreamPtr stream);

protected:
    int _getNumMaterials();
    void _loadMaterial(Material * mat, xml_node * node);;

protected:
    xml_doc doc;
};


}