#pragma once

#include "MWFXComposer.h"
#include "MWFXComposerRenderer.h"

namespace Myway
{

class MW_ENTRY FXComposerManager : public Singleton<FXComposerManager>
{
public:
    FXComposerManager();
    ~FXComposerManager();

    FXComposer *    CreateComposer(const String & sName);
    FXComposer *    CreateComposerFromFile(const String & sName, const String & sFileName);

    FXComposer *    FindByName(const String & sName);

    void            Active(const String & sName);
    void            Active(FXComposer * fx);
    FXComposer *    GetActiveComposer();

    void            DestroyComposer(const String & sName);
    void            DestroyComposer(FXComposer * fx);
    void            DestroyAll();

    Renderer *     GetRectObj();

protected:
    Vector<FXComposer*>     m_Composers;
    FXComposer *            mActiveFX;
    FXComposerRectObj *     m_RectObj;
};

}