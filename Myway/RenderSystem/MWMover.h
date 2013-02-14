 #pragma once

#include "MWRenderDefine.h"


namespace Myway
{

class RenderQueue;
class Camera;
class Node;
class Light;

class MW_ENTRY Mover
{
    DECLARE_ALLOC();

public:
                                    Mover(const TString128 & name);
    virtual                         ~Mover();
            
            void                    SetName(const TString128 & name);
            const TString128 &      GetName() const;

            Vec3                    GetPosition();
            Quat                    GetOrientation();
            Vec3                    GetDirection();
            Vec3                    GetUp();
            Vec3                    GetRight();

    virtual void                    NotifyCamera(Camera * cam);
    virtual void                    UpdateGeometry();
    virtual void                    AddRenderQueue(RenderQueue * rq);
        
    virtual void                    NotifyAttach(Node * node);
    virtual void                    NotifyDetach();

    virtual void                    SetBounds(const Aabb & aabb, const Sphere & sph);        

    virtual const Aabb &            GetLocalAabb() const;
    virtual const Sphere &          GetLocalSphere() const;

    virtual const Aabb &            GetWorldAabb() const;
    virtual const Sphere &          GetWorldSphere() const;

    virtual void                    NotifyUpdate();

            Node *                  GetAttachNode();
            bool                    IsAttached() const;

            BFlag32 &               GetFlag();

            void                    SetVisible(bool visible);
            bool                    IsVisible() const;

            void                    SetCastShadow(bool cast);
            bool                    IsCastShadow() const;

protected:
    TString128                      mName;
    Node *                          mNode;
    BFlag32                         mFlag;

    Aabb                            mAabbLocal;
    Aabb                            mAabbWorld;
    Sphere                          mSphLocal;
    Sphere                          mSphWorld;

    bool                            mVisible;

    bool                            mCastShadow;
};

}