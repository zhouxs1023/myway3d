/*
--------------------------------------------------------------------------
    Default Material Loader
    
      keyword               mean                    default                 format
     
     technique          define a technique            null              technique name
     scheme             scheme of the technique       null              scheme name
     pass               pass of the technique         null              pass name
     group              render group of the pass      50                group id (0 - 255)
     lighting           lighting                      none              lighting mode (all, directional, point, spot, none)
     ambient            ambient                       white             ambient r g b a
     emissive           emissive                      black             emissive r g b a
     diffuse            diffuse                       white             diffuse r g b a
     specular           specular                      white             specular r g b a
     power              specular power                1.0f              power val
     cull_mode          cull mode                     ccw               cull_mode mode (ccw, cw, none)
     shade_mode         shade mode                    gouraud           shade_mode mode (flat, gouraud)
     fill_mode          fill mode                     solid             fill_mode mode (solid, frame, point)
     color_write        color write                   argb              color_write color
     depth_write        depth write enable            true              depth_write bool (true, false)
     depth_check        depth check                   always            depth_check func (nerver, less, equal, less_equal, 
                                                                                          greater, not_equal, greater_equal, always)
     depth_bias         depth bias                    0 0               depth_bias const slope
     alpha_blend        alpha blend                   one zero add      alpha_blend src dest op
                                                                             src dest(zero, one, src_color, inv_src_color, 
                                                                                      src_alpha, inv_src_alpha, dest_alpha,
                                                                                      inv_dest_alpha, dest_color, inv_dest_color,
                                                                                      blend_srcalphasat)
                                                                              op (add, substract,rev_substract, min, max)

    alpha_test          alpha test                    always 0          alpha_test func ref
    vertex_shader       vertex shader                                   vertex_shader name
    pixel_shader        pixel shader                                    pixel_shader name
    point_sprite        point sprite                  false             point_sprite bool
    point_size          point size                    1 64              point_size min max
    point_scale         point scale                   1 0 0             point_scale a b c
    fog                 fog                           0 1 1 white       fog start end density color

    texture_stage       texture stage                 null              texture_stage name
    cube_texture        cube texture                                    cube_texture name mipmaps
    volume_texture      volume texture                                  volume_texture name mipmaps
    texture             texture                                         texture name mipmaps
    filter_type         filter type                                     filter_type mag min mip (none point linear anisotropy)
    address_mode        address mode                                    address_mode u v w (wrap, clamp , mirror, 
                                                                                            border, mirror_once)
    border_color        border_color                                    border_color color
    maxmip_level        maxmip_level                 0                  maxmip_level level
    max_anisotropy      max_anisotropy               1                  max_anisotropy max     
    linker              number                       -1                 linker number.
    

    ST_Instance         ST_Instance                   ""                ST_Instance name
    ST_ParamFloat4      ST_ParamFloat4                xx                ST_ParamFloat4 name

--------------------------------------------------------------------------
*/

#include "MWMaterialLoader.h"
#include "MWRenderSystem.h"
#include "MWResourceManager.h"
#include "MWMaterialManager.h"

using namespace Myway;

//#define st_instance                     "ST_Instance"
//#define st_paramloat4                 "ST_ParamFloat4"
//
//
//#define technique                       "technique"
//#define scheme                          "scheme"
//#define force_solid                     "force_solid"
//#define group                           "group"
//#define pass                            "pass"
//#define null                            ""
//
//#define lighting                        "lighting"
//#define spot                            "spot"
//#define point                           "point"
//#define directional                     "directional"
//#define all                             "all"
//#define none                            "none"
//#define ambient                         "ambient"
//#define emissive                        "emissive"
//#define diffuse                         "diffuse"
//#define specular                        "specular"
//#define power                           "power"
//#define cull_mode                       "cull_mode"
//#define shade_mode                      "shade_mode"
//#define fill_mode                       "fill_mode"
//#define color_write                     "color_write"
//#define depth_write                     "depth_write"
//#define depth_check                     "depth_check"
//#define depth_bias                      "depth_bias"
//#define alpha_blend                     "alpha_blend"
//#define alpha_test                      "alpha_test"
//#define vertex_shader                   "vertex_shader"
//#define pixel_shader                    "pixel_shader"
//#define point_sprite                    "point_sprite"
//#define point_size                      "point_size"
//#define point_scale                     "point_scale"
//#define fog                             "fog"
//
//
//#define texture_stage                   "texture_stage"
//#define cube_texture                    "cube_texture"
//#define volume_texture                  "volume_texture"
//#define texture                         "texture"
//#define filter_type                     "filter_type"
//#define address_mode                    "address_mode"
//#define border_color                    "border_color"
//#define maxmip_level                    "maxmip_level"
//#define max_anisotropy                  "max_anisotropy"
//#define linker                          "linker"
//
//#define shader_param                    "param"
//#define paramnt1                      "int1"
//#define paramnt2                      "int2"
//#define paramnt3                      "int3"
//#define paramnt4                      "int3"
//#define paramloat1                    "float1"
//#define paramloat2                    "float2"
//#define paramloat3                    "float3"
//#define paramloat4                    "float4"
//#define paramMat3x3                    "mat3x3"
//#define paramMat4x4                    "mat4x4"
//#define paramMat_w                     "mat_world"
//#define paramMat_v                     "mat_view"
//#define paramMat_p                     "mat_proj"
//#define paramMat_wv                    "mat_worldview"
//#define paramMat_vp                    "mat_viewproj"
//#define paramMat_wvp                   "mat_worldviewproj"
//#define paramMat_shadow_v              "mat_shadow_view"
//#define paramMat_shadow_p              "mat_shadow_proj"
//#define paramMat_shadow_vp             "mat_shadow_viewproj"
//#define paramMat_shadow_wv             "mat_shadow_worldview"
//#define paramMat_shadow_wvp            "mat_shadow_worldviewproj"
//#define paramMat_blend                 "mat_blend"
//#define paramMat_proj                  "mat_projection"
//#define paramMtrl_ambient              "mtl_ambient"
//#define paramMtrl_emissive             "mtl_emissive"
//#define paramMtrl_diffuse              "mtl_diffuse"
//#define paramMtrl_specular             "mtl_specular"
//#define paramMtrl_power                "mtl_power"
//#define param_light_ambient             "light_ambient"
//#define param_light_diffuse             "light_diffuse"
//#define param_light_specular            "light_specular"
//#define param_light_position            "light_position"
//#define param_light_direction           "light_direction"
//#define param_light_attenuation         "light_attenuation"
//#define param_light_spot_params         "light_spot_params"
//#define param_camera_position           "camera_position"
//#define param_camera_direction          "camera_direction"
//#define param_camera_right              "camera_right"
//#define param_camera_up                 "camera_up"
//
//#define paramexture_size              "texture_size"
//#define paramnv_texture_size          "inv_texture_size"
//
//#define paramog_param                 "fog_param"
//#define paramog_color                 "fog_color"
//
//#define paramime                      "time"
//#define param_sin_time                  "sin_time"
//#define param_cos_time                  "cos_time"
//#define paraman_time                  "tan_time"
//#define paramrame_time                "frame_time"
//#define paramime_2pi                  "time_2pi"
//#define paramps                       "fps"
//#define param_user                      "user"
//
//#define group_a                         "group                "
//#define ambient_a                       "ambient              "
//#define emissive_a                      "emissive             "
//#define diffuse_a                       "diffuse              "
//#define specular_a                      "specular             "
//#define power_a                         "power                "
//#define cull_mode_a                     "cull_mode            "
//#define shade_mode_a                    "shade_mode           "
//#define fill_mode_a                     "fill_mode            "
//#define color_write_a                   "color_write          "
//#define depth_write_a                   "depth_write          "
//#define depth_check_a                   "depth_check          "
//#define depth_func_a                    "depth_func           "
//#define depth_bais_a                    "depth_bais           "
//#define alpha_blend_a                   "alpha_blend          "
//#define src_blend_a                     "src_blend            "
//#define dest_blend_a                    "dest_blend           "
//#define blend_op_a                      "blend_op             "
//#define alpha_test_a                    "alpha_test           "
//#define alpha_test_func_a               "alpha_test_func      "
//#define alpha_test_ref_a                "alpha_test_ref       "
//#define vertex_shader_a                 "vertex_shader        "
//#define pixel_shader_a                  "pixel_shader         "
//#define point_min_a                     "point_min            "
//#define point_max_a                     "point_max            "
//#define point_scale_a_a                 "point_scale_a        "
//#define point_scale_b_a                 "point_scale_b        "
//#define point_scale_c_a                 "point_scale_c        "
//#define point_sprite_a                  "point_sprite         "
//#define fog_start_a                     "fog_start            "
//#define fog_end_a                       "fog_end              "
//#define fog_density_a                   "fog_density          "
//#define fog_color_a                     "fog_color            "
//
//#define texture_a                       "texture              "
//#define mag_filter_a                    "mag_filter           "
//#define min_filter_a                    "min_filter           "
//#define mip_filter_a                    "mip_filter           "
//#define address_u_a                     "address_u            "
//#define address_v_a                     "address_v            "
//#define address_w_a                     "address_w            "
//#define border_color_a                  "border_color         "
//#define mipmap_level_a                  "mipmap_level         "
//#define max_anisotropy_a                "max_anisotropy       "
//
//#define cull_none                       "none"
//#define cull_ccw                        "ccw"
//#define cull_cw                         "cw"
//
//#define shade_flat                      "flat"
//#define shade_gouraud                   "gouraud"
//
//#define fill_point                      "point"
//#define fill_solid                      "solid"
//#define fill_frame                      "frame"
//
//#define color_none                      "none"
//
//#define on                              "true"
//#define off                             "false"
//#define tab                             "    "
//#define space                           " "
//#define bracket_b                       "{"
//#define bracket_e                       "}"
//
//
//#define cmp_nerver                      "nerver"
//#define cmp_less                        "less"
//#define cmp_equal                       "equal"
//#define cmp_lessequal                   "less_equal"
//#define cmp_greater                     "greater"
//#define cmp_notequal                    "not_equal"
//#define cmp_greaterequal                "greater_equal"
//#define cmp_always                      "always"
//
//#define blend_zero                      "zero"
//#define blend_one                       "one"
//#define blend_srccolor                  "src_color"
//#define blend_invsrccolor               "inv_src_color"
//#define blend_srcalpha                  "src_alpha"
//#define blend_invsrcalpha               "inv_src_alpha"
//#define blend_destalpha                 "dest_alpha"
//#define blend_invdestalpha              "inv_dest_alpha"
//#define blend_destcolor                 "dest_color"
//#define blend_invdestcolor              "inv_dest_color"
//#define blend_srcalphasat               "src_alpha_sat"
//
//
//#define blendop_add                     "add"
//#define blendop_subtract                "subtract"
//#define blendop_revsubtract             "rev_subtract"
//#define blendop_min                     "min"
//#define blendop_max                     "max"
//
//#define filter_none                     "none"
//#define filter_point                    "point"
//#define filter_linear                   "linear"
//#define filter_anisotropy               "anisotropy"
//
//#define address_wrap                    "wrap"
//#define address_clamp                   "clamp"
//#define address_mirror                  "mirror"
//#define address_border                  "border"
//#define address_mirroronce              "mirror_once"
//
//String GetCullModeString(CULL_MODE m)
//{
//    switch (m)
//    {
//    case CULL_CCW:
//        return cull_ccw;
//
//    case CULL_CW:
//        return cull_cw;
//
//    case CULL_NONE:
//        return cull_none;
//
//    default:
//        return cull_none;
//    }
//}
//
//String GetFillModeString(FILL_MODE m)
//{
//    switch (m)
//    {
//    case FILL_SOLID:
//        return fill_solid;
//
//    case FILL_FRAME:
//        return fill_frame;
//
//    case FILL_POINT:
//        return fill_point;
//
//    default:
//        return fill_solid;
//    }
//}
//
//String GetShadeModeString(SHADE_MODE m)
//{
//    switch (m)
//    {
//    case SHADE_GOURAUD:
//        return shade_gouraud;
//
//    case SHADE_FLAT:
//        return shade_flat;
//
//    default:
//        return shade_gouraud;
//    }
//}
//
//String GetDeptchWriteString(bool bOn)
//{
//    if (bOn)
//        return on;
//    else
//        return off;
//}
//
//String GetDepthCheckString(bool bOn)
//{
//    if (bOn)
//        return on;
//    else
//        return off;
//}
//
//String GetAlphaBlendString(bool bOn)
//{
//    if (bOn)
//        return on;
//    else
//        return off;
//}
//
//String GetAlphaTestString(bool bOn)
//{
//    if (bOn)
//        return on;
//    else
//        return off;
//}
//
//String GetPointSpriteEnableString(bool bOn)
//{
//    if (bOn)
//        return on;
//    else
//        return off;
//}
//
//String GetCompareFuncString(COMPARE_FUNCTION f)
//{
//    switch (f)
//    {
//    case CMP_NEVER:
//        return cmp_nerver;
//
//    case CMP_LESS:
//        return cmp_less;
//
//    case CMP_EQUAL:
//        return cmp_equal;
//
//    case CMP_LESSEQUAL:
//        return cmp_lessequal;
//
//    case CMP_GREATER:
//        return cmp_greater;
//
//    case CMP_NOTEQUAL:
//        return cmp_notequal;
//
//    case CMP_GREATEREQUAL:
//        return cmp_greaterequal;
//
//    case CMP_ALWAYS:
//        return cmp_always;
//
//    default:
//        return cmp_always;
//    }
//}
//
//String GetBlendFactoryString(BLEND_FACTORY f)
//{
//    switch (f)
//    {
//    case BLEND_ZERO:
//        return blend_zero;
//
//    case BLEND_ONE:
//        return blend_one;
//
//    case BLEND_SRCCOLOR:
//        return blend_srccolor;
//
//    case BLEND_INVSRCCOLOR:
//        return blend_invsrccolor;
//
//    case BLEND_SRCALPHA:
//        return blend_srcalpha;
//
//    case BLEND_INVSRCALPHA:
//        return blend_invsrcalpha;
//
//    case BLEND_DESTALPHA:
//        return blend_destalpha;
//
//    case BLEND_INVDESTALPHA:
//        return blend_invdestalpha;
//
//    case BLEND_DESTCOLOR:
//        return blend_destcolor;
//
//    case BLEND_INVDESTCOLOR:
//        return blend_invdestcolor;
//
//    case BLEND_SRCALPHASAT:
//        return blend_srcalphasat;
//
//    default:
//        return blend_one;
//    }
//}
//
//String GetBlendOperatorString(BLEND_OPERATOR op)
//{
//    switch (op)
//    {
//    case BLENDOP_ADD:
//        return blendop_add;
//
//    case BLENDOP_SUBTRACT:
//        return blendop_subtract;
//
//    case BLENDOP_REVSUBTRACT:
//        return blendop_revsubtract;
//
//    case BLENDOP_MIN:
//        return blendop_min;
//
//    case BLENDOP_MAX:
//        return blendop_max;
//
//    default:
//        return blendop_add;
//    }
//}
//
//CULL_MODE GetCullMode(const String & m)
//{
//    if (m == cull_ccw)
//        return CULL_CCW;
//    else if (m == cull_cw)
//        return CULL_CW;
//    else if (m == cull_none)
//        return CULL_NONE;
//    else
//    {
//        LOG_PRINT("unknown command: " + m + "\n");
//        return CULL_CCW;
//    }
//}
//
//SHADE_MODE GetShadeMode(const String & m)
//{
//    if (m == shade_gouraud)
//        return SHADE_GOURAUD;
//    else if (m == shade_flat)
//        return SHADE_FLAT;
//    else
//    {
//        LOG_PRINT("unknown command: " + m + "\n");
//        return SHADE_GOURAUD;
//    }
//}
//
//FILL_MODE GetFillMode(const String & m)
//{
//    if (m == fill_solid)
//        return FILL_SOLID;
//    else if (m == fill_frame)
//        return FILL_FRAME;
//    else if (m == fill_point)
//        return FILL_POINT;
//    else
//    {
//        LOG_PRINT("unknown command: " + m + "\n");
//        return FILL_SOLID;
//    }
//}
//
//bool GetDeptchWrite(const String & s)
//{
//    if (s == on)
//        return TRUE;
//    else if (s == off)
//        return FALSE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return FALSE;
//    }
//}
//
//bool GetDeptchCheck(const String & s)
//{
//    if (s == on)
//        return TRUE;
//    else if (s == off)
//        return FALSE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return FALSE;
//    }
//}
//
//bool GetAlphaBlendEnable(const String & s)
//{
//    if (s == on)
//        return TRUE;
//    else if (s == off)
//        return FALSE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return FALSE;
//    }
//}
//
//bool GetAlphaTest(const String & s)
//{
//    if (s == on)
//        return TRUE;
//    else if (s == off)
//        return FALSE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return FALSE;
//    }
//}
//
//bool GetPointSpriteEnable(const String & s)
//{
//    if (s == on)
//        return TRUE;
//    else if (s == off)
//        return FALSE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return FALSE;
//    }
//}
//
//COMPARE_FUNCTION GetCompareFunc(const String & s)
//{
//    if (s == cmp_nerver)
//        return CMP_NEVER;
//    else if (s == cmp_less)
//        return CMP_LESS;
//    else if (s == cmp_equal)
//        return CMP_EQUAL;
//    else if (s == cmp_lessequal)
//        return CMP_LESSEQUAL;
//    else if (s == cmp_greater)
//        return CMP_GREATER;
//    else if (s == cmp_notequal)
//        return CMP_NOTEQUAL;
//    else if (s == cmp_greaterequal)
//        return CMP_GREATEREQUAL;
//    else if (s == cmp_always)
//        return CMP_ALWAYS;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return CMP_ALWAYS;
//    }
//}
//
//BLEND_FACTORY GetBlendFactory(const String & s)
//{
//    if (s == blend_zero)
//        return BLEND_ZERO;
//    else if (s == blend_one)
//        return BLEND_ONE;
//    else if (s == blend_srccolor)
//        return BLEND_SRCCOLOR;
//    else if (s == blend_invsrccolor)
//        return BLEND_INVSRCCOLOR;
//    else if (s == blend_srcalpha)
//        return BLEND_SRCALPHA;
//    else if (s == blend_invsrcalpha)
//        return BLEND_INVSRCALPHA;
//    else if (s == blend_destalpha)
//        return BLEND_DESTALPHA;
//    else if (s == blend_invdestalpha)
//        return BLEND_INVDESTALPHA;
//    else if (s == blend_destcolor)
//        return BLEND_DESTCOLOR;
//    else if (s == blend_invdestcolor)
//        return BLEND_INVDESTCOLOR;
//    else if (s == blend_srcalphasat)
//        return BLEND_SRCALPHASAT;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return BLEND_ZERO;
//    }
//}
//
//BLEND_OPERATOR GetBlendOperator(const String & s)
//{
//    if (s == blendop_add)
//        return BLENDOP_ADD;
//    else if (s == blendop_subtract)
//        return BLENDOP_SUBTRACT;
//    else if (s == blendop_revsubtract)
//        return BLENDOP_REVSUBTRACT;
//    else if (s == blendop_min)
//        return BLENDOP_MIN;
//    else if (s == blendop_max)
//        return BLENDOP_MAX;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return BLENDOP_ADD;
//    }
//}
//
//String GetFilterTypeString(TEXTURE_FILTER_TYPE t)
//{
//    switch (t)
//    {
//    case TEXF_NONE:
//        return filter_none;
//
//    case TEXF_POINT:
//        return filter_point;
//
//    case TEXF_LINEAR:
//        return filter_linear;
//
//    case TEXF_ANISOTROPIC:
//        return filter_anisotropy;
//
//    default:
//        return filter_none;
//    }
//}
//
//String GetAddressModeString(TEXTURE_ADDRESS_MODE m)
//{
//    switch (m)
//    {
//    case TEXADDRESS_WRAP:
//        return address_wrap;
//
//    case TEXADDRESS_BORDER:
//        return address_border;
//
//    case TEXADDRESS_CLAMP:
//        return address_clamp;
//
//    case TEXADDRESS_MIRROR:
//        return address_mirror;
//
//    case TEXADDRESS_MIRRORONCE:
//        return address_mirroronce;
//
//    default:
//        return address_wrap;
//    }
//}
//
//TEXTURE_FILTER_TYPE GetFilterType(const String & s)
//{
//    if (s == filter_none)
//        return TEXF_NONE;
//    else if (s == filter_point)
//        return TEXF_POINT;
//    else if (s == filter_linear)
//        return TEXF_LINEAR;
//    else if (s == filter_anisotropy)
//        return TEXF_ANISOTROPIC;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return TEXF_NONE;
//    }
//}
//
//TEXTURE_ADDRESS_MODE GetAddressMode(const String & s)
//{
//    if (s == address_wrap)
//        return TEXADDRESS_WRAP;
//    else if (s == address_clamp)
//        return TEXADDRESS_CLAMP;
//    else if (s == address_mirror)
//        return TEXADDRESS_MIRROR;
//    else if (s == address_border)
//        return TEXADDRESS_BORDER;
//    else if (s == address_mirroronce)
//        return TEXADDRESS_MIRRORONCE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return TEXADDRESS_WRAP;
//    }
//}
//
//String GetColorWriteString(int iColorWrite)
//{
//    String sColorWrite("");
//
//    if (iColorWrite & COLORWRITEENABLE_ALPHA)
//        sColorWrite << "a";
//    if (iColorWrite & COLORWRITEENABLE_RED)
//        sColorWrite << "r";
//    if (iColorWrite & COLORWRITEENABLE_GREEN)
//        sColorWrite << "g";
//    if (iColorWrite & COLORWRITEENABLE_BLUE)
//        sColorWrite << "b";
//
//    return (sColorWrite != "") ? sColorWrite : color_none;
//}
//
//int GetColorWrite(const String & s)
//{
//    int i = 0;
//
//    if (s.Find("a"))
//        i |= COLORWRITEENABLE_ALPHA;
//    if (s.Find("r"))
//        i |= COLORWRITEENABLE_RED;
//    if (s.Find("g"))
//        i |= COLORWRITEENABLE_GREEN;
//    if (s.Find("b"))
//        i |= COLORWRITEENABLE_BLUE;
//
//    return i;
//}
//
//RenderLightingType GetLightingType(const String & s)
//{
//    if (s == directional)
//        return RLT_DIRECTIONAL;
//    else if (s == point)
//        return RLT_POINT;
//    else if (s == spot)
//        return RLT_SPOT;
//    else if (s == all)
//        return RLT_ALL;
//    else if (s == none)
//        return RLT_NONE;
//    else
//    {
//        LOG_PRINT("unknown command: " + s + "\n");
//        return RLT_NONE;
//    }
//}
//
//
//
//
//DefaultMaterialLoader::DefaultMaterialLoader()
//{
//}
//
//DefaultMaterialLoader::~DefaultMaterialLoader()
//{
//}
//
//MaterialPtr DefaultMaterialLoader::Load(const String & sName, const String & filename)
//{
//    profile_code();
//
//    DataStreamPtr stream = ResourceManager::Instance().OpenResource(filename);
//
//    if (stream.IsNull())
//    {
//        LOG_PRINT("FileSystem Error: " + filename + " don't find in any file systems.\n");
//        return MaterialPtr();
//    }
//
//    MaterialPtr mtl = MaterialManager::Instance().Create(sName);
//    CommandScript cst;
//
//    cst.LoadStream(stream);
//
//    String cmd;
//    String param;
//    Technique * tech = NULL;
//
//    LOG_PRINT(String("************* Parse Material: ") + mtl->GetName() + "*************\n");
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param = cst.ParseStringParam();
//
//            if (technique == cmd)
//            {
//                tech = mtl->CreateTechnique(param);
//                ReadTechnique(tech, cst);
//            }
//
//            else if (null == cmd)
//            {
//            }
//
//            else
//            {
//                LOG_PRINT("Error: unknow command '" + cmd + "' \n");
//            }
//        }
//    }
//
//    LOG_PRINT(String("************* Parse Material OK *************\n\n"));
//
//    cst.Shutdown();
//
//    return mtl;
//}
//
//void DefaultMaterialLoader::ReadTechnique(Technique * tech, CommandScript & cst)
//{
//    Pass * p;
//    String cmd, param;
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param = cst.ParseStringParam();
//
//            if (bracket_b == cmd)
//            {
//                continue;
//            }
//
//            else if (bracket_e == cmd)
//            {
//                break;
//            }
//
//            else if (scheme == cmd)
//            {
//                tech->SetSchemeName(param);
//            }
//
//            //render group
//            else if (group == cmd)
//            {
//                tech->SetRenderGroup(param.ToInt());
//            }
//
//            // force solid
//            else if (force_solid == cmd)
//            {
//                tech->SetForceSolid(param.Tobool());
//            }
//
//            else if (pass == cmd)
//            {
//                p = tech->CreatePass();
//                ReadPass(p, cst);
//            }
//
//            else if (null == cmd)
//            {
//
//            }
//
//            else
//            {
//                LOG_PRINT("Error: unknow command '" + cmd + "' \n");
//            }
//        }
//    }
//}
//
//void DefaultMaterialLoader::ReadPass(Pass * p, CommandScript & cst)
//{
//    TextureStage * stage;
//    String cmd;
//    String param1;
//    String param2;
//    String param3;
//    String param4;
//    String param5;
//    String param6;
//    String param7;
//    String param8;
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//            param3 = cst.ParseStringParam();
//            param4 = cst.ParseStringParam();
//            param5 = cst.ParseStringParam();
//            param6 = cst.ParseStringParam();
//            param7 = cst.ParseStringParam();
//            param8 = cst.ParseStringParam();
//
//            if (bracket_b == cmd)
//            {
//                continue;
//            }
//
//            else if (bracket_e == cmd)
//            {
//                break;
//            }
//
//            //lighting
//            else if (lighting == cmd)
//            {
//				int count = param2.ToInt();
//				p->SetLighting(GetLightingType(param1), count > 0 ? count : 1);
//            }
//
//            //ambient
//            else if (ambient == cmd)
//            {
//                p->SetAmbient(param1.ToFloat(),
//                              param2.ToFloat(),
//                              param3.ToFloat(),
//                              param4.ToFloat());
//            }
//
//            //emissive
//            else if (emissive == cmd)
//            {
//                p->SetEmissive(param1.ToFloat(),
//                               param2.ToFloat(),
//                               param3.ToFloat(),
//                               param4.ToFloat());
//            }
//
//            //diffuse
//            else if (diffuse == cmd)
//            {
//                p->SetDiffuse(param1.ToFloat(),
//                              param2.ToFloat(),
//                              param3.ToFloat(),
//                              param4.ToFloat());
//            }
//
//            //specular
//            else if (specular == cmd)
//            {
//                p->SetSpecular(param1.ToFloat(),
//                               param2.ToFloat(),
//                               param3.ToFloat(),
//                               param4.ToFloat());
//            }
//
//            //power
//            else if (power == cmd)
//            {
//                p->SetPower(param1.ToFloat());
//            }
//
//            //cull mode
//            else if (cull_mode == cmd)
//            {
//                p->SetCullMode(GetCullMode(param1));
//            }
//
//            //fill mode
//            else if (fill_mode == cmd)
//            {
//                p->SetFillMode(GetFillMode(param1));
//            }
//
//            //shade mode
//            else if (shade_mode == cmd)
//            {
//                p->SetShadeMode(GetShadeMode(param1));
//            }
//
//            //color write
//            else if (color_write == cmd)
//            {
//                p->SetColorWrite(GetColorWrite(param1));
//            }
//
//            //depth write
//            else if (depth_write == cmd)
//            {
//                p->SetDepthWrite(GetDeptchWrite(param1));
//            }
//
//            //depth check
//            else if (depth_check == cmd)
//            {
//                COMPARE_FUNCTION func = GetCompareFunc(param1);
//                bool enable = (func != CMP_ALWAYS);
//                p->SetDepthCheck(enable);
//                p->SetDepthFunction(func);
//            }
//
//            //depth bais
//            else if (depth_bias == cmd)
//            {
//                p->SetDepthBias(param1.ToFloat());
//                p->SetDepthBiasSlopeScale(param2.ToFloat());
//            }
//
//            //alpha_blend
//            else if (alpha_blend == cmd)
//            {
//                BLEND_FACTORY src = GetBlendFactory(param1);
//                BLEND_FACTORY dest = GetBlendFactory(param2);
//                BLEND_OPERATOR op = GetBlendOperator(param3);
//                bool enable = ((src != BLEND_ONE)   ||
//                               (dest != BLEND_ZERO) ||
//                               (op != BLENDOP_ADD));
//
//                p->SetAlphaBlendEnable(enable);
//                p->SetAlphaBlendSourceFactory(src);
//                p->SetAlphaBlendDestFactory(dest);
//                p->SetAlphaBlendOperator(op);
//            }
//
//            //alpha test
//            else if (alpha_test == cmd)
//            {
//                COMPARE_FUNCTION func = GetCompareFunc(param1);
//                int val = (int)param2.ToInt();
//                bool enable = (func != CMP_ALWAYS);
//
//                p->SetAlphaTestEnable(enable);
//                p->SetAlphaTestFunction(func);
//                p->SetAlphaTestReference(val);
//            }
//
//            //point size
//            else if (point_size == cmd)
//            {
//                p->SetPointMinSize(param1.ToFloat());
//                p->SetPointMaxSize(param2.ToFloat());
//            }
//
//            //point scale a
//            else if (point_scale == cmd)
//            {
//                p->SetPointScaleA(param1.ToFloat());
//                p->SetPointScaleB(param2.ToFloat());
//                p->SetPointScaleC(param3.ToFloat());
//            }
//
//            //point sprite
//            else if (point_sprite == cmd)
//            {
//                p->SetPointSpriteEnable(GetPointSpriteEnable(param1));
//            }
//
//            //fog start
//            else if (fog == cmd)
//            {
//                p->SetFogStart(param1.ToFloat());
//                p->SetFogEnd(param2.ToFloat());
//                p->SetFogDensity(param3.ToFloat());
//                p->SetFogColor(param4.ToFloat(),
//                               param5.ToFloat(),
//                               param6.ToFloat(),
//                               param7.ToFloat());
//            }
//
//            //vertex shader
//            else if (vertex_shader == cmd)
//            {
//                p->SetVertexShader(param1);
//                ReadVertexShader(p, cst);
//            }
//
//            //pixel shader
//            else if (pixel_shader == cmd)
//            {
//                p->SetPixelShader(param1);
//                ReadPixelShader(p, cst);
//            }
//
//            //texture stage
//            else if (texture_stage == cmd)
//            {
//                stage = p->CreateTextureStage(param1);
//                ReadTextureStage(stage, cst);
//            }
//
//            //st_instance
//            else if (st_instance == cmd)
//            {
//                p->SetSTInstance(param1);
//            }
//
//            else if (st_paramloat4 == cmd)
//            {
//                ShaderParam * f4 = p->GetPixelShaderParamTable()->GetParam(param1);
//
//                Vec4 v;
//                v.x = param1.ToFloat();
//                v.y = param2.ToFloat();
//                v.z = param3.ToFloat();
//                v.w = param4.ToFloat();
//
//                f4->SetVector(v);
//            }
//
//            else if (null == cmd)
//            {
//            }
//
//            else
//            {
//                LOG_PRINT("Error: unknow command '" + cmd + "' \n");
//            }
//
//        }
//    }
//}
//
//void DefaultMaterialLoader::ReadTextureStage(TextureStage * stage, CommandScript & cst)
//{
//    String cmd;
//    String param1;
//    String param2;
//    String param3;
//    String param4;
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//            param3 = cst.ParseStringParam();
//            param4 = cst.ParseStringParam();
//
//            if (bracket_b == cmd)
//            {
//                continue;
//            }
//
//            else if (bracket_e == cmd)
//            {
//                break;
//            }
//
//            //texture
//            else if (texture == cmd)
//            {
//                TexturePtr pTexture = VideoBufferManager::Instance().FindTexture(param1);
//               
//                if (pTexture.IsNull())
//                {
//                    int iMipmap = param2.ToInt();
//                    iMipmap = iMipmap > 0 ? iMipmap : DEFAULT_MIPMAP_LEVEL;
//
//                    pTexture = VideoBufferManager::Instance().CreateTextureFromFile(param1,
//                                                                                    param1,
//                                                                                    iMipmap,
//                                                                                    USAGE_NORMAL,
//                                                                                    POOL_MANAGED);
//                }
//
//                stage->SetTexture(pTexture);
//            }
//
//            //volume texture
//            else if (volume_texture == cmd)
//            {
//                TexturePtr pTexture = VideoBufferManager::Instance().FindTexture(param1);
//
//                if (pTexture.IsNull())
//                {
//                    int iMipmap = param2.ToInt();
//                    iMipmap = iMipmap > 0 ? iMipmap : DEFAULT_MIPMAP_LEVEL;
//
//                    pTexture = VideoBufferManager::Instance().CreateVolumeTextureFromFile(param1,
//                                                                                          param1,
//                                                                                          iMipmap,
//                                                                                          USAGE_NORMAL,
//                                                                                          POOL_MANAGED);
//                }
//
//                stage->SetTexture(pTexture);
//            }
//
//            //cube texture
//            else if (cube_texture == cmd)
//            {
//                TexturePtr pTexture = VideoBufferManager::Instance().FindTexture(param1);
//
//                if (pTexture.IsNull())
//                {
//                    int iMipmap = param2.ToInt();
//                    iMipmap = iMipmap > 0 ? iMipmap : DEFAULT_MIPMAP_LEVEL;
//
//                    pTexture = VideoBufferManager::Instance().CreateCubeTextureFromFile(param1,
//                                                                                        param1,
//                                                                                        iMipmap,
//                                                                                        USAGE_NORMAL,
//                                                                                        POOL_MANAGED);
//                }
//
//                stage->SetTexture(pTexture);
//            }
//
//            //filter type
//            else if (filter_type == cmd)
//            {
//                stage->SetMagFilter(GetFilterType(param1));
//                stage->SetMinFilter(GetFilterType(param2));
//                stage->SetMipFilter(GetFilterType(param3));
//            }
//
//            //address u
//            else if (address_mode == cmd)
//            {
//                stage->SetAddressModeU(GetAddressMode(param1));
//                stage->SetAddressModeV(GetAddressMode(param1));
//                stage->SetAddressModeW(GetAddressMode(param1));
//            }
//
//            //max mipmap level
//            else if (maxmip_level == cmd)
//            {
//                stage->SetMaxMipLevel(param1.ToInt());
//            }
//
//            //max anisotropy
//            else if (max_anisotropy == cmd)
//            {
//                stage->SetMaxAnsitropy(param1.ToFloat());
//            }
//
//            //border color
//            else if (border_color == cmd)
//            {
//                stage->SetBorderColor(param1.ToInt(),
//                                      param2.ToInt(),
//                                      param3.ToInt(),
//                                      param4.ToInt());
//            }
//
//            //linker
//            else if (linker == cmd)
//            {
//                stage->SetTextureLinker(param1.ToInt());
//            }
//
//            else if (null == cmd)
//            {
//            }
//
//            else
//            {
//                LOG_PRINT("Error: unknow command '" + cmd + "' \n");
//            }
//        }
//    }
//
//}
//
//void DefaultMaterialLoader::ReadVertexShader(Pass * p, CommandScript & cst)
//{
//    String cmd;
//    String param1;
//    String param2;
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//
//            if (bracket_b == cmd)
//            {
//                continue;
//            }
//
//            else if (bracket_e == cmd)
//            {
//                break;
//            }
//
//            else if (shader_param == cmd)
//            {
//                ReadShaderParam(p->GetVertexShaderParamTable()->GetParam(param1), cst, param2);
//            }
//
//            else if (null == cmd)
//            {
//
//            }
//
//            else
//            {
//                LOG_PRINT("Error: unknow command '" + cmd + "' \n");
//            }
//        }
//    }
//}
//
//void DefaultMaterialLoader::ReadPixelShader(Pass * p, CommandScript & cst)
//{
//    String cmd;
//    String param1;
//    String param2;
//
//    while (cst.MoveToNextLine())
//    {
//        if (!cst.IsLineComment())
//        {
//            cmd = cst.ParseCommand();
//            param1 = cst.ParseStringParam();
//            param2 = cst.ParseStringParam();
//
//            if (bracket_b == cmd)
//            {
//                continue;
//            }
//
//            else if (bracket_e == cmd)
//            {
//                break;
//            }
//
//            else if (shader_param == cmd)
//            {
//                ReadShaderParam(p->GetPixelShaderParamTable()->GetParam(param1), cst, param2);
//            }
//
//            else if (null == cmd)
//            {
//
//            }
//
//            else
//            {
//                LOG_PRINT("Error: unknow command '" + cmd + "' \n");
//            }
//        }
//    }
//}
//
//
//void DefaultMaterialLoader::ReadShaderParam(ShaderParam * spp, CommandScript & cst, const String & type)
//{
//    assert (spp);
//
//    String param;
//    int m;
//
//#define __PARSE_INT_VAL(x, v)               \
//    for (m = 0; m < x;++m)                  \
//    {                                       \
//        param = cst.ParseStringParam();     \
//        (v)[m] = param.ToInt();             \
//    }
//
//#define __PARSE_FLOAT_VAL(x, v)             \
//    for (m = 0; m < x;++m)                  \
//    {                                       \
//        param = cst.ParseStringParam();     \
//        (v)[m] = param.ToFloat();           \
//    }                                       
//
//    if (IsCustom(type))
//    {
//        switch (GetType(type))
//        {
//        case SPT_INT1:
//            {
//                int i;
//                __PARSE_INT_VAL(1, &i);
//                spp->SetInt(i);
//            }
//            break;
//
//        case SPT_INT2:
//            {
//                Veci2 i;
//                __PARSE_INT_VAL(2, (int *)&i);
//                spp->SetVector(i);
//            }
//            break;
//
//        case SPT_INT3:
//            {
//                Veci3 i;
//                __PARSE_INT_VAL(3, (int *)&i);
//                spp->SetVector(i);
//            }
//            break;
//
//        case SPT_INT4:
//            {
//                Veci4 i;
//                __PARSE_INT_VAL(4, (int *)&i);
//                spp->SetVector(i);
//            }
//            break;
//
//        case SPT_FLOAT1:
//            {
//                float f;
//                __PARSE_FLOAT_VAL(1, (float *)&f);
//                spp->SetFloat(f);
//            }
//            break;
//
//        case SPT_FLOAT2:
//            {
//                Vec2 f;
//                __PARSE_FLOAT_VAL(2, (float*)&f);
//                spp->SetVector(f);
//            }
//            break;
//
//        case SPT_FLOAT3:
//            {
//                Vec3 f;
//                __PARSE_FLOAT_VAL(3, (float*)&f);
//                spp->SetVector(f);
//            }
//            break;
//
//        case SPT_FLOAT4:
//            {
//                Vec4 f;
//                __PARSE_FLOAT_VAL(4, (float*)&f);
//                spp->SetVector(f);
//            }
//            break;
//
//        case SPT_MATRIX3x3:
//            {
//                Mat3 f;
//                __PARSE_FLOAT_VAL(9, (float*)&f);
//                spp->SetMatrix(f);
//            }
//            break;
//
//        case SPT_MATRIX4x4:
//            {
//                Mat4 f;
//                __PARSE_FLOAT_VAL(16, (float*)&f);
//                spp->SetMatrix(f);
//            }
//            break;
//
//        default:
//            LOG_PRINT("Error: unknown param type '" + type + "' \n");
//            break;
//        }
//    }
//    else 
//    {
//        AUTO_SHADER_PARAMETER_TYPE t = GetAutoType(type);
//        int index = (int)cst.ParseStringParam().ToInt();
//
//        if (t == ASPT_UNKNOWN)
//            LOG_PRINT("Error: unknown param type '" + type + "' \n");
//
//        spp->SetAutoType(t);
//        spp->SetAutoIndex(index);
//    }
//
//#undef __PARSE_INT_VAL
//#undef __PARSE_FLOAT_VAL
//
//}
//
//
//
//
//
//void DefaultMaterialLoader::Save(MaterialPtr mtl, const String & filename)
//{
//    assert(mtl.NotNull());
//
//    File file;
//
//    file.Open(filename);
//
//    EXCEPTION_DEBUG(file.IsOpen(), filename + " can't open.");
//
//    file << "//**************************************************************************//" << File::ENDL;
//    file << "//                      Myway Material Script                               //" << File::ENDL;
//    file << "//**************************************************************************//" << File::ENDL;
//    file << File::ENDL;
//
//    for (int i = 0; i < mtl->GetTechniqueCount(); ++i)
//    {
//        WriteTechinque(mtl->GetTechnique(i), file);
//    }
//
//    file.Close();
//}
//
//
//void DefaultMaterialLoader::WriteTechinque(Technique * tech, File & file)
//{
//    file << technique << space << tech->GetName() << File::ENDL;
//    file << bracket_b << File::ENDL;
//
//    for (int i = 0; i < tech->GetPassCount(); ++i)
//    {
//        WritePass(tech->GetPass(i), file);
//    }
//
//    file << bracket_e << File::ENDL;
//}
//
//void DefaultMaterialLoader::WritePass(Pass * p, File & file)
//{
//    file << tab << pass << File::ENDL;
//    file << tab << bracket_b << File::ENDL;
//
//    //ambient
//    file << tab << tab
//        << ambient_a << space
//        << String(p->GetAmbient().r) << space
//        << String(p->GetAmbient().g) << space
//        << String(p->GetAmbient().b) << space
//        << String(p->GetAmbient().a) << space
//        << File::ENDL;
//
//    //emissive
//    file << tab << tab
//        << emissive_a << space
//        << String(p->GetEmissive().r) << space
//        << String(p->GetEmissive().g) << space
//        << String(p->GetEmissive().b) << space
//        << String(p->GetEmissive().a) << space
//        << File::ENDL;
//
//    //diffuse
//    file << tab << tab
//        << diffuse_a << space
//        << String(p->GetDiffuse().r) << space
//        << String(p->GetDiffuse().g) << space
//        << String(p->GetDiffuse().b) << space
//        << String(p->GetDiffuse().a) << space
//        << File::ENDL;
//
//    //specular
//    file << tab << tab
//        << specular_a << space
//        << String(p->GetSpecular().r) << space
//        << String(p->GetSpecular().g) << space
//        << String(p->GetSpecular().b) << space
//        << String(p->GetSpecular().a) << space
//        << File::ENDL;
//
//    //power
//    file << tab << tab
//        << ambient_a << space
//        << String(p->GetPower())
//        << File::ENDL;
//
//    //cull mode
//    file << tab << tab
//        << cull_mode_a << space 
//        << GetCullModeString(p->GetCullMode()) << File::ENDL;
//
//    //fill mode
//    file << tab << tab
//        << fill_mode_a << space 
//        << GetFillModeString(p->GetFillMode()) << File::ENDL;
//
//    //shade mode
//    file << tab << tab
//        << shade_mode_a << space 
//        << GetShadeModeString(p->GetShadeMode()) << File::ENDL;
//
//    //color write
//    file << tab << tab
//        << color_write_a << space
//        << GetColorWriteString(p->GetColorWrite()) << File::ENDL;
//
//    //depth write
//    file << tab << tab
//        << depth_write_a << space
//        << GetDeptchWriteString(p->GetDepthWriteEnable()) << File::ENDL;
//
//    //depth check
//    file << tab << tab
//        << depth_check_a << space
//        << GetDepthCheckString(p->GetDepthCheckEnable()) << File::ENDL;
//
//    //depth check func
//    file << tab << tab
//        << depth_func_a << space
//        << GetCompareFuncString(p->GetDepthCheckFuncion()) << File::ENDL;
//
//    //alpha blend
//    file << tab << tab
//        << alpha_blend_a << space
//        << GetAlphaBlendString(p->GetAlphaBlendEnable()) << File::ENDL;
//
//    //source blend
//    file << tab << tab
//        << src_blend_a << space
//        << GetBlendFactoryString(p->GetAlphaBlendSourceFactory()) << File::ENDL;
//
//    //dest blend
//    file << tab << tab
//        << dest_blend_a << space
//        << GetBlendFactoryString(p->GetAlphaBlendDestFactory()) << File::ENDL;
//
//    //blend operator
//    file << tab << tab
//        << blend_op_a << space
//        << GetBlendOperatorString(p->GetAlphaBlendOperator()) << File::ENDL;
//
//    //alpha test
//    file << tab << tab
//        << alpha_test_a << space
//        << GetAlphaTestString(p->GetAlphaTestEnable()) << File::ENDL;
//
//    //alpha test func
//    file << tab << tab
//        << alpha_test_func_a << space
//        << GetCompareFuncString(p->GetAlphaTestFunction()) << File::ENDL;
//
//    //alpha test ref
//    file << tab << tab
//        << alpha_test_ref_a << space
//        << String(p->GetAlphaTestReference()) << File::ENDL;
//
//    //point min
//    file << tab << tab
//        << point_min_a << space
//        << String(p->GetPointMinSize()) << File::ENDL;
//
//    //point max
//    file << tab << tab
//        << point_max_a << space
//        << String(p->GetPointMaxSize()) << File::ENDL;
//
//    //point scale a
//    file << tab << tab
//        << point_scale_a_a << space
//        << String(p->GetPointScaleA()) << File::ENDL;
//
//    //point scale b
//    file << tab << tab
//        << point_scale_b_a << space
//        << String(p->GetPointScaleB()) << File::ENDL;
//
//    //point scale c
//    file << tab << tab
//        << point_scale_c_a << space
//        << String(p->GetPointScaleC()) << File::ENDL;
//
//    //point sprite
//    file << tab << tab
//        << point_sprite_a << space
//        << GetPointSpriteEnableString(p->GetPointSpriteEnable()) << File::ENDL;
//
//    //fog start
//    file << tab << tab
//        << fog_start_a << space
//        << String(p->GetFogStart()) << File::ENDL;
//
//    //fog end
//    file << tab << tab
//        << fog_end_a << space
//        << String(p->GetFogEnd()) << File::ENDL;
//
//    //fog density
//    file << tab << tab
//        << fog_density_a << space
//        << String(p->GetFogDensity()) << File::ENDL;
//
//    //fog color
//    file << tab << tab
//        << fog_color_a << space
//        << String(p->GetFogColor().r) << space
//        << String(p->GetFogColor().g) << space
//        << String(p->GetFogColor().b) << space
//        << String(p->GetFogColor().a) << space
//        << File::ENDL;
//
//    //vertex shader
//    file << tab << tab
//        << vertex_shader_a
//        << space
//        << p->GetVertexShader()->GetName()
//        << File::ENDL;
//
//    //pixel shader
//    file << tab << tab
//        << pixel_shader_a
//        << space
//        << p->GetPixelShader()->GetName()
//        << File::ENDL
//        << File::ENDL;
//
//    //texture stage
//    for (int t = 0; t < p->GetTextureStageCount(); ++t)
//    {
//        WriteTextureStage(p->GetTextureStage(t), file);
//    }
//
//    file << tab << bracket_e << File::ENDL;
//    file << File::ENDL;
//}
//
//void DefaultMaterialLoader::WriteVertexShader(ShaderProgram * vs, File & file)
//{
//}
//
//void DefaultMaterialLoader::WritePixelShader(ShaderProgram * ps, File & file)
//{
//}
//
//void DefaultMaterialLoader::WriteTextureStage(TextureStage * stage, File & file)
//{
//    file << tab << tab
//        << texture_stage
//        << File::ENDL;
//
//    file << tab << tab
//        << bracket_b
//        << File::ENDL;
//
//    //texture
//    file << tab << tab << tab
//        << texture_a << space 
//        << stage->GetTexture()->GetName()
//        << space
//        << String(stage->GetTexture()->GetMipLevels())
//        << File::ENDL;
//
//    //filter type
//    file << tab << tab << tab
//        << min_filter_a << space 
//        << GetFilterTypeString(stage->GetMinFilter())
//        << File::ENDL;
//
//    file << tab << tab << tab
//        << mip_filter_a << space 
//        << GetFilterTypeString(stage->GetMipFilter())
//        << File::ENDL;
//
//    file << tab << tab << tab
//        << mag_filter_a << space 
//        << GetFilterTypeString(stage->GetMagFilter())
//        << File::ENDL;
//
//    //address mode
//    file << tab << tab << tab
//        << address_u_a << space 
//        << GetAddressModeString(stage->GetAddressModeU())
//        << File::ENDL;
//
//    file << tab << tab << tab
//        << address_v_a << space 
//        << GetAddressModeString(stage->GetAddressModeV())
//        << File::ENDL;
//
//    file << tab << tab << tab
//        << address_w_a << space 
//        << GetAddressModeString(stage->GetAddressModeW())
//        << File::ENDL;
//
//    //mip level
//    file << tab << tab << tab
//        << mipmap_level_a << space
//        << String(stage->GetMaxMipLevel())
//        << File::ENDL;
//
//    //max anisotropy
//    file << tab << tab << tab
//        << max_anisotropy_a << space
//        << String(stage->GetMaxAnsitropy())
//        << File::ENDL;
//
//    //border color
//    file << tab << tab << tab
//        << border_color_a << space
//        << String(stage->GetBorderColor().r) << space
//        << String(stage->GetBorderColor().g) << space
//        << String(stage->GetBorderColor().b) << space
//        << String(stage->GetBorderColor().a) << space
//        << File::ENDL;
//
//    //other is not support
//    //...
//    file << tab << tab
//        << bracket_e
//        << File::ENDL
//        << File::ENDL;
//}
//
//
//SHADER_PARAMETER_TYPE DefaultMaterialLoader::GetType(const String & type)
//{
//    if (type == paramnt1)
//        return SPT_INT1;
//
//    else if (type == paramnt2)
//        return SPT_INT2;
//
//    else if (type == paramnt3)
//        return SPT_INT3;
//
//    else if (type == paramnt4)
//        return SPT_INT4;
//
//    else if (type == paramloat1)
//        return SPT_FLOAT1;
//
//    else if (type == paramloat2)
//        return SPT_FLOAT2;
//
//    else if (type == paramloat3)
//        return SPT_FLOAT3;
//
//    else if (type == paramloat4)
//        return SPT_FLOAT4;
//
//    else if (type == paramMat3x3)
//        return SPT_MATRIX3x3;
//
//    else if (type == paramMat4x4)
//        return SPT_MATRIX4x4;
//
//    else
//        return SPT_UNKNOWN;
//}
//
//AUTO_SHADER_PARAMETER_TYPE DefaultMaterialLoader::GetAutoType(const String & type)
//{
//    if (type == paramMat_w)
//        return ASPT_WORLD_MATRIX;
//    else if (type == paramMat_v)
//        return ASPT_VIEW_MATRIX;
//    else if (type == paramMat_p)
//        return ASPT_PROJ_MATRIX;
//    else if (type == paramMat_wv)
//        return ASPT_WORLD_VIEW_MATRIX;
//    else if (type == paramMat_vp)
//        return ASPT_VIEW_PROJ_MATRIX;
//    else if (type == paramMat_wvp)
//        return ASPT_WORLD_VIEW_PROJ_MATRIX;
//    else if (type == paramMat_blend)
//        return ASPT_BLEND_MATRIX;
//    else if (type == paramMat_proj)
//        return ASPT_PROJECTION_MATRIX;
//
//    else if (type == paramMat_shadow_v)
//        return ASPT_SHADOW_VIEW_MATRIX;
//    else if (type == paramMat_shadow_p)
//        return ASPT_SHADOW_PROJ_MATRIX;
//    else if (type == paramMat_shadow_wv)
//        return ASPT_SHADOW_WORLD_VIEW_MATRIX;
//    else if (type == paramMat_shadow_vp)
//        return ASPT_SHADOW_VIEW_PROJ_MATRIX;
//    else if (type == paramMat_shadow_wvp)
//        return ASPT_SHADOW_WORLD_VIEW_PROJ_MATRIX;
//
//    else if (type == paramMtrl_ambient)
//        return ASPT_MATERIAL_AMBIENT;
//    else if (type == paramMtrl_emissive)
//        return ASPT_MATERIAL_EMISSIVE;
//    else if (type == paramMtrl_diffuse)
//        return ASPT_MATERIAL_DIFFUSE;
//    else if (type == paramMtrl_specular)
//        return ASPT_MATERIAL_SPECULAR;
//    else if (type == paramMtrl_power)
//        return ASPT_MATERIAL_POWER;
//
//    else if (type == param_light_ambient)
//        return ASPT_LIGHT_AMBIENT;
//    else if (type == param_light_diffuse)
//        return ASPT_LIGHT_DIFFUSE;
//    else if (type == param_light_specular)
//        return ASPT_LIGHT_SPECULAR;
//    else if (type == param_light_position)
//        return ASPT_LIGHT_POSITION;
//    else if (type == param_light_direction)
//        return ASPT_LIGHT_DIRECTION;
//    else if (type == param_light_attenuation)
//        return ASPT_LIGHT_ATTENUATION;
//    else if (type == param_light_spot_params)
//        return ASPT_LIGHT_SPOT_PARAMS;
//
//    else if (type == param_camera_position)
//        return ASPT_CAMERA_POSITION;
//    else if (type == param_camera_direction)
//        return ASPT_CAMERA_DIRECTION;
//    else if (type == param_camera_right)
//        return ASPT_CAMERA_RIGHT;
//    else if (type == param_camera_up)
//        return ASPT_CAMERA_UP;
//
//    else if (type == paramexture_size)
//        return ASPT_TEXTURE_SIZE;
//    else if (type == paramnv_texture_size)
//        return ASPT_INV_TEXTURE_SIZE;
//
//    else if (type == paramime)
//        return ASPT_TIME;
//    else if (type == param_sin_time)
//        return ASPT_SIN_TIME;
//    else if (type == param_cos_time)
//        return ASPT_COS_TIME;
//    else if (type == paraman_time)
//        return ASPT_TAN_TIME;
//    else if (type == paramime_2pi)
//        return ASPT_TIME_2PI;
//    else if (type == paramrame_time)
//        return ASPT_FRAME_TIME;
//    else if (type == paramps)
//        return ASPT_FPS;
//
//    else if (type == paramog_param)
//        return ASPT_FOG_PARAM;
//    else if (type == paramog_color)
//        return ASPT_FOG_COLOR;
//
//    else if (type == param_user)
//        return ASPT_USER;
//
//    else
//        return ASPT_UNKNOWN;
//}
//
//bool DefaultMaterialLoader::IsCustom(const String & type)
//{
//   return type == paramnt1 || type == paramnt2 || type == paramnt3 || type == paramnt4 ||
//          type == paramloat1 || type == paramloat2 || type == paramloat3 || type == paramloat4 ||
//          type == paramMat3x3 || type == paramMat4x4;
//}