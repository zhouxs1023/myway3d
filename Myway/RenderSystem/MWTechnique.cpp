#include "MWTechnique.h"

using namespace Myway;


void Technique::DestroyAllPass()
{
    Vector<Pass*>::Iterator iter;
    Vector<Pass*>::Iterator end;

    iter = m_passes.Begin();
    end = m_passes.End();

    while (iter != end)
    {
        delete *iter;
        ++iter;
    }

    m_passes.Clear();
}

void Technique::Clone(Technique * tech)
{
    DestroyAllPass();

    m_name = tech->m_name;
    m_scheme = tech->m_scheme;
    m_forceSolid = tech->m_forceSolid;
    m_group = tech->m_group;

    Vector<Pass*>::Iterator iter;
    Vector<Pass*>::Iterator end;
    Pass * pass;

    iter = tech->m_passes.Begin();
    end = tech->m_passes.End();

    while (iter != end)
    {
        pass = new Pass();
        pass->Clone(*iter);
        m_passes.PushBack(pass);

        ++iter;
    }
}

Technique::Technique(const Technique & tech)
{
    assert(0 && "Technique(const Technique &) is invalid.");
}

Technique & Technique::operator =(const Technique & tech)
{
    assert(0 && "Technique::operator =(const Technique &) is invalid.");
    return *this;
}