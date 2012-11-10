#pragma once

#include "MWShaderNode.h"

namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Node_Sampler2D : public ST_Node
{
public:
    ST_Node_Sampler2D(ST_Instance * parent, const String & name);
    virtual ~ST_Node_Sampler2D();

    ST_NODE_TYPE    GetType() const;
    ST_VALUE_TYPE   GetValueType() const;
    String          Generte();
    
    void            NotifyDestroy(ST_Node * node);
    String          GetUniformParamDefine();

    void            SetInput(ST_Node * node);
    void            SetInput(const String & name);

    ST_Node *       GetInput();


    void            SetTexture(const String & texture);
    const String &  GetTexture() const;

protected:
    String          m_code;
    String          m_texture;
    ST_Node *       mnput;
};


}

}