#include "MWFXComposerManager.h"
#include "MWFXComposerImport.h"

using namespace Myway;

FXComposerManager::FXComposerManager()
: mActiveFX(NULL),
  m_RectObj(NULL)
{
    m_RectObj = new FXComposerRectObj();
}

FXComposerManager::~FXComposerManager()
{
    DestroyAll();
    delete m_RectObj;
}

FXComposer * FXComposerManager::CreateComposer(const String & sName)
{
    EXCEPTION_DEBUG(!FindByName(sName), "Composer exist.");

    FXComposer * fx = new FXComposer(sName);
    m_Composers.PushBack(fx);
    return fx;
}

FXComposer * FXComposerManager::CreateComposerFromFile(const String & sName, const String & sFileName)
{
    EXCEPTION_DEBUG(!FindByName(sName), "Composer exist.");

    FXComposer * fx = new FXComposer(sName);
    FXComposerImport::Import(fx, sFileName);
    m_Composers.PushBack(fx);
    return fx;
}

FXComposer * FXComposerManager::FindByName(const String & sName)
{
    for (int i = 0; i < m_Composers.Size(); ++i)
    {
        if (m_Composers[i]->GetName() == sName)
            return m_Composers[i];
    }

    return NULL;
}

void FXComposerManager::Active(const String & sName)
{
    FXComposer * fx = FindByName(sName);

    if (fx)
        fx->Active();

    mActiveFX = fx;
}

void FXComposerManager::Active(FXComposer * fx)
{
    if (fx)
        fx->Active();

    mActiveFX = fx;
}

FXComposer * FXComposerManager::GetActiveComposer()
{
    return mActiveFX;
}

void FXComposerManager::DestroyComposer(const String & sName)
{
    FXComposer * fx;
    Vector<FXComposer*>::Iterator iter;
    Vector<FXComposer*>::Iterator end;
    iter = m_Composers.Begin();
    end = m_Composers.End();

    while (iter != end)
    {
        fx = *iter;
        if (fx->GetName() == sName)
        {
            delete fx;
            m_Composers.Erase(iter);
            break;
        }

        ++iter;
    }
}

void FXComposerManager::DestroyComposer(FXComposer * fx)
{
    FXComposer * fx1;
    Vector<FXComposer*>::Iterator iter;
    Vector<FXComposer*>::Iterator end;
    iter = m_Composers.Begin();
    end = m_Composers.End();

    while (iter != end)
    {
        fx1 = *iter;
        if (fx1 == fx)
        {
            delete fx1;
            m_Composers.Erase(iter);
            break;
        }

        ++iter;
    }
}

void FXComposerManager::DestroyAll()
{
    FXComposer * fx1;
    Vector<FXComposer*>::Iterator iter;
    Vector<FXComposer*>::Iterator end;
    iter = m_Composers.Begin();
    end = m_Composers.End();

    while (iter != end)
    {
        fx1 = *iter;
        delete fx1;
        ++iter;
    }

    m_Composers.Clear();
}

Renderer * FXComposerManager::GetRectObj()
{
    return m_RectObj;
}