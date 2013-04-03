#include "MWWorld.h"
#include "MWMeshManager.h"
#include "MWSceneOctree.h"
#include "Engine.h"
#include "MWRenderEvent.h"

#pragma warning(disable : 4291)

using namespace Myway;

IMP_SLN(World);
World::World()
{
    INIT_SLN;

    mSceneManager = new OctreeScene(Aabb(-1024, -1024, -1024, 1024, 1024, 1024));

    const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

    mMainCamera = new Camera("Core_MainCamera");
    mMainCamera->SetAspect((float)dp->Width / (float)dp->Height);

    mMainCameraNode = CreateSceneNode("Core_MainCameraNode");
    mMainCameraNode->Attach(mMainCamera);
}

World::~World()
{
    safe_delete (mSceneManager);

    DestroyAllLight();
    DestroyAllEntity();
    DestroyAllCamera();

    safe_delete (mMainCamera);

    DestroyAllSceneNode();

    SHUT_SLN;
}

void World::Resize(int xSize, int ySize, int zSize)
{
    mSceneManager->Resize(Aabb(0, 0, 0, (float)xSize, (float)ySize, (float)zSize));
}

Camera * World::CreateCamera(const TString128 & name)
{
    debug_assert(!HasCamera(name), "camera exist.");

    Camera * cam = new Camera(name);

    mCameras.PushBack(cam);

    return cam;
}

Camera * World::GetCamera(const TString128 & name)
{
    List<Camera*>::Iterator iter;
    List<Camera*>::Iterator end;

    iter = mCameras.Begin();
    end = mCameras.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    return iter != end ? (*iter) : NULL;
}

bool World::HasCamera(const TString128 & name)
{
    List<Camera*>::Iterator iter;
    List<Camera*>::Iterator end;

    iter = mCameras.Begin();
    end = mCameras.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    return iter != end;
}

void World::DestroyCamera(const TString128 & name)
{
    List<Camera*>::Iterator iter;
    List<Camera*>::Iterator end;

    iter = mCameras.Begin();
    end = mCameras.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    debug_assert(iter != end, "camera not exist.");

    delete *iter;
    mCameras.Erase(iter);
}

void World::DestroyCamera(Camera * cam)
{
    List<Camera*>::Iterator iter;
    List<Camera*>::Iterator end;

    iter = mCameras.Begin();
    end = mCameras.End();

    while (iter != end && *iter != cam)
    {
        ++iter;
    }

    debug_assert(iter != end, "camera not exist.");

    delete *iter;
    mCameras.Erase(iter);
}

void World::DestroyAllCamera()
{
    List<Camera*>::Iterator iter;
    List<Camera*>::Iterator end;

    iter = mCameras.Begin();
    end = mCameras.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    mCameras.Clear();
}


Entity * World::CreateEntity(const TString128 & name, const TString128 & mesh)
{
    d_assert(!HasEntity(name) && "entity has been created.");

    Entity * pEntity = new Entity(name, MeshManager::Instance()->Load(mesh, mesh));

    mEntitys.PushBack(pEntity);

    return pEntity;
}

Entity * World::CreateEntity(const TString128 & name, MeshPtr mesh)
{
    d_assert(!HasEntity(name) && mesh.NotNull() && "entity has been created.");

    Entity * pEntity = new Entity(name,mesh);

    mEntitys.PushBack(pEntity);

    return pEntity;
}

Entity * World::CreateEntity(const TString128 & name)
{
    d_assert(!HasEntity(name) && "entity has been created.");

    Entity * pEntity = new Entity(name);

    mEntitys.PushBack(pEntity);

    return pEntity;
}

bool World::HasEntity(const TString128 & name)
{
    List<Entity*>::Iterator iter;
    List<Entity*>::Iterator end;

    iter = mEntitys.Begin();
    end = mEntitys.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    return iter != end;
}

bool World::RenameEntity(const TString128 & name, Entity * entity)
{
    if (HasEntity(name))
        return false;

    entity->SetName(name);

    return true;
}


Entity * World::GetEntity(const TString128 & name)
{
    List<Entity*>::Iterator iter;
    List<Entity*>::Iterator end;

    iter = mEntitys.Begin();
    end = mEntitys.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    return iter != end ? (*iter) : NULL;
}

void World::DestroyEntity(const TString128 & name)
{
    List<Entity*>::Iterator iter;
    List<Entity*>::Iterator end;

    iter = mEntitys.Begin();
    end = mEntitys.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    debug_assert(iter != end, "entity not exist.");

    delete *iter;
    mEntitys.Erase(iter);
}

void World::DestroyEntity(Entity * entity)
{
    List<Entity*>::Iterator iter;
    List<Entity*>::Iterator end;

    iter = mEntitys.Begin();
    end = mEntitys.End();

    while (iter != end && (*iter) != entity)
    {
        ++iter;
    }

    debug_assert(iter != end, "entity not exist.");

    delete *iter;
    mEntitys.Erase(iter);
}

void World::DestroyAllEntity()
{
    List<Entity*>::Iterator iter;
    List<Entity*>::Iterator end;

    iter = mEntitys.Begin();
    end = mEntitys.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    mEntitys.Clear();
}

Light * World::CreateLight(const TString128 & name)
{
    d_assert(!HasLight(name));

    Light * light = new Light(name);

    mLights.PushBack(light);

    return light;
}

bool World::HasLight(const TString128 & name)
{
    List<Light*>::Iterator iter;
    List<Light*>::Iterator end;

    iter = mLights.Begin();
    end = mLights.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    return iter != end;
}

bool World::RenameLight(const TString128 & name, Light * light)
{
	if (HasLight(name))
		return false;

	light->SetName(name);

	return true;
}

Light * World::GetLight(const TString128 & name)
{
    List<Light*>::Iterator iter;
    List<Light*>::Iterator end;

    iter = mLights.Begin();
    end = mLights.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    return iter != end ? *iter : NULL;
}

void World::DestroyLight(const TString128 & name)
{
    List<Light*>::Iterator iter;
    List<Light*>::Iterator end;

    iter = mLights.Begin();
    end = mLights.End();

    while (iter != end && (*iter)->GetName() != name)
    {
        ++iter;
    }

    d_assert(iter != end);

    delete *iter;
    mLights.Erase(iter);
}

void World::DestroyLight(Light * light)
{
    List<Light*>::Iterator iter;
    List<Light*>::Iterator end;

    iter = mLights.Begin();
    end = mLights.End();

    while (iter != end && (*iter) != light)
    {
        ++iter;
    }

    d_assert(iter != end);

    delete *iter;
    mLights.Erase(iter);
}

void World::DestroyAllLight()
{
    List<Light*>::Iterator iter;
    List<Light*>::Iterator end;

    iter = mLights.Begin();
    end = mLights.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    mLights.Clear();
}

void World::GetVisibleLights(Node * node, List<Light*> & lights)
{
    profile_code();

    lights.Clear();

    List<Light*>::Iterator iter;
    List<Light*>::Iterator end;

    iter = mLights.Begin();
    end = mLights.End();

    Light * light;

    while (iter != end)
    {
        light = *iter;

        if (light->IsVisible())
        {
            LIGHT_TYPE type = light->GetType();

            if (type == LT_DIRECTIONAL)
            {
                lights.PushBack(light);
            }

            else if (type == LT_POINT)
            {
                const Sphere & sph = node->GetWorldBoundingSphere();
                float len = Math::VecLength(light->GetPosition() - sph.center);
                if (len - sph.radius < light->GetRange())
                    lights.PushBack(light);
            }

            else if (type == LT_SPOT)
            {
                Aabb aabb;
                Vec3 ltf, lbf, rtf, rbf;
                Vec3 ltb, lbb, rtb, rbb;
                Vec3 lp;

                float sq1, sq2, sq3, sq4;
                float sq5, sq6, sq7, sq8;
                float rsq;

                Vec3 d1, d2, d3, d4;
                Vec3 d5, d6, d7, d8;
                Vec3 ld;
                float dt1, dt2, dt3, dt4;
                float dt5, dt6, dt7, dt8;
                float mdt;

                aabb = node->GetWorldAabb();

                ltf = aabb.GetLeftTopFrontPoint();
                lbf = aabb.GetLeftBottomFrontPoint();
                rtf = aabb.GetRightTopFrontPoint();
                rbf = aabb.GetRightBottomFrontPoint();

                ltb = aabb.GetLeftTopBackPoint();
                lbb = aabb.GetLeftBottomBackPoint();
                rtb = aabb.GetRightTopBackPoint();
                rbb = aabb.GetRightBottomBackPoint();

                lp = light->GetPosition();
                rsq = light->GetRange() * light->GetRange();

                sq1 = Math::VecDistanceSq(lp, ltf);
                sq2 = Math::VecDistanceSq(lp, lbf);
                sq3 = Math::VecDistanceSq(lp, rtf);
                sq4 = Math::VecDistanceSq(lp, rbf);
                sq5 = Math::VecDistanceSq(lp, ltb);
                sq6 = Math::VecDistanceSq(lp, lbb);
                sq7 = Math::VecDistanceSq(lp, rtb);
                sq8 = Math::VecDistanceSq(lp, rbb);

                if (sq1 < rsq || sq2 < rsq || sq3 < rsq || sq4 < rsq ||
                    sq5 < rsq || sq6 < rsq || sq7 < rsq || sq8 < rsq)
                {
                    ld = light->GetDirection();
                    mdt = light->GetOuter();

                    d1 = ltf - lp;
                    d2 = lbf - lp;
                    d3 = rtf - lp;
                    d4 = rbf - lp;
                    d5 = ltb - lp;
                    d6 = lbb - lp;
                    d7 = rtb - lp;
                    d8 = rbb - lp;

                    Math::VecNormalize(d1, d1);
                    Math::VecNormalize(d2, d2);
                    Math::VecNormalize(d3, d3);
                    Math::VecNormalize(d4, d4);
                    Math::VecNormalize(d5, d5);
                    Math::VecNormalize(d6, d6);
                    Math::VecNormalize(d7, d7);
                    Math::VecNormalize(d8, d8);

                    dt1 = Math::VecDot(ld, d1);
                    dt2 = Math::VecDot(ld, d2);
                    dt3 = Math::VecDot(ld, d3);
                    dt4 = Math::VecDot(ld, d4);
                    dt5 = Math::VecDot(ld, d5);
                    dt6 = Math::VecDot(ld, d6);
                    dt7 = Math::VecDot(ld, d7);
                    dt8 = Math::VecDot(ld, d8);

                    if (dt1 > mdt || dt2 > mdt || dt3 > mdt || dt4 > mdt ||
                        dt5 > mdt || dt6 > mdt || dt7 > mdt || dt7 > mdt)
                    {
                        lights.PushBack(light);
                    }
                }
            }
        }
        
        ++iter;
    }
}




void World::RayTracing(const Ray & ray, float dist, Array<Scene::TraceInfo> & nodes, int flags)
{
    mSceneManager->RayTracing(ray, dist, nodes, flags);
}


SceneNode * World::CreateSceneNode(const TString128 & name)
{
    d_assert (!GetSceneNode(name));

    SceneNode * node = new SceneNode(name);
    mSceneNodes.PushBack(node);

    mSceneManager->AddSceneNode(node);

    return node;
}

SceneNode * World::GetSceneNode(const TString128 & name)
{
    SceneNodeList::Iterator whr = mSceneNodes.Begin();
    SceneNodeList::Iterator end = mSceneNodes.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    return (whr != end) ? *whr : NULL;
}

SceneNode * World::CreateSceneNode()
{
    static int uniqueId = 0;
    TString128 name("__unknown_");
    name += uniqueId++;

    return CreateSceneNode(name);
}

void World::DestroySceneNode(SceneNode * sn)
{
    SceneNodeList::Iterator whr = mSceneNodes.Begin();
    SceneNodeList::Iterator end = mSceneNodes.End();

    while (whr != end && (*whr) != sn)
    {
        ++whr;
    }

    d_assert (whr != end);

    mSceneManager->RemoveSceneNode(*whr);

    delete *whr;

    mSceneNodes.Erase(whr);
}

void World::DestroyAllSceneNode()
{
    SceneNodeList::Iterator iter;
    SceneNodeList::Iterator end;

    iter = mSceneNodes.Begin();
    end = mSceneNodes.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    mSceneNodes.Clear();
}

void World::UpdateFrame()
{
    profile_code();

    RenderEvent::OnPreUpdateScene(NULL, NULL);

    UpdateScene();

    RenderEvent::OnPostUpdateScene(NULL, NULL);

    RenderSystem * render = RenderSystem::Instance();

    render->BeginScene();

    render->SetTime(Engine::Instance()->GetTime());

	RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

	d_assert (scheme != NULL);

	scheme->DoRender();

    render->EndScene();
}

void World::UpdateScene()
{
    profile_code();

    SceneNodeList::Iterator whr = mSceneNodes.Begin();
    SceneNodeList::Iterator end = mSceneNodes.End();

    while (whr != end)
    {
        SceneNode * node = *whr;
        if (node->_NeedUpdate())
            node->_UpdateTransform();

        ++whr;
    }
}

void World::CullNodes(VisibleCullResult & result, Camera * cam)
{
    mSceneManager->GetVisibleSceneNodes(result.nodes, cam);
}

void World::CullLights(VisibleCullResult & result, Camera * cam)
{
    List<Light*>::Iterator whr = mLights.Begin();
    List<Light*>::Iterator end = mLights.End();

    while (whr != end)
    {
        // current it's false
        Light * light = *whr;

		if (!light->IsVisible())
			continue;

		switch (light->GetType())
		{
		case LT_DIRECTIONAL:
			result.lights.PushBack(light);
			break;

		case LT_POINT:
		case LT_SPOT:
			{
				Sphere sph = Sphere(light->GetPosition(), light->GetRange());
				if (cam->IsVisible(sph))
					result.lights.PushBack(light);
			}
			break;
		}

        ++whr;
    }
}

void World::UpdateGeo(VisibleCullResult & result, Camera * cam)
{
    //notify update
    List<SceneNode *>::Iterator whr = result.nodes.Begin();
    List<SceneNode *>::Iterator end = result.nodes.End();

    while (whr != end)
    {
        SceneNode::MoverVisitor visitor = (*whr)->GetMovers();

        while (!visitor.Endof())
        {
            Mover * geo = *visitor.Cursor();
            geo->NotifyCamera(cam);
            ++visitor;
        }

        ++whr;
    }

    //update geometry
    whr = result.nodes.Begin();
    end = result.nodes.End();

    while (whr != end)
    {
        SceneNode::MoverVisitor visitor = (*whr)->GetMovers();

        while (!visitor.Endof())
        {
            Mover * geo = *visitor.Cursor();
            geo->UpdateGeometry();
            ++visitor;
        }

        ++whr;
    }
}

void World::ImpVisibleCull(VisibleCullResult & result, Camera * cam, bool updateGeo, bool cullLights)
{
    profile_code();

    RenderEvent::OnPreVisibleCull(cam, NULL);

    CullNodes(result, cam);

	if (cullLights)
		CullLights(result, cam);

    if (updateGeo)
        UpdateGeo(result, cam);

    RenderEvent::OnPostVisibleCull(cam, NULL);
}
