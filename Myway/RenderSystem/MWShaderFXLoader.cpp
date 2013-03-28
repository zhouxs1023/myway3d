#include "MWShaderFXLoader.h"
#include "MWShaderFX.h"

namespace Myway
{
    SHADER_PARAM_TYPE GetShaderParamType(const char * type)
    {
        if (strcmp(type, "float4") == 0)
            return SPT_FLOAT4;
        else if (strcmp(type, "float3x4") == 0)
            return SPT_MATRIX3x4;
        else if (strcmp(type, "float4x4") == 0)
            return SPT_MATRIX4x4;
        else
            LOG_PRINT_FORMAT("unknown shader param '%s'.\n", type);

        return SPT_FLOAT4;
    }

    SHADER_PARAM_BIND_TYPE GetShaderParamBindType(const char * type)
    {
        if (strcmp(type, "matWorld") == 0)
            return SPBT_WORLD_MATRIX;
        else if (strcmp(type, "matView") == 0)
            return SPBT_VIEW_MATRIX;
        else if (strcmp(type, "matProj") == 0)
            return SPBT_PROJ_MATRIX;
        else if (strcmp(type, "matVP") == 0)
            return SPBT_VIEW_PROJ_MATRIX;
        else if (strcmp(type, "matWVP") == 0)
            return SPBT_WORLD_VIEW_PROJ_MATRIX;
		else if (strcmp(type, "matWV") == 0)
			return SPBT_WORLD_VIEW_MATRIX;
        else if (strcmp(type, "matBlend") == 0)
            return SPBT_BLEND_MATRIX;

		else if (strcmp(type, "mirrorColor") == 0)
			return SPBT_MIRROR_COLOR;

        else if (strcmp(type, "fogParam") == 0)
            return SPBT_FOG_PARAM;
        else if (strcmp(type, "fogColor") == 0)
            return SPBT_FOG_COLOR;

        else if (strcmp(type, "mtlAmbient") == 0)
            return SPBT_MATERIAL_AMBIENT;
        else if (strcmp(type, "mtlDiffuse") == 0)
            return SPBT_MATERIAL_DIFFUSE;
        else if (strcmp(type, "mtlSpecular") == 0)
            return SPBT_MATERIAL_SPECULAR;

        else if (strcmp(type, "lgPosition") == 0)
            return SPBT_LIGHT_POSITION;
        else if (strcmp(type, "lgDirection") == 0)
            return SPBT_LIGHT_DIRECTION;
        else if (strcmp(type, "lgAttenuation") == 0)
            return SPBT_LIGHT_ATTENUATION;
        else if ((strcmp(type, "lgSpotParam")) == 0)
            return SPBT_LIGHT_SPOT_PARAM;

        else if (strcmp(type, "texSize") == 0)
            return SPBT_TEXTURE_SIZE;
        else if (strcmp(type, "invTexSize") == 0)
            return SPBT_INV_TEXTURE_SIZE;

        else if (strcmp(type, "time") == 0)
            return SPBT_TIME;

        else if (strcmp(type, "clipPlane") == 0)
            return SPBT_CLIP_PANE;

        else if (strcmp(type, "user") == 0)
            return SPBT_USER;

        else if (strcmp(type, "none") == 0)
            return SPBT_UNKNOWN;
        else
            LOG_PRINT_FORMAT ("unknown shader bind type '%s'.\n", type);

        return SPBT_UNKNOWN;
    }

    SHADER_LANGUAGE GetShaderLanguage(const char * lan)
    {
        if (strcmp(lan, "hlsl") == 0)
            return SL_HLSL;
        else if (strcmp(lan, "cg") == 0)
            return SL_CG;
        else if (strcmp(lan, "asm") == 0)
            return SL_ASM;
        else if (strcmp(lan, "precompiled") == 0)
            return SL_PRECOMPILED;
        else
            LOG_PRINT_FORMAT ("unknown shader language '%s'\n", lan);

        return SL_CG;
    }

    SHADER_PROFILE GetShaderProfile(const char * pro)
    {
        if (strcmp(pro, "vs20") == 0)
            return SP_VS_2_0;
        else if (strcmp(pro, "vs30") == 0)
            return SP_VS_3_0;
        else if (strcmp(pro, "ps20") == 0)
            return SP_PS_2_0;
        else if (strcmp(pro, "ps30") == 0)
            return SP_PS_3_0;
        else
            LOG_PRINT_FORMAT ("unknown shader profile '%s'./n", pro);

        return SP_UNKNOWN;
    }

    Vec4 GetShaderParamValue(const char * val)
    {
        float x = 0, y = 0, z = 0, w = 0;

        sscanf_s(val, "%f,%f,%f,%f", &x, &y, &z, &w);

        return Vec4(x, y, z, w);

    }

    void LoadShader(xml_node * nd, ShaderLib * lib)
    {
        xml_attri * aName = nd->first_attribute("name");
        xml_attri * aLanguage = nd->first_attribute("language");
        xml_attri * aProfile = nd->first_attribute("profile");
        xml_attri * aSource = nd->first_attribute("source");

        d_assert (aName && aLanguage && aProfile && aSource);

        xml_node * np = nd->first_node("param");
        List<ShaderParamDesc> params;

        while (np)
        {
            ShaderParamDesc desc;

            xml_attri * name = np->first_attribute("name");
            xml_attri * type = np->first_attribute("type");
            xml_attri * btype = np->first_attribute("btype");
            xml_attri * bindex = np->first_attribute("bindex");
            xml_attri * count = np->first_attribute("count");
            xml_attri * reg = np->first_attribute("register");
            xml_attri * val = np->first_attribute("value");

            d_assert (name && type);

            desc.name = name->value();
            desc.type = GetShaderParamType(type->value());
            desc.btype = btype ? GetShaderParamBindType(btype->value()) : SPBT_UNKNOWN;
            desc.bindex = bindex ? atoi(bindex->value()) : 0;
            desc.count = count ? atoi(count->value()) : 1;
            desc.reg = reg ? atoi(reg->value()) : 0;
            desc.val = val ? GetShaderParamValue(val->value()) : Vec4(0, 0, 0, 0);

            params.PushBack(desc);

            np = np->next_sibling("param");
        }

        {
            const char * name = aName->value();
            SHADER_LANGUAGE lan = GetShaderLanguage(aLanguage->value());
            SHADER_PROFILE pro = GetShaderProfile(aProfile->value());
            TString128 source = aSource->value();
            const char * entry = "main";

			TString128 path = File::GetFileDir(lib->GetSource());

			if (path != "")
				source = path + "\\" + source;

            ShaderProgram * shader = lib->AddShader(name, lan, pro, source.c_str(), entry, &params);
            d_assert (shader);
        }
    }

    SamplerBindType GetSamplerBindType(const char * btype)
    {
        if (strcmp(btype, "emissive") == 0)
            return SBT_Emissive;
        else if (strcmp(btype, "diffuse") == 0)
            return SBT_Diffuse;
        else if (strcmp(btype, "normal") == 0)
            return SBT_Normal;
        else if (strcmp(btype, "specular") == 0)
            return SBT_Specular;
        else if (strcmp(btype, "shadow") == 0)
            return SBT_Shadow;
        else if (strcmp(btype, "scene") == 0)
            return SBT_Scene;
        else if (strcmp(btype, "extern0") == 0)
            return SBT_Extern0;
        else if (strcmp(btype, "extern1") == 0)
            return SBT_Extern1;
        else if (strcmp(btype, "extern2") == 0)
            return SBT_Extern2;
        else if (strcmp(btype, "extern3") == 0)
            return SBT_Extern3;
        else if (strcmp(btype, "extern4") == 0)
            return SBT_Extern4;
        else if (strcmp(btype, "extern5") == 0)
            return SBT_Extern5;
        else if (strcmp(btype, "extern6") == 0)
            return SBT_Extern6;
        else if (strcmp(btype, "extern7") == 0)
            return SBT_Extern7;
        else
            LOG_PRINT_FORMAT("unknown sampler bind type '%s'.\n", btype);

        return SBT_Diffuse;
    }

    TEXTURE_FILTER_TYPE GetSamplerFilterType(const char * type)
    {
        if (strcmp(type, "default") == 0)
            return TEXF_DEFAULT;
        else if (strcmp(type, "point") == 0)
            return TEXF_POINT;
        else if (strcmp(type, "linear") == 0)
            return TEXF_LINEAR;
        else if (strcmp(type, "anisotropic") == 0)
            return TEXF_ANISOTROPIC;
        else
            LOG_PRINT_FORMAT("unknown sampler filter type '%s'.\n", type);

        return TEXF_LINEAR;
    }

    TEXTURE_ADDRESS_MODE GetSamplerAddressType(const char * type)
    {
        if (strcmp(type, "wrap") == 0)
            return TEXA_WRAP;
        else if (strcmp(type, "mirror") == 0)
            return TEXA_MIRROR;
        else if (strcmp(type, "clamp") == 0)
            return TEXA_CLAMP;
        else if (strcmp(type, "border") == 0)
            return TEXA_BORDER;
        else
            LOG_PRINT_FORMAT("unknown sampler address type '%s'.\n", type);

        return TEXA_WRAP;
    }

    Color GetSamplerBorderColor(const char * cr)
    {
        int r = 0, g = 0, b = 0, a = 0;
        sscanf_s(cr, "%d,%d,%d,%d", &r, &g, &b, &a);
        return Color(r, g, b, a);
    }

    void LoadTechnique(xml_node * nd, ShaderLib * lib)
    {
        xml_attri * aname = nd->first_attribute("name");
        xml_attri * avshader = nd->first_attribute("vs");
        xml_attri * apshader = nd->first_attribute("ps");

        d_assert (aname && avshader && apshader);

        const char * name = aname->value();
        const char * vshader = avshader->value();
        const char * pshader = apshader->value();

        Technique * tech = lib->AddTechnique(name);
        tech->SetVertexShader(vshader);
        tech->SetPixelShader(pshader);

        nd = nd->first_node("sampler");

        while (nd)
        {
            xml_attri * aIndex = nd->first_attribute("index");
            xml_attri * aBindType = nd->first_attribute("btype");
            xml_attri * aFilter = nd->first_attribute("filter");
            xml_attri * aAddress = nd->first_attribute("address");
            xml_attri * aBorder = nd->first_attribute("border");
            xml_attri * aMaxMipLevel = nd->first_attribute("mlevel");

            d_assert (aIndex && aBindType);

            int index = atoi(aIndex->value());

            SamplerState & state = tech->GetSamplerState(index);

            state.BindType = GetSamplerBindType(aBindType->value());
            state.Filter = aFilter ? GetSamplerFilterType(aFilter->value()) : TEXF_DEFAULT;
            state.Address = aAddress ? GetSamplerAddressType(aAddress->value()) : TEXA_WRAP;
            state.BorderColor = aBorder ? GetSamplerBorderColor(aBorder->value()) : Color::Black;
            state.MaxMipLevel = aMaxMipLevel ? atoi(aMaxMipLevel->value()) : 0;

            nd = nd->next_sibling("sampler");
        }
    }








    void ShaderLibLoader::Load(ShaderLib * lib, DataStreamPtr stream)
    {
        xml_doc doc;

        doc.parse<0>((char *)stream->GetData());

        xml_node * root = doc.first_node();

        // shader
        {
            xml_node * ndp = root->first_node("Shaders");
            xml_node * nd = ndp->first_node("Shader");
            int count = 0;

            while (nd)
            {
                ++count;
                LoadShader(nd, lib);
                nd = nd->next_sibling("Shader");
            }
        }

        // technique
        {
            xml_node * ndp = root->first_node("Techniques");
            xml_node * nd = ndp->first_node("Technique");

            while (nd)
            {
                LoadTechnique(nd, lib);
                nd = nd->next_sibling("Technique");
            }
        }

        stream->Close();
    }
}