/* 
--------------------------------------------------------------------------------------
                        Myway Engine

        desc:       Shader Library
        author:     Myway
        email:      404793124@qq.com

--------------------------------------------------------------------------------------
*/

#pragma once

#include "MWRenderDefine.h"

#include "MWShaderNode.h"
#include "MWShaderNodeAdd.h"
#include "MWShaderNodeSub.h"
#include "MWShaderNodeMultiply.h"
#include "MWShaderNodeClamp.h"
#include "MWShaderNodeConstFloat1.h"
#include "MWShaderNodeConstFloat2.h"
#include "MWShaderNodeConstFloat3.h"
#include "MWShaderNodeConstFloat4.h"
#include "MWShaderNodeParamFloat4.h"
#include "MWShaderNodeSampler2D.h"
#include "MWShaderCodeGenerter.h"
#include "MWShaderCodeFactory.h"
#include "MWShaderInstance.h"
#include "MWShaderLibraryLoader.h"

namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Library : public Singleton<ST_Library>
{
    typedef HashMap<String, ST_Instance*, String::hash_t>   InstanceMap;

public:
    ST_Library();
    ~ST_Library();

    ST_InstancePtr CreateSTInstance(const String & name);
    ST_InstancePtr GetSTInstance(const String & name);
    void           DestroySTInstance(const String & name);
    void           DestroySTInstance(ST_Instance * inst);
    void           DestroyAllSTInstnace();

    void           SetLoader(ST_Loader * loader);

    ST_InstancePtr Load(const String & name, const String & source);
    void           Save(const String & filename, ST_InstancePtr inst);

    void SetDefaultVertexShaderProfile(SHADER_PROGRAM_PROFILE profile);
    void SetDefaultPixelShaderProfile(SHADER_PROGRAM_PROFILE profile);
    SHADER_PROGRAM_PROFILE GetDefaultVertexShaderProfile() const;
    SHADER_PROGRAM_PROFILE GetDefaultPixelShaderProfile() const;

protected:
    InstanceMap             m_InstanceMap;
    ST_Code_Factory         mactor;
    ST_Loader *             m_loader;
    SHADER_PROGRAM_PROFILE m_vsprofile;
    SHADER_PROGRAM_PROFILE msprofile;
};

}

}