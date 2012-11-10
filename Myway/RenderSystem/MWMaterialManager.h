#pragma once

#include "MWMaterialLoader.h"

namespace Myway
{

//class MW_ENTRY MaterialManager : public Singleton<MaterialManager>
//{
//    typedef HashMap<String, Material*, String::hash_t>  MaterialMap;
//public:
//    MaterialManager();
//    ~MaterialManager();
//
//    void SetLoader(MaterialLoader * loader);
//
//    MaterialPtr Create(const String & sName);
//    MaterialPtr Find(const String & sName);
//    MaterialPtr Clone(const String & sNewName, const String & sSrcName);
//    MaterialPtr Clone(const String & sNewName, MaterialPtr src);
//
//    void Destroy(Material * mtl);
//
//    MaterialPtr Load(const String & sName, const String & sSource);
//    void Save(MaterialPtr mtl, const String & sFileName);
//
//
//protected:
//    MaterialMap                                     m_Materials;
//    MaterialLoader *                                m_Loader;
//    TextureLinker                                   m_Linker;
//};

}