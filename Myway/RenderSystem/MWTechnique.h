#pragma once

#include "MWPass.h"

namespace Myway
{

/* class: Technique
----------------------------------------------------------

----------------------------------------------------------
*/

class MW_ENTRY Technique : public AllocObj
{
public:
    Technique(const String & name);
    ~Technique();

    Pass *          CreatePass();
    Pass *          GetPass(int index);
    void            DestroyPass(int index);
    void            DestroyAllPass();
    int          GetPassCount() const;
    const String &  GetName() const;

    void            SetForceSolid(bool solid);
    bool            GetForceSolid() const;
    bool            IsTransparency() const;

    void            SetSchemeName(const String & scheme);
    const String &  GetSchemeName() const;

    //render group
    void            SetRenderGroup(int id);
    int          GetRenderGroup() const;


    void            Clone(Technique * tech);

    explicit        Technique(const Technique & tech);
    Technique &     operator =(const Technique & tech);

public:
    String          m_name;
    String          m_scheme;
    bool            m_forceSolid;
    Vector<Pass*>   m_passes;
    int          m_group;
};

#include "MWTechnique.inl"

}