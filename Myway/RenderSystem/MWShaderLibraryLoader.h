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
#include "MWShaderCodeGenerter.h"
#include "MWShaderCodeFactory.h"
#include "MWShaderInstance.h"

namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Loader : public AllocObj
{
public:
    ST_Loader() {}
    virtual ~ST_Loader() {}

    virtual ST_InstancePtr Load(const String & name, const String & source) = 0;
    virtual void Save(const String & filename, ST_InstancePtr inst) = 0;
};


class MW_ENTRY ST_Loader_Default : public ST_Loader
{
public:
    ST_Loader_Default();
    virtual ~ST_Loader_Default();

    virtual ST_InstancePtr Load(const String & name, const String & source);
    virtual void Save(const String & filename, ST_InstancePtr inst);

protected:
    void ReadSampler2D(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadAddition(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadSubtract(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadMultiply(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadClamp(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadConstFloat1(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadConstFloat2(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadConstFloat3(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadConstFloat4(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadParamFloat4(ST_InstancePtr inst, const String & name, CommandScript & cst);

    void ReadConnection(ST_InstancePtr inst, const String & name, CommandScript & cst);
    void ReadConnectionSampler2D(ST_Node * node, CommandScript & cst);
    void ReadConnectionAddition(ST_Node * node, CommandScript & cst);
    void ReadConnectionSubtract(ST_Node * node, CommandScript & cst);
    void ReadConnectionMultiply(ST_Node * node, CommandScript & cst);
    void ReadConnectionClamp(ST_Node * node, CommandScript & cst);

    void ReadShader(ST_InstancePtr inst, CommandScript & cst);


};

}

}