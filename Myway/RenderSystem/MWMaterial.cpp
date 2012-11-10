#include "MWMaterial.h"
#include "MWVideoBufferManager.h"
#include "MWResourceManager.h"
#include "MWRenderHelper.h"

#pragma warning(push)
#pragma warning(disable : 4244)

namespace Myway
{

Material::Material()
{
    mRenderState.cullMode = CULL_CCW;
    mRenderState.fillMode = FILL_SOLID;
    mRenderState.colorWrite = CWE_ALL;
    mRenderState.depthWrite = true;
    mRenderState.depthCheck = DCM_LESS_EQUAL;
    mRenderState.blendMode = BM_OPATICY;
    mRenderState.alphaTestRef = 0.5f;

    mEmissive = Color4::Black;
    mAmbient = Color4::Gray;
    mDiffuse = Color4::White;
    mSpecular = Color4::Black;
    mSpecularPower = 80;

    mEmissiveMap = RenderHelper::Instance()->GetBlackTexture();
    mDiffuseMap = RenderHelper::Instance()->GetWhiteTexture();
    mSpecularMap = RenderHelper::Instance()->GetBlackTexture();
    mNormalMap = RenderHelper::Instance()->GetDefaultNormalTexture();
}

Material::~Material()
{
}

void Material::SetEmissive(const Color4 & color)
{
    mEmissive = color;
}

void Material::SetAmbient(const Color4 & color)
{
    mAmbient = color;
}

void Material::SetDiffuse(const Color4 & color)
{
    mDiffuse = color;
}

void Material::SetSpecular(const Color4 & color)
{
    mSpecular = color;
}

void Material::SetEmissiveMap(const TString128 & tex)
{
    mEmissiveMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
}

void Material::SetDiffuseMap(const TString128 & tex)
{
    mDiffuseMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
}

void Material::SetSpecularMap(const TString128 & tex)
{
    mSpecularMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
}

void Material::SetNormalMap(const TString128 & tex)
{
    mNormalMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
}

void Material::SetSpecularPower(float power)
{
    mSpecularPower = power;
}

void Material::SetEmissiveMap(TexturePtr tex)
{
    mEmissiveMap = tex;
}

void Material::SetDiffuseMap(TexturePtr tex)
{
    mDiffuseMap = tex;
}

void Material::SetSpecularMap(TexturePtr tex)
{
    mSpecularMap = tex;
}

void Material::SetNormalMap(TexturePtr tex)
{
    mNormalMap = tex;
}

const Color4 & Material::GetEmissive() const
{
    return mEmissive;
}

const Color4 & Material::GetAmbient() const
{
    return mAmbient;
}

const Color4 & Material::GetDiffuse() const
{
    return mDiffuse;
}

const Color4 & Material::GetSpecular() const
{
    return mSpecular;
}

float Material::GetSpecularPower() const
{
    return mSpecularPower;
}

TexturePtr Material::GetEmissiveMap() const
{
    return mEmissiveMap;
}

TexturePtr Material::GetDiffuseMap() const
{
    return mDiffuseMap;
}

TexturePtr Material::GetSpecularMap() const
{
    return mSpecularMap;
}

TexturePtr Material::GetNormalMap() const
{
    return mNormalMap;
}












MaterialResource::MaterialResource()
{
    mMaterials = NULL;
    mNumMaterials = 0;
}

MaterialResource::~MaterialResource()
{
    safe_delete_array(mMaterials);
    mNumMaterials = 0;
}

void MaterialResource::Alloc(int num)
{
    safe_delete_array(mMaterials);

    mNumMaterials = num;
    mMaterials = new Material[num];
}

Material * MaterialResource::GetMaterial(int index)
{
    d_assert (index < mNumMaterials);
    return &mMaterials[index];
}

int MaterialResource::GetNumMaterials()
{
    return mNumMaterials;
}

void MaterialResource::Load()
{
    if (mLoadState == Resource::LOADED)
        return ;

    ResourceManager::Instance()->GetResourceLoader()->Load(this);
}

void MaterialResource::Reload()
{
    if (mLoadState == Resource::LOADED)
        Unload();

    ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
}

void MaterialResource::Unload()
{
    safe_delete_array(mMaterials);
    mNumMaterials = 0;

    mLoadState = Resource::UNLOADED;
}

void MaterialResource::LoadImp(DataStreamPtr stream)
{
    MaterialLoader loader;
    
    loader.Load(this, stream);

    mLoadState = Resource::LOADED;
}










CULL_MODE GetCullMode(const char * m)
{
    if (strcmp(m, "none") == 0)
        return CULL_NONE;
    else if (strcmp(m, "ccw") == 0)
        return CULL_CCW;
    else if (strcmp(m, "cw") == 0)
        return CULL_CW;
    else 
    {
        LOG_PRINT_FORMAT("unknown cull mode '%s'.\n", m);
        return CULL_NONE;
    }
}

FILL_MODE GetFillMode(const char * m)
{
    if (strcmp(m, "solid") == 0)
        return FILL_SOLID;
    else if (strcmp(m, "frame") == 0)
        return FILL_FRAME;
    else if (strcmp(m, "point") == 0)
        return FILL_POINT;
    else
    {
        LOG_PRINT_FORMAT("unknown fill mode '%s'.\n", m);
        return FILL_SOLID;
    }
}

bool GetBoolen(const char * m)
{
    if (strcmp(m, "true") == 0)
        return true;
    else if (strcmp(m, "false") == 0)
        return false;
    else
    {
        LOG_PRINT_FORMAT("unknown bool '%s'. \n", m);
        return true;
    }
}

DEPTH_CHECK_MODE GetDepthCheck(const char * m)
{
    if (strcmp(m, "less_equal") == 0)
        return DCM_LESS_EQUAL;
    else if (strcmp(m, "less") == 0)
        return DCM_LESS;
    else if (strcmp(m, "greater_equal") == 0)
        return DCM_GREATER_EQUAL;
    else if (strcmp(m, "greater") == 0)
        return DCM_GREATER;
    else if (strcmp(m, "equal"))
        return DCM_EQUAL;
    else if (strcmp(m, "always") == 0)
        return DCM_ALWAYS;
    else
    {
        LOG_PRINT_FORMAT("unknown depth check '%s'.\n", m);
        return DCM_LESS_EQUAL;
    }
}

BLEND_MODE GetBlendMode(const char * m)
{
    if (strcmp(m, "opaticy") == 0)
        return BM_OPATICY;
    else if (strcmp(m, "alpha_blend") == 0)
        return BM_ALPHA_BLEND;
    else if (strcmp(m, "alpha_test") == 0)
        return BM_ALPHA_TEST;
    else if (strcmp(m, "add") == 0)
        return BM_ADD;
    else if (strcmp(m, "multiply") == 0)
        return BM_MULTIPLY;
    else
    {
        LOG_PRINT_FORMAT("unknown blend mode '%s'.\n", m);
        return BM_OPATICY;
    }
}

int MaterialLoader::_getNumMaterials()
{
    int i = 0;
    xml_node * root = doc.first_node("Materials"); 
    xml_node * mat = root->first_node("Material");

    while (mat)
    {
        ++i;

        mat = mat->next_sibling("Material");
    }

    return i;
}

void MaterialLoader::_loadMaterial(Material * mat, xml_node * node)
{
    xml_node * ndCullMode = node->first_node("cull_mode");
    xml_node * ndFillMode = node->first_node("fill_mode");
    xml_node * ndDepthWrite = node->first_node("depth_write");
    xml_node * ndDepthCheck = node->first_node("depth_check");
    xml_node * ndBlendMode = node->first_node("blend_mode");
    xml_node * ndAlphaTestRef = node->first_node("alpha_test_ref");
    xml_node * ndEmissive = node->first_node("emissive");
    xml_node * ndAmbient = node->first_node("ambient");
    xml_node * ndDiffuse = node->first_node("diffuse");
    xml_node * ndSpecular = node->first_node("specular");
    xml_node * ndSpecularPower = node->first_node("specular_power");
    xml_node * ndEmissiveMap = node->first_node("emissive_map");
    xml_node * ndDiffuseMap = node->first_node("diffuse_map");
    xml_node * ndSpecularMap = node->first_node("specular_map");
    xml_node * ndNormalMap = node->first_node("normal_map");

    xml_node * ndShaderProvider = doc.first_node("shader_provider");

    if (ndCullMode)
    {
        xml_attri * attri = ndCullMode->first_attribute("value");
        CULL_MODE mode = GetCullMode(attri->value());
        mat->SetCullMode(mode);
    }

    if (ndFillMode)
    {
        xml_attri * attri = ndFillMode->first_attribute("value");
        FILL_MODE mode = GetFillMode(attri->value());
        mat->SetFillMode(mode);
    }

    if (ndDepthWrite)
    {
        xml_attri * attri = ndFillMode->first_attribute("value");
        bool b = GetBoolen(attri->value());
        mat->SetDepthWrite(b);
    }

    if (ndDepthCheck)
    {
        xml_attri * attri = ndDepthCheck->first_attribute("value");
        DEPTH_CHECK_MODE mode = GetDepthCheck(attri->value());
        mat->SetDepthCheck(mode);
    }

    if (ndBlendMode)
    {
        xml_attri * attri = ndBlendMode->first_attribute("value");
        BLEND_MODE mode = GetBlendMode(attri->value());
        mat->SetBlendMode(mode);
    }

    if (ndAlphaTestRef)
    {
        xml_attri * attri = ndAlphaTestRef->first_attribute("value");
        float v = atof(attri->value());
        mat->SetAlphaTestRef(v);
    }

    if (ndEmissive)
    {
        float r = atof(ndEmissive->first_attribute("r")->value()); 
        float g = atof(ndEmissive->first_attribute("g")->value()); 
        float b = atof(ndEmissive->first_attribute("b")->value());

        mat->SetEmissive(Color4(r, g, b));
    }

    if (ndAmbient)
    {
        float r = atof(ndAmbient->first_attribute("r")->value()); 
        float g = atof(ndAmbient->first_attribute("g")->value()); 
        float b = atof(ndAmbient->first_attribute("b")->value());

        mat->SetAmbient(Color4(r, g, b));
    }

    if (ndDiffuse)
    {
        float r = atof(ndDiffuse->first_attribute("r")->value()); 
        float g = atof(ndDiffuse->first_attribute("g")->value()); 
        float b = atof(ndDiffuse->first_attribute("b")->value());

        mat->SetDiffuse(Color4(r, g, b));
    }

    if (ndSpecular)
    {
        float r = atof(ndSpecular->first_attribute("r")->value()); 
        float g = atof(ndSpecular->first_attribute("g")->value()); 
        float b = atof(ndSpecular->first_attribute("b")->value());

        mat->SetSpecular(Color4(r, g, b));
    }

    if (ndSpecularPower)
    {
        float v = atof(ndSpecularPower->first_attribute("value")->value());
        mat->SetSpecularPower(v);
    }

    if (ndEmissiveMap)
    {
        const char * t = ndEmissiveMap->first_attribute("filename")->value();
        mat->SetEmissiveMap(t);
    }

    if (ndDiffuseMap)
    {
        const char * t = ndDiffuseMap->first_attribute("filename")->value();
        mat->SetDiffuseMap(t);
    }

    if (ndSpecularMap)
    {
        const char * t = ndSpecularMap->first_attribute("filename")->value();
        mat->SetSpecularMap(t);
    }

    if (ndNormalMap)
    {
        const char * t = ndNormalMap->first_attribute("filename")->value();
        mat->SetNormalMap(t);
    }
}


void MaterialLoader::Load(MaterialResource * res, DataStreamPtr stream)
{
    doc.parse<0>((char *)stream->GetData());

    int num = _getNumMaterials();
    int count = num;
    int i = 0;

    xml_node * root = doc.first_node("Materials"); 
    xml_node * node = root->first_node("Material");

    res->Alloc(num);

    d_assert (count > 0);

    while (node)
    {
        Material * mat = &res->mMaterials[i++];

        _loadMaterial(mat, node);

        node = node->next_sibling("Material");
    }

    d_assert (i == count);
}


}


#pragma warning(pop)

