#include "MWShaderLibraryLoader.h"
#include "MWShaderLibrary.h"
#include "MWResourceManager.h"

using namespace Myway;
using namespace Shader;

#define _diffuse        "diffuse"
#define _diffuse_power  "diffuse_power"
#define _specular       "specular"
#define _specular_power "specular_power"
#define _normal         "normal"
#define _transparency   "transparency"

#define _instance       "ST_Instance"
#define _lighting_mode  "lighting_mode"
#define _skeleton_anim  "skeleton_anim"
#define _shadow         "shadow"
#define _vs_profile     "vs_profile"
#define _ps_profile     "ps_profile"

#define _vs_1_1         "vs_1_1"
#define _vs_2_0         "vs_2_0"
#define _vs_3_0         "vs_3_0"

#define _ps_1_1         "ps_1_1"
#define _ps_1_2         "ps_1_2"
#define _ps_1_3         "ps_1_3"
#define _ps_2_0         "ps_2_0"
#define _ps_3_0         "ps_3_0"

#define _addition       "addition"
#define _subtract       "subtract"
#define _multiply       "multiply"
#define _clamp          "clamp"
#define _const_float1   "const_float1"
#define _const_float2   "const_float2"
#define _const_float3   "const_float3"
#define _const_float4   "const_float4"
#define _sampler2D      "sampler2D"
#define _paramloat4   "paramloat4"
#define _connection     "connection"
#define _shader         "shader"

bool Getboolean(const String & param)
{
    if (param == "true")
        return TRUE;
    else if (param == "false")
        return FALSE;
    else
    {
        assert(0);
        LOG_PRINT("unknown parameter: " + param + ".\n");
        return FALSE;
    }
}

LIGHTING_MODE GetLightingMode(const String & param)
{
    if (param == "phone")
        return LM_PHONE;
    else if (param == "none")
        return LM_NONE;
    else
    {
        assert(0);
        LOG_PRINT("unknown parameter: " + param + ".\n");
        return LM_NONE;
    }
}

SHADER_PROGRAM_PROFILE GetShaderProfile(const String & param)
{
    if (param == _vs_1_1)
        return SHADER_PROFILE_VS_1_1;
    else if (param == _vs_2_0)
        return SHADER_PROFILE_VS_2_0;
    else if (param == _vs_3_0)
        return SHADER_PROFILE_VS_3_0;

    else if (param == _ps_1_1)
        return SHADER_PROFILE_PS_1_1;
    else if (param == _ps_1_2)
        return SHADER_PROFILE_PS_1_2;
    else if (param == _ps_1_3)
        return SHADER_PROFILE_PS_1_3;
    else if (param == _ps_2_0)
        return SHADER_PROFILE_PS_2_0;
    else if (param == _ps_3_0)
        return SHADER_PROFILE_PS_3_0;

    else
    {
        assert(0);
        LOG_PRINT("unknown shader profile: " + param + ".\n");
        return SHADER_PROFILE_UNKNOWN;
    }

}

ST_Loader_Default::ST_Loader_Default()
{
}

ST_Loader_Default::~ST_Loader_Default()
{
}

ST_InstancePtr ST_Loader_Default::Load(const String & name, const String & source)
{
    DataStreamPtr stream = ResourceManager::Instance().OpenResource(source);

    if (stream.IsNull())
    {
        LOG_PRINT("FileSystem Error: " + source + " don't find in any file systems.\n\n");
        return ST_InstancePtr();
    }

    ST_InstancePtr inst = ST_Library::Instance().CreateSTInstance(name);

    CommandScript cst;
    cst.LoadStream(stream);

    LOG_PRINT(String("\n<-    Parse ST_Instance: ") + source + "    ->\n");

    String cmd, param;
    bool flag = false, beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == _instance)
            {
                flag = true;
            }

            else if (flag && !beg && cmd == "{")
            {
                beg = true;
            }

            // light mode
            else if (beg && cmd == _lighting_mode)
            {
                inst->SetLightingMode(GetLightingMode(param));
            }

            // skeleton animation?
            else if (beg && cmd == _skeleton_anim)
            {
                inst->SetSkeletonAnimation(Getboolean(param));
            }

            // shadow?
            else if (beg && cmd == _shadow)
            {
                inst->SetShadowEnable(Getboolean(param));
            }

            else if (beg && cmd == _vs_profile)
            {
                inst->SetVertexShaderProfile(GetShaderProfile(param));
            }

            else if (beg && cmd == _ps_profile)
            {
                inst->SetPixelShaderProfile(GetShaderProfile(param));
            }

            // addition
            else if (beg && cmd == _addition)
            {
                ReadAddition(inst, param, cst);
            }

            // subtrat
            else if (beg && cmd == _subtract)
            {
                ReadSubtract(inst, param, cst);
            }

            // multiply
            else if (beg && cmd == _multiply)
            {
                ReadMultiply(inst, param, cst);
            }

            // clamp
            else if (beg && cmd == _clamp)
            {
                ReadClamp(inst, param, cst);
            }

            // sampler2D
            else if (beg && cmd == _sampler2D)
            {
                ReadSampler2D(inst, param, cst);
            }

            // const float1
            else if (beg && cmd == _const_float1)
            {
                ReadConstFloat1(inst, param, cst);
            }

            // const float2
            else if (beg && cmd == _const_float2)
            {
                ReadConstFloat2(inst, param, cst);
            }

            // const float3
            else if (beg && cmd == _const_float3)
            {
                ReadConstFloat3(inst, param, cst);
            }

            // const float4
            else if (beg && cmd == _const_float4)
            {
                ReadConstFloat4(inst, param, cst);
            }

            // param float4
            else if (beg && cmd == _paramloat4)
            {
                ReadParamFloat4(inst, param, cst);
            }

            else if (beg && cmd == _connection)
            {
                ReadConnection(inst, param, cst);
            }

            else if (beg && cmd == _shader)
            {
                ReadShader(inst, cst);
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }

    LOG_PRINT(String("<-    end    ->\n\n"));

    return inst;
}

void ST_Loader_Default::ReadSampler2D(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_Sampler2D * node = (ST_Node_Sampler2D *)inst->CreateNode(name, STNODE_SAMPLER2D);

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "texture")
            {
                node->SetTexture(param);
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }
            
            else if (cmd == "")
            {

            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadAddition(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_Add * node = (ST_Node_Add *)inst->CreateNode(name, STNODE_ADD);

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (cmd == "")
            {

            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadSubtract(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_Sub * node = (ST_Node_Sub *)inst->CreateNode(name, STNODE_SUB);

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (cmd == "")
            {

            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadMultiply(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_Multiply * node = (ST_Node_Multiply *)inst->CreateNode(name, STNODE_MULTIPLY);

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();


            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (cmd == "")
            {

            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadClamp(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_Clamp * node = (ST_Node_Clamp *)inst->CreateNode(name, STNODE_CLAMP);

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConstFloat1(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_ConstFloat1 * node = (ST_Node_ConstFloat1 *)inst->CreateNode(name, STNODE_CONST_FLOAT1);

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();


            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "value")
            {
                node->SetValue(param.ToFloat());
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConstFloat2(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_ConstFloat2 * node = (ST_Node_ConstFloat2 *)inst->CreateNode(name, STNODE_CONST_FLOAT2);

    String cmd, param0, param1;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param0 = cst.ParseStringParam();
            param1 = cst.ParseStringParam();


            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "value")
            {
                Vec2 v;
                v.x = param0.ToFloat();
                v.y = param1.ToFloat();

                node->SetValue(v);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConstFloat3(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_ConstFloat3 * node = (ST_Node_ConstFloat3 *)inst->CreateNode(name, STNODE_CONST_FLOAT3);

    String cmd, param0, param1, param2;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param0 = cst.ParseStringParam();
            param1 = cst.ParseStringParam();
            param2 = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "value")
            {
                Vec3 v;
                v.x = param0.ToFloat();
                v.y = param1.ToFloat();
                v.z = param2.ToFloat();

                node->SetValue(v);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConstFloat4(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_ConstFloat4 * node = (ST_Node_ConstFloat4 *)inst->CreateNode(name, STNODE_CONST_FLOAT4);

    String cmd, param0, param1, param2, param3;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param0 = cst.ParseStringParam();
            param1 = cst.ParseStringParam();
            param2 = cst.ParseStringParam();
            param3 = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "value")
            {
                Vec4 v;
                v.x = param0.ToFloat();
                v.y = param1.ToFloat();
                v.z = param2.ToFloat();
                v.w = param3.ToFloat();

                node->SetValue(v);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadParamFloat4(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node_ParamFloat4 * node = (ST_Node_ParamFloat4 *)inst->CreateNode(name, STNODE_PARAM_FLOAT4);

    String cmd, param0, param1, param2, param3;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param0 = cst.ParseStringParam();
            param1 = cst.ParseStringParam();
            param2 = cst.ParseStringParam();
            param3 = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "value")
            {
                Vec4 v;
                v.x = param0.ToFloat();
                v.y = param1.ToFloat();
                v.z = param2.ToFloat();
                v.w = param3.ToFloat();

                node->SetValue(v);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConnection(ST_InstancePtr inst, const String & name, CommandScript & cst)
{
    ST_Node * node = inst->GetNode(name);

    if (!node)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: unknown node '" + name + "';\n");
    }

    switch (node->GetType())
    {
    case STNODE_SAMPLER2D:
        ReadConnectionSampler2D(node, cst);
        break;

    case STNODE_ADD:
        ReadConnectionAddition(node, cst);
        break;

    case STNODE_SUB:
        ReadConnectionSubtract(node, cst);
        break;

    case STNODE_CLAMP:
        ReadConnectionClamp(node, cst);
        break;

    case STNODE_MULTIPLY:
        ReadConnectionMultiply(node, cst);
        break;

    default:
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: invalidate connection '" + name + "';\n");
        break;
    }
}

void ST_Loader_Default::ReadConnectionSampler2D(ST_Node * node, CommandScript & cst)
{
    ST_Node_Sampler2D * _node = (ST_Node_Sampler2D *)node;

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "input0")
            {
                _node->SetInput(param);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + _node->GetParent()->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConnectionAddition(ST_Node * node, CommandScript & cst)
{
    ST_Node_Add * _node = (ST_Node_Add *)node;

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "input0")
            {
                _node->SetInput0(param);
            }

            else if (beg && cmd == "input1")
            {
                _node->SetInput1(param);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + _node->GetParent()->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConnectionSubtract(ST_Node * node, CommandScript & cst)
{
    ST_Node_Sub * _node = (ST_Node_Sub *)node;

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "input0")
            {
                _node->SetInput0(param);
            }

            else if (beg && cmd == "input1")
            {
                _node->SetInput1(param);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + _node->GetParent()->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConnectionMultiply(ST_Node * node, CommandScript & cst)
{
    ST_Node_Multiply * _node = (ST_Node_Multiply *)node;

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "input0")
            {
                _node->SetInput0(param);
            }

            else if (beg && cmd == "input1")
            {
                _node->SetInput1(param);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + _node->GetParent()->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadConnectionClamp(ST_Node * node, CommandScript & cst)
{
    ST_Node_Clamp * _node = (ST_Node_Clamp *)node;

    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == "input0")
            {
                _node->SetInput0(param);
            }

            else if (beg && cmd == "input1")
            {
                _node->SetInput1(param);
            }

            else if (beg && cmd == "input2")
            {
                _node->SetInput2(param);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + _node->GetParent()->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}

void ST_Loader_Default::ReadShader(ST_InstancePtr inst, CommandScript & cst)
{
    String cmd, param;
    bool beg = false, end = false;
    while (cst.MoveToNextLine())
    {
        if (!cst.IsLineComment())
        {
            cmd = cst.ParseCommand();
            param = cst.ParseStringParam();

            if (cmd == "{")
            {
                beg = true;
            }

            else if (beg && cmd == "}")
            {
                end = true;
                break;
            }

            else if (beg && cmd == _diffuse)
            {
                inst->SetDiffuse(param);
            }

            else if (beg && cmd == _diffuse_power)
            {
                inst->SetDiffusePower(param);
            }

            else if (beg && cmd == _specular)
            {
                inst->SetSpecular(param);
            }

            else if (beg && cmd == _specular_power)
            {
                inst->SetSpecularPower(param);
            }

            else if (beg && cmd == _normal)
            {
                inst->SetNormal(param);
            }

            else if (beg && cmd == _transparency)
            {
                inst->SetTransparency(param);
            }

            else if (cmd == "")
            {
            }

            else
            {
                assert(0);
                LOG_PRINT("unknown command: " + cmd + ".\n");
            }
        }
    }

    if (!beg || !end)
    {
        EXCEPTION("Parse Instance '" + inst->GetName() + "' error: do you forget '{' or '}'. \n");
    }
}














void ST_Loader_Default::Save(const String & filename, ST_InstancePtr inst)
{
    //...
}