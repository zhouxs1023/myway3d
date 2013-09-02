#pragma once

#include "MWEntity.h"
#include "MWBillboard.h"
#include "MWCamera.h"
#include "MWSceneNode.h"
#include "MWLight.h"
#include "MWScene.h"

namespace Myway
{

class Environment;

struct VisibleCullResult
{
    List<Light*> lights;
    List<SceneNode*> nodes;
};

class MW_ENTRY World
{
    typedef HashMap<TString128, SceneNode *, TString128::hash_t> SceneNodeMap;

    DECLARE_ALLOC();
    DECLARE_SINGLETON(World);

public:
                        World();
                        ~World();

    void                Resize(int xSize, int ySize, int zSize);

    //camera
    Camera *            CreateCamera(const TString128 & name);
    Camera *            GetCamera(const TString128 & name);
    bool                HasCamera(const TString128 & name);
    void                DestroyCamera(const TString128 & name);
    void                DestroyCamera(Camera * cam);
    void                DestroyAllCamera();
    Camera *            MainCamera() { return mMainCamera; }
    SceneNode *         MainCameraNode() { return mMainCameraNode; }

    //entity
    Entity *            CreateEntity(const TString128 & name, const TString128 & mesh);
    Entity *            CreateEntity(const TString128 & name, MeshPtr mesh);
    Entity *            CreateEntity(const TString128 & name);
    bool                HasEntity(const TString128 & name);
    Entity *            GetEntity(const TString128 & name);
    void                DestroyEntity(const TString128 & name);
    void                DestroyEntity(Entity * entity);
    void                DestroyAllEntity();

    //light
    Light *             CreateLight(const TString128 & name);
    bool                HasLight(const TString128 & name);
	bool				RenameLight(const TString128 & name, Light * light);
    Light *             GetLight(const TString128 & name);
    void                DestroyLight(const TString128 & name);
    void                DestroyLight(Light * light);
    void                DestroyAllLight();

    void                GetVisibleLights(Node * node, List<Light*> & lights);

    //node
    SceneNode *         CreateSceneNode();
    SceneNode *         CreateSceneNode(const TString128 & name);
    SceneNode *         GetSceneNode(const TString128 & name);
    void                DestroySceneNode(SceneNode * sn);
    void                DestroyAllSceneNode();
    const List<SceneNode *> & GetSceneNodes() { return mSceneNodes; }

    //ray trace
    void                RayTracing(const Ray & ray, float dist, Array<Scene::TraceInfo> & nodes, int flags);

    void                UpdateFrame();

    void                ImpVisibleCull(VisibleCullResult & result, Camera * cam, bool updateGeo, bool cullLights = false);

protected:
    void                UpdateScene();

    void                CullNodes(VisibleCullResult & result, Camera * cam);
    void                CullLights(VisibleCullResult & result, Camera * cam);

    void                UpdateGeo(VisibleCullResult & result, Camera * cam);

protected:
    List<SceneNode *>   mSceneNodes;

    List<Entity*>       mEntitys;

    List<Camera*>       mCameras;

    Camera *            mMainCamera;
    SceneNode *         mMainCameraNode;

    List<Light*>        mLights;
    Scene *             mSceneManager;
};

}