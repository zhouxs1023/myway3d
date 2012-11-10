#include "MWShaderLibrary.h"
#include "MWShaderProgramManager.h"

using namespace Myway;
using namespace Shader;

ST_Library::ST_Library()
{
    m_vsprofile = SHADER_PROFILE_VS_2_0;
    msprofile = SHADER_PROFILE_PS_2_0;

    m_loader = new ST_Loader_Default();
}

ST_Library::~ST_Library()
{
    safe_delete(m_loader);
    DestroyAllSTInstnace();
}

ST_InstancePtr ST_Library::CreateSTInstance(const String & name)
{
    assert (GetSTInstance(name).IsNull());

    ST_Instance * inst = new ST_Instance(name);
    inst->SetVertexShaderProfile(m_vsprofile);
    inst->SetPixelShaderProfile(msprofile);
    m_InstanceMap.Insert(name, inst);

    return ST_InstancePtr(inst);
}

ST_InstancePtr ST_Library::GetSTInstance(const String & name)
{
    InstanceMap::Iterator iter;
    InstanceMap::Iterator end;

    iter = m_InstanceMap.Find(name);
    end = m_InstanceMap.End();

    return iter != end ? ST_InstancePtr(iter->second) : ST_InstancePtr(NULL);
}

void ST_Library::DestroySTInstance(const String & name)
{
    InstanceMap::Iterator iter;
    InstanceMap::Iterator end;

    iter = m_InstanceMap.Find(name);
    end = m_InstanceMap.End();

    assert (iter != end);

    delete iter->second;
    m_InstanceMap.Erase(iter);
}

void ST_Library::DestroySTInstance(ST_Instance * inst)
{
    DestroySTInstance(inst->GetName());
}

void ST_Library::DestroyAllSTInstnace()
{
    InstanceMap::Iterator iter;
    InstanceMap::Iterator end;

    iter = m_InstanceMap.Begin();
    end = m_InstanceMap.End();

    while (iter != end)
    {
        delete iter->second;
        ++iter;
    }

    m_InstanceMap.Clear();
}

void ST_Library::SetLoader(ST_Loader * loader)
{
    safe_delete(m_loader);
}

ST_InstancePtr ST_Library::Load(const String & name, const String & source)
{
    assert (m_loader);
    ST_InstancePtr inst = m_loader->Load(name, source);
    inst->GenerteShader();
    return inst;
}

void ST_Library::Save(const String & filename, ST_InstancePtr inst)
{
    assert (m_loader);

    m_loader->Save(filename, inst);
}

void ST_Library::SetDefaultVertexShaderProfile(SHADER_PROGRAM_PROFILE profile)
{
    assert (ShaderProgramManager::IsVertexShader(profile));
    m_vsprofile = profile;
}

void ST_Library::SetDefaultPixelShaderProfile(SHADER_PROGRAM_PROFILE profile)
{
    assert (ShaderProgramManager::IsPixelShader(profile));
    msprofile = profile;
}

SHADER_PROGRAM_PROFILE ST_Library::GetDefaultVertexShaderProfile() const
{
    return m_vsprofile;
}

SHADER_PROGRAM_PROFILE ST_Library::GetDefaultPixelShaderProfile() const
{
    return msprofile;
}