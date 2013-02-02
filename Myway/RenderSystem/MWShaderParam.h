/* 
--------------------------------------------------------------------------------------
                        Myway Engine

        desc:       Shader parameter
        author:     Myway
        email:      404793124@qq.com

--------------------------------------------------------------------------------------
*/

#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

struct MW_ENTRY  ShaderParamDesc
{
    DECLARE_ALLOC();

    TString32 name;
    SHADER_PARAM_TYPE type;
    SHADER_PARAM_BIND_TYPE btype;
    int bindex;
    int count;
    int reg;
    Vec4 val;

    ShaderParamDesc() : type(SPT_UNKNOWN), btype(SPBT_UNKNOWN), bindex(0), count(1), reg(0), val(0, 0, 0, 0){}
};

class ShaderParamTable;

class MW_ENTRY ShaderParam : public AllocObj
{
    friend class ShaderParamTable;
public:
    static int                  GetTypeSize(SHADER_PARAM_TYPE type);

public:
                                ShaderParam();
                                ~ShaderParam();

    void                        SetUnifom(float x, float y, float z, float w);
    void                        SetFloat(float x);
    void                        SetVector(const Vec4 & v);
    void                        SetMatrix(const Mat4 & m);
    void                        SetColor(const Color4 & c);

    void                        SetVector(const Vec4 * v, int count);
	void                        SetMatrix(const Mat4 * m, int count);
    void                        SetMatrix(const Mat3x4 * m, int count);
    void                        SetColor(const Color4 * c, int count);


    void                        Set(const void * data, int size);
    void                        Get(void * data, int size) const;
    const void *                GetData() const;

    void                        SetBindType(SHADER_PARAM_BIND_TYPE type);
    void                        SetBindIndex(int index);

    SHADER_PARAM_TYPE           GetType() const;
    SHADER_PARAM_BIND_TYPE      GetBindType() const;
    int                         GetBindIndex() const;
    const TString32 &           GetName() const;
    int                         GetCount() const;
    int                         GetSize() const;
    int                         GetRealSize() const;
    int                         GetPosition() const;
    int                         GetRegister() const;
    bool                        IsInt() const;
    bool                        IsAuto() const;

    int                         GetVec4Count() const;


protected:
    ShaderParamTable *          mParent;
    TString32                   mName;
    SHADER_PARAM_TYPE           mType;
    SHADER_PARAM_BIND_TYPE      mBType;
    int                         mBIndex;
    int                         mCount;
    int                         mSize;
    int                         mPosition;
    int                         mRegister;
};


class MW_ENTRY ShaderParamTable
{
    DECLARE_ALLOC();

    static const int BUFFER_SIZE = 256 * sizeof(Vec4);

public:
                                ShaderParamTable();
                                ~ShaderParamTable();

    ShaderParam *               AddParam(const TString32 & name,
                                         SHADER_PARAM_TYPE type, 
                                         int count,
                                         int reg,
                                         SHADER_PARAM_BIND_TYPE autotype = SPBT_UNKNOWN,
                                         int autoindex = 0);

    void                        Clear();

    int                         GetNumParam() const;
    ShaderParam *               GetParam(int index);
    ShaderParam *               GetParam(const TString32 & name);

    void                        SetData(int pos, const void * data, int size);
    void                        GetData(int pos, void * data, int size);
    const void *                GetData(int pos);

    void                        Update();

protected:
    Array<ShaderParam>          mParams;
    unsigned char               mBuffer[BUFFER_SIZE];
    int                         mSize;
};

}