#include "MWSceneRenderUniform.h"
#include "MWRenderQueue.h"
#include "MWSceneManager.h"
#include "MWRenderSystem.h"

using namespace Myway;

IMPLEMENT_SINGLETON(SceneRenderUniform);

SceneRenderUniform::SceneRenderUniform()
: m_camera(NULL)
{
    //intialize Render Target.

    INITIALIZE_SINGLETON();
}

SceneRenderUniform::~SceneRenderUniform()
{
}

//////////////////////////////////////////////////////////////////////////
//  Render Scene
//      Á÷³Ì:
//              pre - z
//              color
//              lighting
//              shadow
//              blend
//              projection
//              render transparency
//
//////////////////////////////////////////////////////////////////////////
void SceneRenderUniform::RenderScene(Camera * cam)
{
    profile_code();

    m_camera = cam;
    
    RenderToCBuffer();
    RenderProjection();

    RenderQueue & rq = RenderQueue::Instance();

    rq.SortTransparency(cam);
    rq.RenderTransparency(0);
}

void SceneRenderUniform::SetScheme(RenderUniformStage stage, const String & scheme)
{
    switch(stage)
    {
    case RUS_DIFFUSE:
        m_diffuse_scheme = scheme;
        break;

    case RUS_PROJECTION:
        mroj_scheme = scheme;
        break;
    }
}

const String & SceneRenderUniform::GetScheme(RenderUniformStage stage)
{
    switch(stage)
    {
    case RUS_DIFFUSE:
        return m_diffuse_scheme;

    case RUS_PROJECTION:
        return mroj_scheme;
    }

    assert (0);

    return m_diffuse_scheme;
}

void SceneRenderUniform::RenderToCBuffer()
{
    profile_code();

    RenderQueue & rq = RenderQueue::Instance();

    rq.SetScheme(m_diffuse_scheme);
    rq.SetDefault(MaterialPtr(NULL));
    rq.Clear();

    SceneManager::VisibleNodeVisitor v = SceneManager::Instance().GetVisibleSceneNodes();

    while (!v.Endof())
    {
        SceneNode * node = *v.Cursor();
        node->AddRenderQueue(&rq);
        AddRenderQueue(*v.Cursor());

        ++v;
    }

    rq.Render(0);
}


void SceneRenderUniform::RenderProjection()
{
    RenderSystem * render = RenderSystem::InstancePtr();

    SceneManager::ProjVisitor visitor = SceneManager::Instance().GetProjections();

    while (!visitor.Endof())
    {
        Projection * proj = *visitor.Cursor();

        if (proj->GetVisible())
        {
            MaterialPtr mtl = proj->GetMaterial();

            //update render queue
            RenderQueue & rq = RenderQueue::Instance();

            rq.SetScheme(mroj_scheme);
            rq.SetDefault(mtl);
            rq.Clear();

            SceneManager::VisibleNodeVisitor nv = SceneManager::Instance().GetVisibleSceneNodes();

            while (!nv.Endof())
            {
                SceneNode * node = *nv.Cursor();

                if (node->IsVisible())
                {
                    SceneNode::MoverVisitor v = node->GetGeometres();

                    while (!v.Endof())
                    {
                        Mover * geo = *v.Cursor();

                        if (geo->HasProjection(proj))
                            geo->AddRenderQueue(&rq);

                        ++v;
                    }
                 }

                ++nv;
            }

            //render render queue
            Technique * tech = mtl->GetTechniqueByScheme(mroj_scheme);
            if (tech)
                tech = mtl->GetTechnique(0);

            assert(tech);

            Pass * pass = tech->GetPass(0);
            assert(pass);

            int i;
            for (i = 0; i < pass->GetTextureStageCount(); ++i)
            {
                TexturePtr tex = pass->GetTextureStage(i)->GetTexture();
                assert (!tex.IsNull());
                render->SetTexture(i, tex.GetPointer());
            }

            render->SetProjectionMatrix(proj->GetProjectionMatrix());

            rq.Render(i);
        }

        ++visitor;
    }  
}


void SceneRenderUniform::AddRenderQueue(SceneNode * node)
{
    RenderQueue & rq = RenderQueue::Instance();

    SceneNode::MoverVisitor v = node->GetGeometres();

    while (!v.Endof())
    {
        Mover * geo = *v.Cursor();

        if (geo->GetVisible())
            geo->AddRenderQueue(&rq);

        ++v;
    }
}

void SceneRenderUniform::HDR_Init()
{
}

void SceneRenderUniform::HDR_DeInit()
{
}

void SceneRenderUniform::HDR_Render()
{
}

void SceneRenderUniform::HDR_Bloom()
{
}

void SceneRenderUniform::HDR_Lum()
{
}

void SceneRenderUniform::HDR_ToneMap()
{
}