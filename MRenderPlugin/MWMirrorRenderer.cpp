#include "MWMirrorRenderer.h"
#include "MWDeferredRenderer.h"
#include "MWRenderEvent.h"

namespace Myway {

	MirrorRenderLoop::MirrorRenderLoop(DeferredRenderer * sch)
		: mScheme(sch)
	{
	}

	MirrorRenderLoop::~MirrorRenderLoop()
	{
	}

	void MirrorRenderLoop::DoRender(Camera * cam)
	{
		RenderSystem * render = RenderSystem::Instance();
		RenderQueue * rq = mScheme->GetMirrorRenderQueue();

		 // --->render terrain
        if (Environment::Instance()->GetTerrain())
            Environment::Instance()->GetTerrain()->RenderInMirror();

        // ---> render sky
        if (Environment::Instance()->GetSky2())
            Environment::Instance()->GetSky2()->RenderReflection(cam->GetMirrorPlane());

        // ---> render sun
        /*if (Environment::Instance()->GetSun())
            Environment::Instance()->GetSun()->Render();*/

        // ---> render moons
        if (Environment::Instance()->GetMoon())
            Environment::Instance()->GetMoon()->Render();

        // ---> render cloud
        //if (Environment::Instance()->GetCloud())
            //Environment::Instance()->GetCloud()->Render(false);

		// render solider
		{
			RenderSystem * render = RenderSystem::Instance();

			const Array<Renderer *> & objs = rq->GetSolidRender();

			for (int i = 0; i < objs.Size(); ++i)
			{
				Renderer * rd = objs[i];

				bool skined = (rd->GetBlendMatrix(NULL) > 0);
				Technique * tech = rd->GetTechnique(eRenderTechType::RTT_Mirror);

				if (!tech)
					tech = mScheme->GetMainShaderProvider()->GetTechnique(eRenderTechType::RTT_Mirror, skined);

				render->Render(tech, rd);
			}
		}

		RenderEvent::OnMirrorRenderSolid1(cam);
		RenderEvent::OnMirrorRenderSolid2(cam);
		RenderEvent::OnMirrorRenderSolid3(cam);
	}


}