#include "stdafx.h"
#include "xMesh.h"

DF_PROPERTY_BEGIN(xMesh)
	DF_PROPERTY(xMesh, MeshFile, "General", "File", PT_TString, 128)
    DF_PROPERTY(xMesh, AnimName, "General", "Animation", PT_TString, 128)
	DF_PROPERTY(xMesh, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xMesh, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xMesh, Scale, "Transform", "Scale", PT_Float, 4)
DF_PROPERTY_END();

xMesh::xMesh(const TString128 & name)
    : xObj(name)
	, OnUpdate(xEvent::OnUpdate, this, &xMesh::_Update)
	, OnRenderSkel(RenderEvent::OnAfterDefferedShading, this, &xMesh::_renderSkel)
{
	MeshFile = "";
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = 1;

	mAnimState = NULL;

	mNode = World::Instance()->CreateSceneNode();
	mEntity = World::Instance()->CreateEntity(name);
	mNode->Attach(mEntity);

	mNode->GetFlag().SetFlags(PICK_Flag);

	mEntity->SetBounds(xObj::K_DefaultBound, xObj::K_DefaultSphere);
}

xMesh::~xMesh()
{
    if (mEntity)
        World::Instance()->DestroyEntity(mEntity);

    if (mNode)
        World::Instance()->DestroySceneNode(mNode);
}

Aabb xMesh::GetBound()
{
	return mNode->GetWorldAabb();
}

void xMesh::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 1;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << MeshFile;
		Serializer << Position;
		Serializer << Orientation;
		Serializer << Scale;
	}
	else
	{
		Serializer >> version;
		if (version == 0)
		{
			Serializer >> MeshFile;
			Serializer >> Position;
			Serializer >> Orientation;

			Vec3 scale;
			Serializer >> scale;

			Scale = scale.x;
		}
		else if (version == 1)
		{
			Serializer >> MeshFile;
			Serializer >> Position;
			Serializer >> Orientation;
			Serializer >> Scale;
		}

		SetPosition(Position);
		SetOrientation(Orientation);
		SetScale(Scale);
		SetMeshFile(MeshFile);
	}
}

bool xMesh::OnPropertyChanged(const Property * p)
{
	if (p->name == "Name")
	{
		SetName(Name);
	}
	else if (p->name == "MeshFile")
	{
		SetMeshFile(MeshFile);
	}
	else if (p->name == "AnimName")
	{
		SetAnimName(AnimName);
	}
	else if (p->name == "Position")
	{
		SetPosition(Position);
	}
	else if (p->name == "Orientation")
	{
		SetOrientation(Orientation);
	}
	else if (p->name == "Scale")
	{
		SetScale(Scale);
	}

	return true;
}

void xMesh::SetName(const TString128 & name)
{
	Name = name;
    World::Instance()->RenameEntity(name, mEntity);
}

void xMesh::SetMeshFile(const TString128 & meshFile)
{
	MeshFile = meshFile;

    if (mEntity && mEntity->GetMesh() != NULL && mEntity->GetMesh()->GetSourceName() == meshFile)
        return ;

    mEntity->SetMesh(meshFile);
}

void xMesh::SetAnimName(const TString128 & animName)
{
	AnimName = animName;

	if (mAnimState && mAnimState->GetName() == animName)
		return ;

	if (mAnimState)
	{
		mAnimState->SetEnable(false);
		mAnimState->SetPosition(0);
		mAnimState = NULL;
	}

	mAnimState = mEntity->GetAnimationSet()->GetState(AnimName);

	if (mAnimState)
	{
		mAnimState->SetEnable(true);
		mAnimState->SetPosition(0);
		mAnimState->SetLoop(true);
	}
}

xObj * xMesh::Clone()
{
	xMesh * mesh = (xMesh *)xObjManager::Instance()->Create(GetTypeName().c_str());

	mesh->SetPosition(Position);
	mesh->SetOrientation(Orientation);
	mesh->SetScale(Scale);
	mesh->SetMeshFile(MeshFile);

	return mesh;
}

bool xMesh::IsSceneNode(SceneNode * node)
{
	return node == mNode;
}

void xMesh::SetPosition(const Vec3 & position)
{
	Position = position;
    mNode->SetPosition(position);
}

void xMesh::SetOrientation(const Quat & ort)
{
	Orientation = ort;
	mNode->SetOrientation(ort);
}

void xMesh::SetScale(float scale)
{
	Scale = scale;
    mNode->SetScale(scale);
}

void xMesh::_Update(Event * sender)
{
	float time = Engine::Instance()->GetFrameTime();

	if (mAnimState)
		mAnimState->AddTime(time);

	mEntity->UpdateAnimation();
}

void xMesh::_renderSkel(Event * sender)
{
	/*if (mEntity)
	{
		xSkeletonRenderer::Instance()->Render(mEntity);
		mEntity->SetVisible(false);
	}*/
}









xMeshFactoryListener gListener;


void xMeshFactoryListener::_OnDragFile(Event * sender)
{
	Point2f pt = *(Point2f*)sender->GetParam(0);
	TString128 ActorFile = (const char *)sender->GetParam(1);

	ActorFile.Lower();
	ActorFile.Replace('/', '\\');

	TString128 externName;

	externName = File::GetExternName(ActorFile);

	if (externName != "mesh")
		return ;

	const char * testFile = ActorFile.c_str();
	int length = ActorFile.Length();

	bool exist = false;

	while (length > 0)
	{
		if (testFile[length - 1] == '\\')
		{
			const char * tfile = testFile + length;
			exist = ResourceManager::Instance()->Exist(tfile);

			if (exist)
				break;
		}

		--length;
	}

	if (!exist)
		return ;

	testFile += length;

	xMesh * obj = (xMesh *)xObjManager::Instance()->Create("Mesh");

	d_assert (obj);

	Vec3 pos = xApp::Instance()->GetHitPosition(pt.x, pt.y);

	obj->SetPosition(pos);
	obj->SetMeshFile(testFile);

	xApp::Instance()->SetSelectedObj(obj);
}





xSkeletonRenderer gSkelRender;

IMP_SLN (xSkeletonRenderer);

xSkeletonRenderer::xSkeletonRenderer()
	: OnInit(xEvent::OnInit, this, &xSkeletonRenderer::_init)
	, OnShutdown(xEvent::OnShutdown, this, &xSkeletonRenderer::_shutdown)
{
	INIT_SLN;
}

xSkeletonRenderer::~xSkeletonRenderer()
{
	SHUT_SLN;
}

void _FillVertex(Bone * bone, Vec3 *& buff, int & prims)
{
	for (int i = 0; i < bone->GetNumChildren(); ++i)
	{
		Bone * child = bone->GetChild(i);

		Vec3 orig = bone->GetPosition();
		Vec3 target = child->GetPosition() * bone->GetWorldMatrix();

		if (bone->GetParent())
			orig *= bone->GetParent()->GetWorldMatrix();

		Vec3 origUp = bone->GetUp();
		Vec3 origDr = (target - orig).Normalize();

		if (fabs(origUp.Dot(origDr)) > 0.99f)
		{
			origUp = -bone->GetDirection();
		}

		Vec3 origRt = origUp.CrossN(origDr);
		origUp = origDr.CrossN(origRt);

		const float kw = 5;

		Vec3 v0 = orig - origRt * kw + origUp * kw;
		Vec3 v1 = orig + origRt * kw + origUp * kw;
		Vec3 v2 = orig + origRt * kw - origUp * kw;
		Vec3 v3 = orig - origRt * kw - origUp * kw;

		*buff++ = v0;
		*buff++ = v1;
		*buff++ = v2;
		*buff++ = v3;
		*buff++ = target;

		prims += 6;
	}

	for (int i = 0; i < bone->GetNumChildren(); ++i)
	{
		Bone * child = bone->GetChild(i);

		_FillVertex(child, buff, prims);
	}
}

void xSkeletonRenderer::Render(Entity * entity)
{
	SkeletonInstance * skel = entity->GetSkeletonInstance();

	if (!skel)
		return ;

	int iPrimCount = 0;
	VertexBufferPtr vb = mRenderOp->vxStream.GetStream(0);

	Vec3 * vbuff = (Vec3 *)vb->Lock(0, 0, 0);

	for (int i = 0; i < skel->GetRootBoneCount(); ++i)
	{
		Bone * bone = skel->GetRootBone(i);

		_FillVertex(bone, vbuff, iPrimCount);
	}

	skel->UpdateBoneMatrix();

	for (int i = 0; i < skel->GetBoneCount(); ++i)
	{
		Bone * bone = skel->GetBone(i);

		int j = 0;
	}

	vb->Unlock();

	if (iPrimCount == 0)
		return ;

	mRenderOp->iPrimCount = iPrimCount;

	RenderSystem * render = RenderSystem::Instance();

	ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

	uColor->SetUnifom(0.5f, 0.5f, 0.5f, 1);

	mRenderOp->xform = entity->GetAttachNode()->GetWorldMatrix();

	render->Render(mTech, mRenderOp);
}

void xSkeletonRenderer::_init(Event * sender)
{
	mRenderOp = new RenderOp();

	VertexStream * vxStream = &mRenderOp->vxStream;
	IndexStream * ixStream = &mRenderOp->ixStream;

	int iVertexCount = 256 * 5;
	int iIndexCount = 256 * 6 * 3;
	int iPrimCount = 0;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
	decl->Init();

	vxStream->SetDeclaration(decl);

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12);
	vxStream->Bind(0, vb, 12);
	vxStream->SetCount(iVertexCount);

	IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
	short * indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
	for (int i = 0; i < 256; ++i)
	{
		int start = i * 5;

		*indices++ = start + 0, *indices++ = start + 1; *indices++ = start + 3;
		*indices++ = start + 3; *indices++ = start + 1; *indices++ = start + 2;

		*indices++ = start + 0; *indices++ = start + 1; *indices++ = start + 4;
		*indices++ = start + 1; *indices++ = start + 2; *indices++ = start + 4;
		*indices++ = start + 2; *indices++ = start + 3; *indices++ = start + 4;
		*indices++ = start + 3; *indices++ = start + 0; *indices++ = start + 4;
	}
	ibuffer->Unlock();

	ixStream->SetCount(iIndexCount);
	ixStream->Bind(ibuffer, 0);

	mRenderOp->iPrimCount = iPrimCount;
	mRenderOp->ePrimType = PRIM_TRIANGLELIST;

	mRenderOp->rState.cullMode = CULL_NONE;
	mRenderOp->rState.blendMode = BM_OPATICY;
	mRenderOp->rState.fillMode = FILL_FRAME;

	mTech = xApp::Instance()->GetHelperShaderLib()->GetTechnique("Color");

	d_assert (mTech);
}

void xSkeletonRenderer::_shutdown(Event * sender)
{
	delete mRenderOp;
}
