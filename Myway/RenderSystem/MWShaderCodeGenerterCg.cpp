#include "MWShaderCodeGenerterCg.h"
#include "MWShaderInstance.h"
#include "MWShaderNodeSampler2D.h"
#include "MWShaderNodeAdd.h"
#include "MWShaderNodeSub.h"
#include "MWShaderNodeMultiply.h"
#include "MWShaderNodeClamp.h"
#include "MWShaderNodeConstFloat1.h"
#include "MWShaderNodeConstFloat2.h"
#include "MWShaderNodeConstFloat3.h"
#include "MWShaderNodeConstFloat4.h"
#include "MWShaderNodeParamFloat4.h"

#include "MWShaderProgramManager.h"

using namespace Myway;
using namespace Shader;

//////////////////////////////////////////////////////////////////////////
//
//
//  directional lighting
//      light_pos.w             <-  0
//      light_dir.w             <-  1
//      light_atten.x           <-  max float   // max attenuation distance
//      light_atten.y           <-  1           // atten0
//      light_atten.z           <-  0           // atten1
//      light_atten.w           <-  0           // atten1
//      light_spot_params.x     <-  1           // cosInner
//      light_spot_params.y     <-  -2          // cosOuter
//      light_spot_params.z     <-  0           // falloff

//
//  point lighting
//      light_pos.w             <-  1
//      light_dir.w             <-  0
//      light_spot_params.x     <-  1           // cosInner
//      light_spot_params.y     <-  -2          // cosOuter
//      light_spot_params.z     <-  0           // falloff

//
//  spot lighting
//      light_pos.w             <-  0
//      light_dir.w             <-  1
//
//
//
const char lighting[] =                                      
"                                                                                                              \n"\
"void CalcuLighting(float4 position,                                                                           \n"\
"                   float4 eye,                                                                                \n"\
"                   float4 light_pos,                                                                          \n"\
"                   float4 light_dir,                                                                          \n"\
"                   float4 light_atten,                                                                        \n"\
"                   float4 light_spot,                                                                         \n"\
"                   out float3 olightdir,                                                                      \n"\
"                   out float3 oview,                                                                          \n"\
"                   out float oatten)                                                                          \n"\
"{                                                                                                             \n"\
"    vec3 view = eye.xyz - position.xyz;                                                                       \n"\
"                                                                                                              \n"\
"    // calculate light direction                                                                              \n"\
"    vec3 lightdir1 = light_pos.xyz - position.xyz;                                                            \n"\
"    vec3 lightdir2 = -light_dir.xyz;                                                                          \n"\
"    olightdir = lightdir1 * light_pos.w + lightdir2 * light_dir.w;                                            \n"\
"                                                                                                              \n"\
"    // calculate attenuation                                                                                  \n"\
"    float len = length(lightdir1);                                                                            \n"\
"    float atten = 1.0f / (light_atten.y + light_atten.z * len + light_atten.w * len * len);                   \n"\
"    atten = len < light_atten.x ? atten : 0;                                                                  \n"\
"                                                                                                              \n"\
"    float rho = dot(lightdir2, normalize(lightdir1));                                                         \n"\
"    float spot = saturate((rho - light_spot.y) / (light_spot.x - light_spot.y));                              \n"\
"    spot = pow(spot, light_spot.z);                                                                           \n"\
"                                                                                                              \n"\
"    oatten = atten * spot;                                                                                    \n"\
"    oview  = view;                                                                                            \n"\
"}                                                                                                             \n"\
"                                                                                                              \n";

const char fogging[] = 
"                                                                                                              \n"\
"float fogging(float3 pos, float3 cam, float start, float end, float density, float weight)                    \n"\
"{                                                                                                             \n"\
"   float d = length(pos - cam);                                                                               \n"\
"   float linear = saturate((end - d) / (end - start));                                                      \n"\
"   float exp1 = saturate(1 / exp(d * density));                                                               \n"\
"   return 1.0f - (linear + weight * (exp1 - linear));                                                                  \n"\
"}                                                                                                             \n"\
"                                                                                                              \n";




const char vertex_shader_head[] =
"                                                                                                              \n"\
"in float4 position             : POSITION;                                                                    \n"\
"in float4 color                : COLOR;                                                                       \n"\
"in float3 normal               : NORMAL;                                                                      \n"\
"in float3 tangent              : TANGENT;                                                                     \n"\
"in float2 texcoord             : TEXCOORD0;                                                                   \n"\
"in int4   indices              : BLENDINDICES;                                                                \n"\
"in float4 weights              : BLENDWEIGHT;                                                                 \n"\
"                                                                                                              \n"\
"varying out float4 oposition    : POSITION;                                                                   \n"\
"varying out float4 ocolor       : COLOR;                                                                      \n"\
"varying out float  ofog         : FOG;                                                                        \n"\
"varying out float2 otexcoord    : TEXCOORD0;                                                                  \n"\
"varying out float3 onormal      : TEXCOORD1;                                                                  \n"\
"varying out float3 olightdir    : TEXCOORD2;                                                                  \n"\
"varying out float3 oview        : TEXCOORD3;                                                                  \n"\
"varying out float  oatten       : TEXCOORD4;                                                                  \n"\
"varying out float4 opos         : TEXCOORD5;                                                                  \n"\
"                                                                                                              \n"\
"uniform float4x4 mat_world;                                                                                   \n"\
"uniform float4x4 mat_worldviewproj;                                                                           \n"\
"uniform float4x4 mat_bone[50];                                                                                \n"\
"uniform float4 camera_position;                                                                               \n"\
"uniform float4 light_position;                                                                                \n"\
"uniform float4 light_direction;                                                                               \n"\
"uniform float4 light_attenuation;                                                                             \n"\
"uniform float4 light_spot;                                                                                    \n"\
"uniform float4 fog_param;                                                                                     \n"\
"                                                                                                              \n"\
"void main(void)                                                                                               \n"\
"{                                                                                                             \n";

const char transform_no_skel[] = 
"    float4 _position = mul(position, mat_world);                                                              \n"\
"    float3 _normal = mul(normal, (float3x3)mat_world);                                                        \n"\
"    float3 _tangent = mul(tangent, (mat3x3)mat_world);                                                        \n"\
"    _normal = normalize(_normal);                                                                             \n"\
"    _tangent = normalize(_tangent);                                                                           \n"\
"    float3 _binormal = cross(_normal, _tangent);                                                              \n"\
"    float3x3 _tbn = float3x3(_tangent, _binormal, _normal);                                                   \n";


const char transform_has_skel[] =
"    float4x4 _matblend = mat_bone[indices.x] * weights.x +                                                    \n"\
"                         mat_bone[indices.y] * weights.y +                                                    \n"\
"                         mat_bone[indices.z] * weights.z +                                                    \n"\
"                         mat_bone[indices.w] * weights.w;                                                     \n"\
"    position = mul(position, _matblend);                                                                      \n"\
"    normal = mul(normal, (float3x3)_matblend);                                                                \n"\
"    tangent = mul(tangent, (mat3x3)_matblend);                                                                \n"\
"                                                                                                              \n";



const char vertex_shader_end[] = 
"}                                                                                                             \n";



const char pixel_shader_head[] =
"\n                                             \n"\
"in float4 color                : COLOR;        \n"\
"in float  fog                  : FOG;          \n"\
"in float2 texcoord             : TEXCOORD0;    \n"\
"in float3 normal               : TEXCOORD1;    \n"\
"in float3 lightdir             : TEXCOORD2;    \n"\
"in float3 view                 : TEXCOORD3;    \n"\
"in float atten                 : TEXCOORD4;    \n"\
"in float4 position             : TEXCOORD5;    \n"\
"                                               \n"\
"varying out float4 ocolor      : COLOR;        \n"\
"                                               \n"\
"uniform float4 scene_ambient;                  \n"\
"uniform float4 fog_color;                      \n"\
"uniform float4 light_diffuse;                  \n"\
"uniform float4 light_specular;                 \n"\
"                                               \n";

const char pixel_shader_begin[] =
"void main(void)                                \n"\
"{                                              \n"\
"    float3 __lightdir = normalize(lightdir);   \n"\
"    float3 __view = normalize(view);           \n"\
"    float3 __normal = normalize(normal);       \n"\
"    float3 __reflect = 0;                      \n"\
"    float4 __diffuse = 1;                      \n"\
"    float4 __specular = 0;                     \n"\
"    float4 __final = 0;                        \n"\
"    float  __NdL = 0, __RdV = 0;               \n"\
"    float4 __shadow = 1;                       \n"\
"    float2 __scoord = 0;                       \n"\
"                                               \n";


const char pixel_shader_end[] = 
"                                               \n"\
"   ocolor = __final;                           \n"\
"}                                              \n"\
"                                               \n";


ST_Code_Generter_Cg::ST_Code_Generter_Cg()
{
}

ST_Code_Generter_Cg::~ST_Code_Generter_Cg()
{
}

void ST_Code_Generter_Cg::GenerteShader(ST_Instance * inst)
{
    GenVertexShader(inst);
    GenPixelShader(inst);
}

void ST_Code_Generter_Cg::GenPixelShader(ST_Instance * inst)
{
    String ps_code;

    FOG_MODE fm = inst->GetFogMode();
    LIGHTING_MODE lm = inst->GetLightingMode();

    ST_Node * diffuse = inst->GetDiffuse();
    ST_Node * diffuse_power = inst->GetDiffusePower();
    ST_Node * specular = inst->GetSpecular();
    ST_Node * specular_power = inst->GetSpecularPower();
    ST_Node * normal = inst->GetNormal();
    ST_Node * transparency = inst->GetTransparency();

    ps_code += pixel_shader_head;

    if (inst->GetShadowEnable())
    {
        ps_code += "uniform sampler2D __shadowMap;\n";
    }

    ST_Instance::NodeVisitor v = inst->GetAllNodes();

    while (!v.Endof())
    {
        ST_Node * node = v.Cursor()->second;
        ps_code += node->GetUniformParamDefine() + "\n";

        ++v;
    }

    ps_code += pixel_shader_begin;

    // shadow
    if (inst->GetShadowEnable())
    {
        ps_code += "    __scoord = (position.xy / position.w) * float2(0.5f, -0.5f) + 0.5f;\n";
        ps_code += "    __shadow = tex2D(__shadowMap, __scoord);\n";
    }

    // texturing
    if (diffuse)
    {

        if (!diffuse->IsGenerated())
            ps_code += diffuse->Generte();

        ps_code += "    __diffuse = " + diffuse->GetVarName() + ";\n";
    }

    if (specular)
    {
        if (!specular->IsGenerated())
            ps_code += specular->Generte();

        ps_code += "    __specular = " + specular->GetVarName() + ";\n";
    }

    // lighting 
    if (lm == LM_PHONE)
    {
        if (normal)
        {
            if (!normal->IsGenerated())
                ps_code += normal->Generte();

            ps_code += "    __normal = " + normal->GetVarName() + ".rgb * 2 - 1;\n";
        }

        //diffuse lighting
        ps_code += "    __NdL = max(0, dot(__normal, __lightdir)); \n";

        //diffuse power
        if (diffuse_power)
        {
            if (!diffuse_power->IsGenerated())
                ps_code += diffuse_power->Generte();

            ps_code += "    __NdL = pow(__NdL, " + diffuse_power->GetVarName() + ");\n";
        }

        //specular lighting
        if (specular)
        {
            ps_code += "    __reflect = reflect(-lightdir, __normal);\n";
            ps_code += "    __RdV = max(0, dot(__reflect, __view));\n";

            if (specular_power)
            {
                if (!specular_power->IsGenerated())
                    ps_code += specular_power->Generte();

                ps_code += "    __RdV = pow(__RdV, " + specular_power->GetVarName() + ");\n";
            }
        }

        ps_code += "    __NdL *= atten;\n";
        ps_code += "    __RdV *= atten;\n";
    }

    //blend
    {
        ps_code += "    __final += __diffuse * (__shadow * __NdL * light_diffuse + scene_ambient);\n";
        ps_code += "    __final += __shadow * __specular * __RdV * light_specular;\n";
    }

    //fogging
    if (fm != FOG_NONE)
    {
        ps_code += "    __final = __final + fog * (fog_color - __final);\n";
    }

    // transparency
    if (transparency)
    {
        if (!transparency->IsGenerated())
            transparency->Generte();

        ps_code += "    __final.a = " + transparency->GetVarName() + ".a;\n";
    }

    ps_code += pixel_shader_end;

#if 0
    LOG_PRINT(ps_code);
#endif

    List<ShaderParamDefine> params;
    GenPixelShaderParam(inst, params);
    ShaderProgramPtr ps = ShaderProgramManager::Instance().CreateShaderFromString(inst->GetName() + "_PS",
                                                                                  SHADER_LANGUAGE_CG,
                                                                                  SHADER_PROFILE_PS_3_0,
                                                                                  ps_code, "main", &params);

    inst->_SetPixelShader(ps);
}

void ST_Code_Generter_Cg::GenPixelShaderParam(ST_Instance * inst, List<ShaderParamDefine> & params)
{
    FOG_MODE fm = inst->GetFogMode();
    LIGHTING_MODE lm = inst->GetLightingMode();
    ST_Node * diffuse = inst->GetDiffuse();
    ST_Node * diffuse_power = inst->GetDiffusePower();
    ST_Node * specular = inst->GetSpecular();
    ST_Node * specular_power = inst->GetSpecularPower();
    ST_Node * normal = inst->GetNormal();

    ShaderParamDefine param;

    //scene ambient
    param.name = "scene_ambient";
    param.autotype = ASPT_SCENE_AMBIENT;
    param.autoindex = 0;
    param.count = 1;
    param.type = SPT_FLOAT4;
    params.PushBack(param);

    //diffuse lighting
    if (lm == LM_PHONE && diffuse)
    {
        param.name = "light_diffuse";
        param.autotype = ASPT_LIGHT_DIFFUSE;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);
    }

    //specular lighting
    if (lm == LM_PHONE && specular)
    {
        param.name = "light_specular";
        param.autotype = ASPT_LIGHT_SPECULAR;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);
    }

    //foggine
    if (fm != FOG_NONE)
    {
        param.name = "fog_color";
        param.autotype = ASPT_FOG_COLOR;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);
    }

}

void ST_Code_Generter_Cg::GenVertexShader(ST_Instance * inst)
{
    String vs_code;

    FOG_MODE fm = inst->GetFogMode();
    LIGHTING_MODE lm = inst->GetLightingMode();
    bool skeleton = inst->GetSkeletonAnimation();
    ST_Node * diffuse = inst->GetDiffuse();
    ST_Node * diffuse_power = inst->GetDiffusePower();
    ST_Node * specular = inst->GetSpecular();
    ST_Node * specular_power = inst->GetSpecularPower();
    ST_Node * normal = inst->GetNormal();

    if (lm == LM_PHONE)
        vs_code += lighting;

    if (fm != FOG_NONE)
        vs_code += fogging;

    vs_code += vertex_shader_head;

    // transform
    if (!skeleton)
    {
        vs_code += transform_no_skel;
    }
    else
    {
        vs_code += transform_has_skel;
        vs_code += transform_no_skel;
    }


    //lighting
    if (lm == LM_PHONE)
    {
        vs_code += "    vec3 lightdir, view;                             \n"\
                   "    float atten;                                     \n"\
                   "                                                     \n"\
                   "    CalcuLighting(_position, camera_position,        \n"\
                   "                  light_position, light_direction,   \n"\
                   "                  light_attenuation, light_spot,     \n"\
                   "                  lightdir, view, atten);            \n"\
                   "                                                     \n\n";

        if (normal)
            vs_code += "    oposition = mul(position, mat_worldviewproj);\n"\
                       "    ocolor = color;                              \n"\
                       "    otexcoord = texcoord;                        \n"\
                       "    onormal = _normal;                           \n"\
                       "    olightdir = mul(_tbn, lightdir);             \n"\
                       "    oview = mul(_tbn, view);                     \n"\
                       "    oatten = atten;                              \n\n";
        else
            vs_code += "    oposition = mul(position, mat_worldviewproj);\n"\
                       "    ocolor = color;                              \n"\
                       "    otexcoord = texcoord;                        \n"\
                       "    olightdir = lightdir;                        \n"\
                       "    onormal = _normal;                           \n"\
                       "    oview = view;                                \n"\
                       "    oatten = atten;                              \n\n";

    }
    else
    {
        vs_code += "    oposition = mul(position, mat_worldviewproj);   \n"\
                   "    ocolor = color;                                 \n"\
                   "    otexcoord = texcoord;                           \n\n";
    }

    if (fm != FOG_NONE)
    {
        vs_code += "    ofog = fogging(_position.xyz, camera_position.xyz,     \n"\
                   "                   fog_param.x, fog_param.y,               \n"\
                   "                   fog_param.z, fog_param.w);              \n\n";
    }

    vs_code += "    opos = oposition; \n";


    vs_code += vertex_shader_end;

#if 0
    LOG_PRINT(vs_code);
#endif

    List<ShaderParamDefine> params;
    GenVertexShaderParam(inst, params);
    ShaderProgramPtr vs = ShaderProgramManager::Instance().CreateShaderFromString(inst->GetName() + "_VS",
                                                                               SHADER_LANGUAGE_CG,
                                                                               SHADER_PROFILE_VS_3_0,
                                                                               vs_code, "main", &params);

    inst->_SetVertexShader(vs);
}

void ST_Code_Generter_Cg::GenVertexShaderParam(ST_Instance * inst, List<ShaderParamDefine> & params)
{
    FOG_MODE fm = inst->GetFogMode();
    LIGHTING_MODE lm = inst->GetLightingMode();
    bool skeleton = inst->GetSkeletonAnimation();
    ST_Node * diffuse = inst->GetDiffuse();
    ST_Node * diffuse_power = inst->GetDiffusePower();
    ST_Node * specular = inst->GetSpecular();
    ST_Node * specular_power = inst->GetSpecularPower();
    ST_Node * normal = inst->GetNormal();
    ShaderParamDefine param;

    // world view proj matirx
    param.name = "mat_worldviewproj";
    param.autotype = ASPT_WORLD_VIEW_PROJ_MATRIX;
    param.autoindex = 0;
    param.count = 1;
    param.type = SPT_MATRIX4x4;
    params.PushBack(param);

    //...
    if (lm == LM_PHONE || fm != FOG_NONE)
    {
        // world matrix
        param.name = "mat_world";
        param.autotype = ASPT_WORLD_MATRIX;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_MATRIX4x4;
        params.PushBack(param);

        // camera position
        param.name = "camera_position";
        param.autotype = ASPT_CAMERA_POSITION;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);
    }

    // lighting
    if (lm == LM_PHONE)
    {
        // light position;
        param.name = "light_position";
        param.autotype = ASPT_LIGHT_POSITION;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);

        //light direction
        param.name = "light_direction";
        param.autotype = ASPT_LIGHT_DIRECTION;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);

        //light attenuation
        param.name = "light_attenuation";
        param.autotype = ASPT_LIGHT_ATTENUATION;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushFront(param);

        //light spot
        param.name = "light_spot";
        param.autotype = ASPT_LIGHT_SPOT_PARAMS;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushFront(param);
    }

    if (skeleton)
    {
        param.name = "mat_bone";
        param.autotype = ASPT_BLEND_MATRIX;
        param.autoindex = 0;
        param.count = MAX_BLEND_MATRIX_VS;
        param.type = SPT_MATRIX4x4;
        params.PushBack(param);
    }

    if (fm != FOG_NONE)
    {
        param.name = "fog_param";
        param.autotype = ASPT_FOG_PARAM;
        param.autoindex = 0;
        param.count = 1;
        param.type = SPT_FLOAT4;
        params.PushBack(param);
    }
}


String ST_Code_Generter_Cg::GenerteCode(ST_Node * node)
{
    switch (node->GetType())
    {
    case STNODE_SAMPLER2D:
        return GenSampler2DCode(node);

    case STNODE_MULTIPLY:
        return GenMultiplyCode(node);

    case STNODE_ADD:
        return GenAddCode(node);
        
    case STNODE_SUB:
        return GenSubCode(node);

    case STNODE_CLAMP:
        return GenClampCode(node);

    case STNODE_CONST_FLOAT1:
        return GenConstFloat1Code(node);

    case STNODE_CONST_FLOAT2:
        return GenConstFloat2Code(node);

    case STNODE_CONST_FLOAT3:
        return GenConstFloat3Code(node);

    case STNODE_CONST_FLOAT4:
        return GenConstFloat4Code(node);

    case STNODE_PARAM_FLOAT4:
        return GenParamFloat4Code(node);

    default:
        assert (0);
        return "";
    }
}

String ST_Code_Generter_Cg::GenSampler2DCode(ST_Node * node)
{
    ST_Node_Sampler2D * sampler = (ST_Node_Sampler2D*)node;
    ST_Node * input = sampler->GetInput();

    String code;

    String texcoord;

    if (input)
    {
        code += input->Generte();
        texcoord = input->GetVarName();
    }
    else
    {
        texcoord = "texcoord";
    }
    
    code = "    float4 " +  sampler->GetVarName() + " = tex2D(" + sampler->GetName() + ", " + texcoord + ");\n";

    return code;
}

String ST_Code_Generter_Cg::GenAddCode(ST_Node * node)
{
    ST_Node_Add * add = (ST_Node_Add *)node;
    ST_Node * input0 = add->GetInput0();
    ST_Node * input1 = add->GetInput1();
    ST_VALUE_TYPE type = add->GetValueType();

    if (type == STVAL_UNKNOWN)
    {
        EXCEPTION("Shader Tree Error: Add Node '" + add->GetName() + "' input invalid.");
    }

    String num;
    if (type > STVAL_FLOAT1)
    {
        num = type - STVAL_FLOAT1 + 1;
    }

    const String & var = add->GetVarName();
    const String & var0 = input0->GetVarName();
    const String & var1 = input1->GetVarName();

    return "    float" + num + " " + var + " = " + var0 + " + " + var1 + ";\n";
}

String ST_Code_Generter_Cg::GenSubCode(ST_Node * node)
{
    ST_Node_Sub * sub = (ST_Node_Sub *)node;
    ST_Node * input0 = sub->GetInput0();
    ST_Node * input1 = sub->GetInput1();
    ST_VALUE_TYPE type = sub->GetValueType();

    if (type == STVAL_UNKNOWN)
    {
        EXCEPTION("Shader Tree Error: Sub Node '" + sub->GetName() + "' input invalid.");
    }

    String num;
    if (type > STVAL_FLOAT1)
    {
        num = type - STVAL_FLOAT1 + 1;
    }

    const String & var = sub->GetVarName();
    const String & var0 = input0->GetVarName();
    const String & var1 = input1->GetVarName();

    return "    float" + num + " " + var + " = " + var0 + " - " + var1 + ";\n";
}

String ST_Code_Generter_Cg::GenMultiplyCode(ST_Node * node)
{
    ST_Node_Multiply * mul = (ST_Node_Multiply *)node;
    ST_Node * input0 = mul->GetInput0();
    ST_Node * input1 = mul->GetInput1();
    ST_VALUE_TYPE type = mul->GetValueType();

    if (type == STVAL_UNKNOWN)
    {
        EXCEPTION("Shader Tree Error: Multiply Node '" + mul->GetName() + "' input invalid.");
    }

    String num;
    if (type > STVAL_FLOAT1)
    {
        num = type - STVAL_FLOAT1 + 1;
    }

    const String & var = mul->GetVarName();
    const String & var0 = input0->GetVarName();
    const String & var1 = input1->GetVarName();

    return "    float" + num + " " + var + " = " + var0 + " * " + var1 + ";\n";
}

String ST_Code_Generter_Cg::GenClampCode(ST_Node * node)
{
    ST_Node_Clamp * clamp = (ST_Node_Clamp *)node;
    ST_Node * input0 = clamp->GetInput0();
    ST_Node * input1 = clamp->GetInput1();
    ST_Node * input2 = clamp->GetInput2();
    ST_VALUE_TYPE type = clamp->GetValueType();

    if (type == STVAL_UNKNOWN)
    {
        EXCEPTION("Shader Tree Error: Add Node '" + clamp->GetName() + "' input invalid.");
    }

    String num;
    if (type > STVAL_FLOAT1)
    {
        num = type - STVAL_FLOAT1 + 1;
    }

    const String & var = clamp->GetVarName();
    const String & var0 = input0->GetVarName();
    const String & var1 = input1->GetVarName();
    const String & var2 = input2->GetVarName();

    return "    float" + num + " " + var + " = clamp(" + var0 + ", " + var1 + ", " + var2 + ");\n";
}

String ST_Code_Generter_Cg::GenConstFloat1Code(ST_Node * node)
{
    ST_Node_ConstFloat1 * _const = (ST_Node_ConstFloat1 *)node;
    return "    float " + _const->GetVarName() + " = " + _const->GetValue() + ";\n";
}

String ST_Code_Generter_Cg::GenConstFloat2Code(ST_Node * node)
{
    ST_Node_ConstFloat2 * _const = (ST_Node_ConstFloat2 *)node;
    const Vec2 & v = _const->GetValue();

    return "    float2 " + _const->GetVarName() + " = float2(" + v.x + ", " + v.y + ");\n";
}

String ST_Code_Generter_Cg::GenConstFloat3Code(ST_Node * node)
{
    ST_Node_ConstFloat3 * _const = (ST_Node_ConstFloat3 *)node;
    const Vec3 & v = _const->GetValue();

    return "    float3 " + _const->GetVarName() + " = float3(" + v.x + ", " + v.y + ", " + v.z + ");\n";
}

String ST_Code_Generter_Cg::GenConstFloat4Code(ST_Node * node)
{
    ST_Node_ConstFloat4 * _const = (ST_Node_ConstFloat4 *)node;
    const Vec4 & v = _const->GetValue();

    return "    float4 " + _const->GetVarName() + " = float4(" + v.x + ", " + v.y + ", " + v.z + ", " + v.w +");\n";
}

String ST_Code_Generter_Cg::GenParamFloat4Code(ST_Node * node)
{
    return "";
}





String ST_Code_Generter_Cg::GenerteUniformParamDefine(ST_Node * node)
{
    switch (node->GetType())
    {
    case STNODE_SAMPLER2D:
        return GenSampler2DUniform(node);

    case STNODE_PARAM_FLOAT4:
        return GenParamFloat4Uniform(node);

    default:
        assert(0);
        return "";
    }
}

String ST_Code_Generter_Cg::GenSampler2DUniform(ST_Node * node)
{
    return "uniform sampler2D " + node->GetName() + ";\n";
}

String ST_Code_Generter_Cg::GenParamFloat4Uniform(ST_Node * node)
{
    return "uniform float4 " + node->GetName() + ";\n";
}