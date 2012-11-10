#pragma once

#include "MWShaderNode.h"


namespace Myway
{

namespace Shader
{

class MW_ENTRY ST_Node_Clamp: public ST_Node
{
public:
    ST_Node_Clamp(ST_Instance * parent, const String & name);
    virtual ~ST_Node_Clamp();

    ST_NODE_TYPE    GetType() const;
    ST_VALUE_TYPE   GetValueType() const;
    String          Generte();
    
    void            NotifyDestroy(ST_Node * node);
    String          GetUniformParamDefine();
    
    void            SetInput0(ST_Node * node);
    void            SetInput1(ST_Node * node);
    void            SetInput2(ST_Node * node);
    void            SetInput0(const String & name);
    void            SetInput1(const String & name);
    void            SetInput2(const String & name);


    ST_Node *       GetInput0();
    ST_Node *       GetInput1();
    ST_Node *       GetInput2();

protected:
    String          m_code;
    ST_Node *       mnput0;
    ST_Node *       mnput1;
    ST_Node *       mnput2;
};

}

}