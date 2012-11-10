#include "MWMaterialManager.h"

using namespace Myway;

//MaterialManager::MaterialManager()
//{
//    m_Loader = new DefaultMaterialLoader();
//}
//
//MaterialManager::~MaterialManager()
//{
//    safe_delete(m_Loader);
//    assert(m_Materials.Size() == 0);
//}
//
//void MaterialManager::SetLoader(MaterialLoader * loader)
//{
//    safe_delete(m_Loader);
//    m_Loader = loader;
//}
//
//MaterialPtr MaterialManager::Create(const String & sName)
//{
//    assert(Find(sName).IsNull());
//
//    Material * mtl = new Material(sName);
//    m_Materials.Insert(sName, mtl);
//
//    return MaterialPtr(mtl);
//}
//
//
//MaterialPtr MaterialManager::Find(const String & sName)
//{
//    MaterialMap::Iterator iter = m_Materials.Find(sName);
//    MaterialMap::Iterator end = m_Materials.End();
//
//    return iter != end ? MaterialPtr(iter->second) : MaterialPtr();
//}
//
//MaterialPtr MaterialManager::Clone(const String & sNewName, const String & sSrcName)
//{
//    assert(Find(sNewName).IsNull());
//
//    MaterialPtr src = Find(sSrcName);
//
//    assert(src.NotNull());
//
//    Material * mtl = new Material(sNewName);
//
//    mtl->Clone(src.GetPointer());
//
//    m_Materials.Insert(sNewName, mtl);
//
//    return MaterialPtr(mtl);
//}
//
//MaterialPtr MaterialManager::Clone(const String & sNewName, MaterialPtr src)
//{
//    assert(Find(sNewName).IsNull());
//
//    Material * mtl = new Material(sNewName);
//
//    mtl->Clone(src.GetPointer());
//
//    m_Materials.Insert(sNewName, mtl);
//
//    return MaterialPtr(mtl);
//}
//
//void MaterialManager::Destroy(Material * mtl)
//{
//    MaterialMap::Iterator iter = m_Materials.Find(mtl->GetName());
//    MaterialMap::Iterator end = m_Materials.End();
//
//    assert(iter != end);
//
//    delete iter->second;
//    m_Materials.Erase(iter);
//}
//
//MaterialPtr MaterialManager::Load(const String & sName, const String & sFileName)
//{
//    assert(m_Loader && Find(sName).IsNull());
//
//    MaterialPtr mtl = m_Loader->Load(sName, sFileName);
//
//    return mtl;
//}
//
//void MaterialManager::Save(MaterialPtr mtl, const String & sFileName)
//{
//    m_Loader->Save(mtl, sFileName);
//}
//
//
//
//
//
//
//
//
//

