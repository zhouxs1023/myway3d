#include "MWShaderProgramDefMgr.h"
#include "MWResourceManager.h"
#include "MWScript.h"

using namespace Myway;

#define program_b                       "[program]"
#define program_name                    "name"
#define programile                    "file"
#define programunc                    "func"
#define programrofile                 "profile"
#define program_language                "language"

#define profile_vs_1_1                  "vs_1_1"
#define profile_vs_2_0                  "vs_2_0"
#define profile_vs_2_x                  "vs_2_x"
#define profile_vs_2_sw                 "vs_2_sw"
#define profile_vs_3_0                  "vs_3_0"

#define profile_ps_1_1                  "ps_1_1"
#define profile_ps_1_2                  "ps_1_2"
#define profile_ps_1_3                  "ps_1_3"
#define profile_ps_2_0                  "ps_2_0"
#define profile_ps_2_x                  "ps_2_x"
#define profile_ps_2_sw                 "ps_2_sw"
#define profile_ps_3_0                  "ps_3_0"

#define language_cg                     "cg"
#define language_hlsl                   "hlsl"
#define language_glsl                   "glsl"

#define shader_param                    "param"
#define param_custom                    "custom"
#define paramnt1                      "int1"
#define paramnt2                      "int2"
#define paramnt3                      "int3"
#define paramnt4                      "int3"
#define paramloat1                    "float1"
#define paramloat2                    "float2"
#define paramloat3                    "float3"
#define paramloat4                    "float4"
#define paramMat3x3                    "mat3x3"
#define paramMat4x4                    "mat4x4"
#define paramMat_w                     "mat_world"
#define paramMat_v                     "mat_view"
#define paramMat_p                     "mat_proj"
#define paramMat_wv                    "mat_worldview"
#define paramMat_vp                    "mat_viewproj"
#define paramMat_wvp                   "mat_worldviewproj"
#define paramMat_shadow_v              "mat_shadow_view"
#define paramMat_shadow_p              "mat_shadow_proj"
#define paramMat_shadow_vp             "mat_shadow_viewproj"
#define paramMat_shadow_wv             "mat_shadow_worldview"
#define paramMat_shadow_wvp            "mat_shadow_worldviewproj"
#define paramMat_blend                 "mat_blend"
#define paramMat_proj                  "mat_projection"
#define paramMtrl_ambient              "mtl_ambient"
#define paramMtrl_emissive             "mtl_emissive"
#define paramMtrl_diffuse              "mtl_diffuse"
#define paramMtrl_specular             "mtl_specular"
#define paramMtrl_power                "mtl_power"
#define param_light_ambient             "light_ambient"
#define param_light_diffuse             "light_diffuse"
#define param_light_specular            "light_specular"
#define param_light_position            "light_position"
#define param_light_direction           "light_direction"
#define param_light_attenuation         "light_attenuation"
#define param_light_spot_params         "light_spot_params"
#define param_camera_position           "camera_position"
#define param_camera_direction          "camera_direction"
#define param_camera_right              "camera_right"
#define param_camera_up                 "camera_up"
#define param_scene_ambeint             "scene_ambient"

#define param_texture_size              "texture_size"
#define paramnv_texture_size          "inv_texture_size"

#define paramog_param                 "fog_param"
#define paramog_color                 "fog_color"

#define param_time                      "time"
#define param_sin_time                  "sin_time"
#define param_cos_time                  "cos_time"
#define param_tan_time                  "tan_time"
#define paramrame_time                "frame_time"
#define param_time_2pi                  "time_2pi"
#define paramps                       "fps"
#define param_user                      "user"


SHADER_PARAMETER_TYPE GetType(const String & type)
{
    if (type == paramnt1)
        return SPT_INT1;

    else if (type == paramnt2)
        return SPT_INT2;

    else if (type == paramnt3)
        return SPT_INT3;

    else if (type == paramnt4)
        return SPT_INT4;

    else if (type == paramloat1)
        return SPT_FLOAT1;

    else if (type == paramloat2)
        return SPT_FLOAT2;

    else if (type == paramloat3)
        return SPT_FLOAT3;

    else if (type == paramloat4)
        return SPT_FLOAT4;

    else if (type == paramMat3x3)
        return SPT_MATRIX3x3;

    else if (type == paramMat4x4)
        return SPT_MATRIX4x4;

    else
    {
        LOG_PRINT("Parse Shader Program Define Error: unkown type " + type + "\n");
        return SPT_UNKNOWN;
    }
}

AUTO_SHADER_PARAMETER_TYPE GetAutoType(const String & type)
{
    if (type == paramMat_w)
        return ASPT_WORLD_MATRIX;
    else if (type == paramMat_v)
        return ASPT_VIEW_MATRIX;
    else if (type == paramMat_p)
        return ASPT_PROJ_MATRIX;
    else if (type == paramMat_wv)
        return ASPT_WORLD_VIEW_MATRIX;
    else if (type == paramMat_vp)
        return ASPT_VIEW_PROJ_MATRIX;
    else if (type == paramMat_wvp)
        return ASPT_WORLD_VIEW_PROJ_MATRIX;
    else if (type == paramMat_blend)
        return ASPT_BLEND_MATRIX;
    else if (type == paramMat_proj)
        return ASPT_PROJECTION_MATRIX;

    else if (type == paramMat_shadow_v)
        return ASPT_SHADOW_VIEW_MATRIX;
    else if (type == paramMat_shadow_p)
        return ASPT_SHADOW_PROJ_MATRIX;
    else if (type == paramMat_shadow_wv)
        return ASPT_SHADOW_WORLD_VIEW_MATRIX;
    else if (type == paramMat_shadow_vp)
        return ASPT_SHADOW_VIEW_PROJ_MATRIX;
    else if (type == paramMat_shadow_wvp)
        return ASPT_SHADOW_WORLD_VIEW_PROJ_MATRIX;

    else if (type == paramMtrl_ambient)
        return ASPT_MATERIAL_AMBIENT;
    else if (type == paramMtrl_emissive)
        return ASPT_MATERIAL_EMISSIVE;
    else if (type == paramMtrl_diffuse)
        return ASPT_MATERIAL_DIFFUSE;
    else if (type == paramMtrl_specular)
        return ASPT_MATERIAL_SPECULAR;
    else if (type == paramMtrl_power)
        return ASPT_MATERIAL_POWER;

    else if (type == param_light_ambient)
        return ASPT_LIGHT_AMBIENT;
    else if (type == param_light_diffuse)
        return ASPT_LIGHT_DIFFUSE;
    else if (type == param_light_specular)
        return ASPT_LIGHT_SPECULAR;
    else if (type == param_light_position)
        return ASPT_LIGHT_POSITION;
    else if (type == param_light_direction)
        return ASPT_LIGHT_DIRECTION;
    else if (type == param_light_attenuation)
        return ASPT_LIGHT_ATTENUATION;
    else if (type == param_light_spot_params)
        return ASPT_LIGHT_SPOT_PARAMS;

    else if (type == param_camera_position)
        return ASPT_CAMERA_POSITION;
    else if (type == param_camera_direction)
        return ASPT_CAMERA_DIRECTION;
    else if (type == param_camera_right)
        return ASPT_CAMERA_RIGHT;
    else if (type == param_camera_up)
        return ASPT_CAMERA_UP;

    else if (type == param_texture_size)
        return ASPT_TEXTURE_SIZE;
    else if (type == paramnv_texture_size)
        return ASPT_INV_TEXTURE_SIZE;

    else if (type == paramog_param)
        return ASPT_FOG_PARAM;
    else if (type == paramog_color)
        return ASPT_FOG_COLOR;

    else if (type == param_time)
        return ASPT_TIME;
    else if (type == param_sin_time)
        return ASPT_SIN_TIME;
    else if (type == param_cos_time)
        return ASPT_COS_TIME;
    else if (type == param_tan_time)
        return ASPT_TAN_TIME;
    else if (type == param_time_2pi)
        return ASPT_TIME_2PI;
    else if (type == paramrame_time)
        return ASPT_FRAME_TIME;
    else if (type == paramps)
        return ASPT_FPS;
    else if (type == param_user)
        return ASPT_USER;

    else if (type == param_scene_ambeint)
        return ASPT_SCENE_AMBIENT;

    else if (type == param_custom)
        return ASPT_UNKNOWN;

    else
    {
        EXCEPTION("Parse Shader Program Define Error: unkown auto type " + type);
        return ASPT_UNKNOWN;
    }
}

bool IsCustom(const String & type)
{
    return type == paramnt1 || type == paramnt2 || type == paramnt3 || type == paramnt4 ||
        type == paramloat1 || type == paramloat2 || type == paramloat3 || type == paramloat4 ||
        type == paramMat3x3 || type == paramMat4x4;
}


ShaderProgramDefMgr::ShaderProgramDefMgr()
{
}

ShaderProgramDefMgr::~ShaderProgramDefMgr()
{
    Unload();
}

void ShaderProgramDefMgr::Load(const String & sFile, bool bAppend)
{
    if (!bAppend)
        m_PromgramDefines.Clear();

    DataStreamPtr stream = ResourceManager::Instance().OpenResource(sFile);

    if (stream.IsNull())
    {
        EXCEPTION("Stream error: " + sFile);
    }

    CommandScript cst;
    cst.LoadStream(stream);

    String cmd;

    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();

            if (program_b == cmd)
            {
               ParseDefine(cst);
            }
            else if ("" == cmd)
            {
            }
            else
            {
                EXCEPTION("program config file error: unknow command '" + cmd + "'.");
            }
        }
    }

    cst.Shutdown();
}

void ShaderProgramDefMgr::ParseDefine(CommandScript & cst)
{
    String cmd;
    String param;

    ShaderProgramDefine * spd = new ShaderProgramDefine();

    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();
           
            //name
            if (cmd == program_name)
            {
                EXCEPTION_DEBUG(!GetShaderProgramDefine(param), "shader program " + param + " redefine.");
                spd->name = param;
            }

            //file
            else if (cmd == programile)
            {
                spd->source = param;
            }

            //func
            else if (cmd == programunc)
            {
                spd->entry = param;
            }

            //profile
            else if (cmd == programrofile)
            {
                spd->profile = GetProfile(param);
            }

            //language
            else if (cmd == program_language)
            {
                spd->language = GetLanguage(param);
            }

            //param
            else if (cmd == shader_param)
            {
                ShaderParamDefine def;
                def.name = param;

                ParseParam(def, cst);

                assert (def.type != SPT_UNKNOWN);
                spd->params.PushBack(def);
            }

            else if (cmd == "")
            {

            }
            
            else if (cmd == program_b)
            {
                cst.MoveToPreLine();
                break;
            }
            else
            {
                EXCEPTION("program config file error: unknow command '" + cmd + "'.");
            }
        }
    }

    m_PromgramDefines.PushBack(spd);
}

void ShaderProgramDefMgr::ParseParam(ShaderParamDefine & def, CommandScript & cst)
{
    String param1, param2;
    param1 = cst.ParseStringParam();
    param2 = cst.ParseStringParam();

    def.type = GetType(param1);
    def.autotype = GetAutoType(param2);

#define __PARSE_INT_VAL(x, v)               \
    for (int m = 0; m < x;++m)           \
    {                                       \
        param = cst.ParseStringParam();     \
        (v)[m] = param.ToInt();             \
    }

#define __PARSE_FLOAT_VAL(x, v)             \
    for (int m = 0; m < x;++m)           \
    {                                       \
        param = cst.ParseStringParam();     \
        (v)[m] = param.ToFloat();           \
    }                      

    //custom.
    if (def.autotype == ASPT_UNKNOWN)
    {
        String param;

        switch (def.type)
        {
        case SPT_FLOAT1:
            __PARSE_FLOAT_VAL(1, (float *)&def.val);
            break;

        case SPT_FLOAT2:
            __PARSE_FLOAT_VAL(2, (float*)&def.val);
            break;

        case SPT_FLOAT3:
            __PARSE_FLOAT_VAL(3, (float*)&def.val);
            break;

        case SPT_FLOAT4:
            __PARSE_FLOAT_VAL(4, (float*)&def.val);
            break;

        default:
            EXCEPTION("parse shader config file error: custom param must be float1 - float4.");
        }
        String param3, param4;
        param3 = cst.ParseStringParam();
        param4 = cst.ParseStringParam();
        def.autoindex = 0;
        def.count = param3.ToInt();
        def.reg = param4.ToInt();
        def.count = def.count ? def.count : 1;
    }
    else
    {
        String param3, param4, param5;
        param3 = cst.ParseStringParam();
        param4 = cst.ParseStringParam();
        param5 = cst.ParseStringParam();
        def.autoindex = param3.ToInt();
        def.count = param4.ToInt();
        def.reg = param5.ToInt();
        def.count = def.count ? def.count : 1;
    }


#undef __PARSE_INT_VAL
#undef __PARSE_FLOAT_VAL
}

void ShaderProgramDefMgr::Unload()
{
    List<ShaderProgramDefine*>::Iterator iter;
    List<ShaderProgramDefine*>::Iterator end;

    iter = m_PromgramDefines.Begin();
    end = m_PromgramDefines.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }
    
    m_PromgramDefines.Clear();
}

const ShaderProgramDefine * ShaderProgramDefMgr::GetShaderProgramDefine(const String & name)
{
    List<ShaderProgramDefine*>::Iterator iter;
    List<ShaderProgramDefine*>::Iterator end;

    iter = m_PromgramDefines.Begin();
    end = m_PromgramDefines.End();

    while (iter != end && (*iter)->name != name)
    {
        ++iter;
    }

    return iter != end ? (*iter) : NULL;
}

SHADER_PROGRAM_PROFILE ShaderProgramDefMgr::GetProfile(const String & s)
{
    if (s == profile_vs_1_1)
        return SHADER_PROFILE_VS_1_1;

    else if (s == profile_vs_2_0)
        return SHADER_PROFILE_VS_2_0;

    else if (s == profile_vs_3_0)
        return SHADER_PROFILE_VS_3_0;

    else if (s == profile_ps_1_1)
        return SHADER_PROFILE_PS_1_1;

    else if (s == profile_ps_1_2)
        return SHADER_PROFILE_PS_1_2;

    else if (s == profile_ps_1_3)
        return SHADER_PROFILE_PS_1_3;

    else if (s == profile_ps_2_0)
        return SHADER_PROFILE_PS_2_0;

    else if (s == profile_ps_3_0)
        return SHADER_PROFILE_PS_3_0;
    else
    {
        EXCEPTION("Parse Shader Program Define Error: unkown profile " + s);
        return SHADER_PROFILE_UNKNOWN;
    }
}

SHADER_PROGRAM_LANGUAGE ShaderProgramDefMgr::GetLanguage(const String & s)
{
    if (s == language_cg)
        return SHADER_LANGUAGE_CG;
    if (s == language_hlsl)
        return SHADER_LANGUAGE_HLSL;
    else
    {
        EXCEPTION("Parse Shader Program Define Error: unkown language " + s);
        return SHADER_LANGUAGE_UNKNOWN;
    }
}