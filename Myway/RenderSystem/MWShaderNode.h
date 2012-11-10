#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

namespace Shader
{

enum ST_VALUE_TYPE
{
    STVAL_UNKNOWN,

    STVAL_FLOAT1,
    STVAL_FLOAT2,
    STVAL_FLOAT3,
    STVAL_FLOAT4,

    MW_ALIGN_ENUM(ST_VALUE_TYPE)
};

enum ST_NODE_TYPE
{
    STNODE_INTERNAL,        //not for user

    STNODE_SAMPLER2D,
    STNODE_MULTIPLY,
    STNODE_ADD,
    STNODE_SUB,
    STNODE_CLAMP,
    STNODE_CONST_FLOAT1,
    STNODE_CONST_FLOAT2,
    STNODE_CONST_FLOAT3,
    STNODE_CONST_FLOAT4,
    STNODE_PARAM_FLOAT4,

    MW_ALIGN_ENUM(ST_NODE_TYPE)
};

class ST_Instance;


class MW_ENTRY ST_Node : public AllocObj
{
public:
    ST_Node(ST_Instance * parent, const String & name) 
        : marent(parent), m_name(name), m_generted(FALSE) { mMyvar = name + "_var"; }

    virtual ~ST_Node()  {}

    ST_Instance *           GetParent() const   { return marent; }
    virtual const String &  GetName() const     { return m_name; }
    virtual const String &  GetVarName() const  { return mMyvar; }

            bool            IsGenerated() const { return m_generted; }

    virtual ST_NODE_TYPE    GetType() const = 0;
    virtual ST_VALUE_TYPE   GetValueType() const = 0;
    virtual String          Generte() = 0;
    virtual void            NotifyDestroy(ST_Node * node) = 0;
    virtual String          GetUniformParamDefine() = 0;

protected:
    ST_Instance *   marent;
    String          m_name;
    String          mMyvar;
    bool            m_generted;
};



} // Shader

} // Myway
