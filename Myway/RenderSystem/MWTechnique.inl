
inline Technique::Technique(const String & name)
: m_name(name),
  m_forceSolid(FALSE),
  m_group(DEFAULT_RENDER_GROUP)
{
}

inline Technique::~Technique()
{
    DestroyAllPass();
}

inline Pass * Technique::CreatePass()
{
    Pass * pass = new Pass();
    m_passes.PushBack(pass);
    return pass;
}

inline Pass * Technique::GetPass(int index)
{
    debug_assert(index >= 0 && index < m_passes.Size(), "Invalid Parameter");
    Vector<Pass*>::Iterator iter;
    iter = m_passes.Begin() + index;
    return *iter;
}

inline void Technique::DestroyPass(int index)
{
    debug_assert(index >= 0 && index < m_passes.Size(), "Invalid Parameter");

    Vector<Pass*>::Iterator iter;
    iter = m_passes.Begin() + index;
    delete *iter;
    m_passes.Erase(iter);
}

inline int Technique::GetPassCount() const
{
    return m_passes.Size();
}

inline const String & Technique::GetName() const
{
    return m_name;
}

inline void Technique::SetSchemeName(const String & scheme)
{
    m_scheme = scheme;
}

inline const String & Technique::GetSchemeName() const
{
    return m_scheme;
}

inline void Technique::SetForceSolid(bool solid)
{
    m_forceSolid = solid;
}

inline bool Technique::GetForceSolid() const
{
    return m_forceSolid;
}

inline bool Technique::IsTransparency() const
{
    return !m_forceSolid && m_passes[0]->IsTransparency();
}

inline void Technique::SetRenderGroup(int id)
{
    d_assert (id < MAX_RENDER_GROUP);
    m_group = id;
}

inline int Technique::GetRenderGroup() const
{
    return m_group;
}

