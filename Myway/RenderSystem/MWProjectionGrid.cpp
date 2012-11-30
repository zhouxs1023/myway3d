#include "MWProjectionGrid.h"
#include "MWEnvironment.h"
#include "MWKeyboard.h"
#include "MWWaterManager.h"

namespace Myway {

    #define _def_MaxFarClipDistance 99999

    ProjectedGrid::ProjectedGrid()
        : mVertices(0)
		, mVerticesChoppyBuffer(0)
		, mNormal(0, 1, 0)
		, mPos(0, 0, 0)
	{
        _init();
        _initGeo();

		SetHeight(0);
	}

	ProjectedGrid::ProjectedGrid(const Options &Options)
        : mVertices(0)
		, mVerticesChoppyBuffer(0)
		, mNormal(0, 1, 0)
		, mPos(0, 0, 0)
	{
        _init();

		SetHeight(0);
	}

	ProjectedGrid::~ProjectedGrid()
	{
        _deinit();
	}

    void ProjectedGrid::_init()
    {
        mNoise = new Perlin();

        mVertices = new Vertex[mOptions.Complexity*mOptions.Complexity];	

        Vertex* Vertices = static_cast<Vertex*>(mVertices);

        for (int i = 0; i < mOptions.Complexity*mOptions.Complexity; i++)
        {
            Vertices[i].nx = 0;
            Vertices[i].ny = -1;
            Vertices[i].nz = 0;
        }

        mVerticesChoppyBuffer = new Vertex[mOptions.Complexity*mOptions.Complexity];

        mRenderCamera = World::Instance()->MainCamera();
    }

    void ProjectedGrid::_initGeo()
    {
        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        int iVertexCount = mOptions.Complexity * mOptions.Complexity;
        int iIndexCount = 6 * (mOptions.Complexity - 1) * (mOptions.Complexity - 1);
        int iPrimCount = iIndexCount / 3;
        int iStride = 24;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * iStride, USAGE_STATIC);

        vxStream->Bind(0, vb, iStride);
        vxStream->SetCount(iVertexCount);

        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));

        short * indexbuffer = (short *)ib->Lock(0, 0, LOCK_NORMAL);

        {
            int i = 0;
            int Complexity = mOptions.Complexity;
            for(int v=0; v<Complexity-1; v++){
                for(int u=0; u<Complexity-1; u++){
                    // face 1 |/
                    indexbuffer[i++] = v*Complexity + u;
                    indexbuffer[i++] = v*Complexity + u + 1;
                    indexbuffer[i++] = (v+1)*Complexity + u;

                    // face 2 /|
                    indexbuffer[i++] = (v+1)*Complexity + u;
                    indexbuffer[i++] = v*Complexity + u + 1;
                    indexbuffer[i++] = (v+1)*Complexity + u + 1;
                }
            }

        }

        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_OPATICY;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.fillMode = FILL_SOLID;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
        mRender.rState.fillMode = FILL_SOLID;
    }

    void ProjectedGrid::_deinit()
    {
        delete[] mVertices;
        delete[] mVerticesChoppyBuffer;

        delete mNoise;
    }

	void ProjectedGrid::SetHeight(float height)
	{
		mPos.y = height;

		mPlane = Plane(mNormal,  mPos);
	}

    void ProjectedGrid::Update(float elapsedTime)
    {
        //if (IKeyboard::Instance()->KeyPressed(KC_1))
        //    mRender.rState.fillMode = FILL_FRAME;
        //else if (IKeyboard::Instance()->KeyPressed(KC_2))
        //    mRender.rState.fillMode = FILL_SOLID;

        mNoise->update(elapsedTime);

        Camera * cam = World::Instance()->MainCamera();

        Vec3 RenderCameraPos = cam->GetPosition();

        float RenderingFarClipDistance = World::Instance()->MainCamera()->GetFarClip();

        Mat4 matRange;

        if (_getMinMax(matRange))
        {
            //
            _renderGeometry(matRange, RenderCameraPos);

            VertexBufferPtr vb = mRender.vxStream.GetStream(0);

            Vertex * vx = (Vertex *)vb->Lock(0, 0, LOCK_DISCARD);
            memcpy(vx, mVertices, mOptions.Complexity * mOptions.Complexity * sizeof(Vertex));
            vb->Unlock();
        }
    }

    bool ProjectedGrid::_getMinMax(Mat4 & range)
    {
        _setDisplacementAmplitude(mOptions.Strength);

        float x_min,y_min,x_max,y_max;
	    Vec3 frustum[8],proj_points[24];		// frustum to check the camera against

	    int n_points=0;
        int cube[] = {
                        0,1, 0,2, 2,3, 1,3,
                        0,4, 2,6, 3,7, 1,5,
                        4,6, 4,5, 5,7, 6,7
                     };	// which frustum points are connected together?

	    // transform frustum points to worldspace (should be done to the rendering_camera because it's the interesting one)
        Mat4 matInvVP;
        Math::MatInverse(matInvVP, mRenderCamera->GetViewProjMatrix());

        Math::VecTransform(frustum[0], Vec3(-1,-1, 0), matInvVP);
	    Math::VecTransform(frustum[1], Vec3(+1,-1, 0), matInvVP);
	    Math::VecTransform(frustum[2], Vec3(-1,+1, 0), matInvVP);
	    Math::VecTransform(frustum[3], Vec3(+1,+1, 0), matInvVP);
	    Math::VecTransform(frustum[4], Vec3(-1,-1,+1), matInvVP);
	    Math::VecTransform(frustum[5], Vec3(+1,-1,+1), matInvVP);
	    Math::VecTransform(frustum[6], Vec3(-1,+1,+1), matInvVP);
	    Math::VecTransform(frustum[7], Vec3(+1,+1,+1), matInvVP);	

	    // check intersections with upper_bound and lower_bound	
	    for(int i = 0; i < 12; i++)
        {
		    int src=cube[i * 2], dst=cube[i * 2 + 1];

            Myway::RayIntersectionInfo info;

            Ray ray(frustum[src], frustum[dst] - frustum[src]);
            float dist = Math::VecDistance(frustum[src], frustum[dst]);

            Math::RayIntersection(info, ray, mUpperBound);
            if (info.iterscetion && info.distance < (dist + DEFAULT_EPSILON))
            {			
			    proj_points[n_points++] = frustum[src] + info.distance * ray.direction;	
		    }

            Math::RayIntersection(info, ray, mLowerBound);
            if (info.iterscetion && info.distance < (dist + DEFAULT_EPSILON))
            {			
                proj_points[n_points++] = frustum[src] + info.distance * ray.direction;	
            }
	    }

	    // check if any of the frustums vertices lie between the upper_bound and lower_bound planes
	    {
		    for(int i = 0; i < 8; i++)
            {
                float d0 = Math::PlaneDistance(mUpperBound, frustum[i]);
                float d1 = Math::PlaneDistance(mLowerBound, frustum[i]);

                if ((d0 / d1) < 0)
                {			
				    proj_points[n_points++] = frustum[i];
			    }		
		    }
	    }


	    {
		    for(int i=0; i<n_points; i++){
			    // project the point onto the surface plane
                proj_points[i] = proj_points[i] - mNormal * mPlane.Distance(proj_points[i]);	
		    }
	    }

	    {
		    for(int i=0; i<n_points; i++){
                Math::VecTransform(proj_points[i], proj_points[i], mRenderCamera->GetViewMatrix());	 
                Math::VecTransform(proj_points[i], proj_points[i], mRenderCamera->GetProjMatrix());	 
		    }
	    }

	    // get max/min x & y-values to determine how big the "projection window" must be
	    if (n_points > 0){
		    x_min = proj_points[0].x;
		    x_max = proj_points[0].x;
		    y_min = proj_points[0].y;
		    y_max = proj_points[0].y;
		    for(int i=1; i<n_points; i++){
			    if (proj_points[i].x > x_max) x_max = proj_points[i].x;
			    if (proj_points[i].x < x_min) x_min = proj_points[i].x;
			    if (proj_points[i].y > y_max) y_max = proj_points[i].y;
			    if (proj_points[i].y < y_min) y_min = proj_points[i].y;
		    }		
    		

		    // build the packing matrix that spreads the grid across the "projection window"
		    Mat4 pack(x_max-x_min,	0,				0,		0,
                      0,			y_max-y_min,	0,		0,
                      0,			0,				1,		0,	
                      x_min,		y_min,			0,		1);

            Math::MatInverse(matInvVP, mRenderCamera->GetViewProjMatrix());
		    range = pack * matInvVP;

		    return true;
	    }

	    return false;
    }

    void ProjectedGrid::_setDisplacementAmplitude(float ampl)
    {
        mUpperBound = Plane(mNormal, mPos + ampl * mPlane.n);
        mLowerBound = Plane(mNormal, mPos - ampl * mPlane.n);
    }

    Vec4 ProjectedGrid::_calculeWorldPosition(const Vec2 & uv, const Mat4& m)
    {
        Mat4 _viewMat = World::Instance()->MainCamera()->GetViewMatrix();

        Vec4 origin(uv.x,uv.y,0,1);
        Vec4 direction(uv.x,uv.y,1,1);

        origin = origin * m;
        direction = direction * m;

        Vec3 _org(origin.x/origin.w,origin.y/origin.w,origin.z/origin.w);
        Vec3 _dir(direction.x/direction.w,direction.y/direction.w,direction.z/direction.w);
        _dir -= _org;
        _dir.NormalizeL();

        Ray _ray(_org,_dir);
        RayIntersectionInfo _result = _ray.Intersection(mPlane);
        float l = _result.distance;
        Vec3 worldPos = _org + _dir*l;
        Vec4 _tempVec = Vec4(worldPos, 1) * _viewMat;
        float _temp = -_tempVec.z/_tempVec.w;
        Vec4 retPos(worldPos, 1);
        retPos /= _temp;

        return retPos;
    }

    void ProjectedGrid::_renderGeometry(const Mat4& m, const Vec3& WorldPos)
    {
        Vec4 t_corners[4];

        t_corners[0] = _calculeWorldPosition(Vec2(0, 0), m);
        t_corners[1] = _calculeWorldPosition(Vec2(1, 0), m);
        t_corners[2] = _calculeWorldPosition(Vec2(0, 1), m);
        t_corners[3] = _calculeWorldPosition(Vec2(1, 1), m);


        float du  = 1.0f/(mOptions.Complexity-1),
            dv  = 1.0f/(mOptions.Complexity-1),
            u,v = 0.0f,
            // _1_u = (1.0f-u)
            _1_u, _1_v = 1.0f,
            divide, noise;

        float x, z, w;

        int i = 0, iv, iu;

        Vertex * Vertices = static_cast<Vertex*>(mVertices);

        for(iv=0; iv<mOptions.Complexity; iv++)
        {
            u = 0.0f;	
            _1_u = 1.0f;
            for(iu=0; iu<mOptions.Complexity; iu++)
            {				
                x = _1_v*(_1_u*t_corners[0].x + u*t_corners[1].x) + v*(_1_u*t_corners[2].x + u*t_corners[3].x);				
                z = _1_v*(_1_u*t_corners[0].z + u*t_corners[1].z) + v*(_1_u*t_corners[2].z + u*t_corners[3].z);
                w = _1_v*(_1_u*t_corners[0].w + u*t_corners[1].w) + v*(_1_u*t_corners[2].w + u*t_corners[3].w);				

                divide = 1 / w;				
                x *= divide;
                z *= divide;

                Vertices[i].x = x;
                Vertices[i].z = z;

                noise = mNoise->getValue(x * 5, z * 5);

                Vertices[i].y = -mPlane.d + noise*mOptions.Strength * 1.2f;

                i++;
                u += du;
                _1_u = 1.0f-u;
            }
            v += dv;
            _1_v = 1.0f-v;
        }

		/*if (mOptions.ChoppyWaves)
		{
			for(int i = 0; i < mOptions.Complexity*mOptions.Complexity; i++)
			{
				mVerticesChoppyBuffer[i] = Vertices[i];
			}
		}*/

		_calculeNormals();

		//_performChoppyWaves();
    }

    void ProjectedGrid::_calculeNormals()
    {
        int v, u;
        Vec3 vec1, vec2, normal;

        Vertex * Vertices = static_cast<Vertex*>(mVertices);

        for (int i = 0; i < mOptions.Complexity * mOptions.Complexity; ++i)
        {
            Vertices[i].nx = 0;
            Vertices[i].ny = 0;
            Vertices[i].nz = 0;
        }

        for(v=1; v<(mOptions.Complexity-1); v++)
        {
            for(u=1; u<(mOptions.Complexity-1); u++)
            {
                vec1 = Vec3(
                    Vertices[v*mOptions.Complexity + u + 1].x-Vertices[v*mOptions.Complexity + u - 1].x,
                    Vertices[v*mOptions.Complexity + u + 1].y-Vertices[v*mOptions.Complexity + u - 1].y, 
                    Vertices[v*mOptions.Complexity + u + 1].z-Vertices[v*mOptions.Complexity + u - 1].z);

                vec2 = Vec3(
                    Vertices[(v+1)*mOptions.Complexity + u].x - Vertices[(v-1)*mOptions.Complexity + u].x,
                    Vertices[(v+1)*mOptions.Complexity + u].y - Vertices[(v-1)*mOptions.Complexity + u].y,
                    Vertices[(v+1)*mOptions.Complexity + u].z - Vertices[(v-1)*mOptions.Complexity + u].z);

                normal = vec2.Cross(vec1);

                Vertices[v*mOptions.Complexity + u].nx += normal.x;
                Vertices[v*mOptions.Complexity + u].ny += normal.y;
                Vertices[v*mOptions.Complexity + u].nz += normal.z;
            }
        }
    }

    void ProjectedGrid::_performChoppyWaves()
    {
        int v, u,
		    Underwater = 1;

        if (WaterManager::Instance()->IsUnderWater())
		{
			Underwater = -1;
		}

		float Dis1,  Dis2;//, 
		   // Dis1_, Dis2_;

		Vec3 CameraDir, Norm;
		Vec2 Dir, Perp, Norm2;

		CameraDir = mRenderCamera->GetDirection();
		Dir       = Vec2(CameraDir.x, CameraDir.z).Normalize();
		Perp      = Dir.Perpendicular();

		if (Dir.x < 0 ) Dir.x = -Dir.x;
		if (Dir.y < 0 ) Dir.y = -Dir.y;

		if (Perp.x < 0 ) Perp.x = -Perp.x;
		if (Perp.y < 0 ) Perp.y = -Perp.y;

		Vertex* Vertices = static_cast<Vertex*>(mVertices);

		for(v=1; v<(mOptions.Complexity-1); v++)
		{
			Dis1 =  (Vec2(mVerticesChoppyBuffer[v*mOptions.Complexity + 1].x,
					               mVerticesChoppyBuffer[v*mOptions.Complexity + 1].z) -
					 Vec2(mVerticesChoppyBuffer[(v+1)*mOptions.Complexity + 1].x,
				                   mVerticesChoppyBuffer[(v+1)*mOptions.Complexity + 1].z)).Length();

			for(u=1; u<(mOptions.Complexity-1); u++)
			{   
				Dis2 = (Vec2(mVerticesChoppyBuffer[v*mOptions.Complexity + u].x,
					                  mVerticesChoppyBuffer[v*mOptions.Complexity + u].z) -
					    Vec2(mVerticesChoppyBuffer[v*mOptions.Complexity + u+1].x,
					                  mVerticesChoppyBuffer[v*mOptions.Complexity + u+1].z)).Length();

				Norm = Vec3(Vertices[v*mOptions.Complexity + u].nx,
					                 Vertices[v*mOptions.Complexity + u].ny,
								     Vertices[v*mOptions.Complexity + u].nz).Normalize();

				Norm2 = Vec2(Norm.x, Norm.z)  * 
					                 ( (Dir  * Dis1)   +
					                   (Perp * Dis2))  *
				 				      mOptions.ChoppyStrength;

				Vertices[v*mOptions.Complexity + u].x = mVerticesChoppyBuffer[v*mOptions.Complexity + u].x + Norm2.x * Underwater;
				Vertices[v*mOptions.Complexity + u].z = mVerticesChoppyBuffer[v*mOptions.Complexity + u].z + Norm2.y * Underwater;
			}
		}
    }
}