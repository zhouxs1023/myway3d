

/* :) inline function
--------------------------------------------------------------
--------------------------------------------------------------
*/

/*
    AnimationState
*/
inline AnimationState::AnimationState()
: mEnable(false),
mLoop(true),
mWeight(1.0f),
mPos(0.0f),
mAnim(NULL),
mParent(NULL)
{
}

inline AnimationState::~AnimationState()
{
}

inline void AnimationState::SetEnable(bool enable)
{
    if (mEnable != enable)
    {
        mEnable = enable;
        mParent->_NotifyActive(this, enable);
    }
}

inline void AnimationState::SetLoop(bool loop)
{
    mLoop = loop;
}

inline void AnimationState::SetWeight(float weight)
{
    mWeight = weight;
}

inline void AnimationState::AddTime(float time)
{
    mPos += time;
}

inline void AnimationState::SetPosition(float pos)
{
    mPos = pos;
}

inline const TString128 & AnimationState::GetName() const
{
    d_assert(mAnim);

    return mAnim->GetName();
}

inline bool AnimationState::GetEnable() const
{
    return mEnable;
}

inline bool AnimationState::GetLoop() const
{
    return mEnable;
}

inline float AnimationState::GetWeight() const
{
    return mWeight;
}

inline float AnimationState::GetPosition() const
{
    return mPos;
}

inline float AnimationState::GetLength() const
{
    return mAnim->GetLength();
}


inline Animation * AnimationState::GetAinmation()
{
    return mAnim;
}

/*
    AnimationSet
*/
inline AnimationSet::AnimationSet()
{
}

inline AnimationSet::~AnimationSet()
{
}

inline AnimationState * AnimationSet::CreateState(Animation * anim)
{
    d_assert(HasState(anim->GetName()) == NULL);

    AnimationState * state = mStates.Increase();
    state->mParent = this;
    state->mAnim = anim;
    return state;
}

inline AnimationState * AnimationSet::GetState(const TString128 & name)
{
    StateVisitor v = GetStates();

    while (!v.Endof() && v.Cursor()->GetName() != name)
    {
        ++v;
    }

    return !v.Endof() ? &(*v.Cursor()) : NULL;
}

inline bool AnimationSet::HasState(const TString128 & name)
{
    StateVisitor v = GetStates();

    while (!v.Endof() && v.Cursor()->GetName() != name)
    {
        ++v;
    }

    return !v.Endof();
}

inline int AnimationSet::GetActiveStateCount() const
{
    return mActives.Size();
}

inline void AnimationSet::_NotifyActive(AnimationState * anim, bool enable)
{
    if (enable)
    {
        mActives.PushBack(anim);
    }
    else 
    {
        _RemoveActive(anim);
    }
}

inline void AnimationSet::_RemoveActive(AnimationState * anim)
{
    ActiveStateVisitor v = GetActiveStates();

    while (!v.Endof() && (*v.Cursor()) != anim)
    {
        ++v;
    }

    d_assert (!v.Endof());

    mActives.Erase(v.Cursor());
}

inline AnimationSet::StateVisitor AnimationSet::GetStates()
{
    return StateVisitor(mStates.Begin(), mStates.End());
}

inline AnimationSet::ActiveStateVisitor AnimationSet::GetActiveStates()
{
    return ActiveStateVisitor(mActives.Begin(), mActives.End());
}