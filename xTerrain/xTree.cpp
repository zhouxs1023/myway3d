#include "stdafx.h"

#include "xTree.h"

DF_PROPERTY_BEGIN(xTree)
	DF_PROPERTY(xTree, TreeFile, "General", "File", PT_TString, 128)
	DF_PROPERTY(xTree, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xTree, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xTree, Scale, "Transform", "Scale", PT_Float, 4)

	DF_PROPERTY(xTree, LeafDiffuse, "Material", "LeafDiffuse", PT_Color, 16)
	DF_PROPERTY(xTree, FrondDiffuse, "Material", "FrondDiffuse", PT_Color, 16)
	DF_PROPERTY(xTree, BranchDiffuse, "Material", "BranchDiffuse", PT_Color, 16)
DF_PROPERTY_END();

xTree::xTree(const TString128 & name)
	: xObj(name)
{
	TreeFile = "";
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = 1;

	LeafDiffuse = Color4::White;
	FrondDiffuse = Color4::White;
	BranchDiffuse = Color4::White;

	mNode = World::Instance()->CreateSceneNode();
	mTree = MForest::Instance()->CreateTreeInstance(name);
	mNode->Attach(mTree);

	mNode->GetFlag().SetFlags(PICK_Flag);
}

xTree::~xTree()
{
	IPhyWorld::Instance()->RemoveNode(mNode);
	MForest::Instance()->DestroyInstance(mTree);
	World::Instance()->DestroySceneNode(mNode);
}

void xTree::SetName(const TString128 & name)
{
	Name = name;
	MForest::Instance()->RenameTreeInstance(name, mTree);
}

void xTree::SetTreeFile(const TString128 & meshFile)
{
	TreeFile = meshFile;

	if (mTree->GetTree() == NULL || mTree->GetTree()->GetSourceName() != meshFile)
	{
		mTree->SetTree(meshFile);

		IPhyWorld::Instance()->RemoveNode(mNode);

		if (mTree->GetTree() == NULL)
			return ;

		if (mTree->GetTree()->GetColMesh()->GetPositions().Size() == 0)
			return ;

		IColObjPtr colObj = IPhyWorld::Instance()->GetColMesh(mTree->GetTree().c_ptr(), Scale);

		if (colObj == NULL)
			colObj = IPhyWorld::Instance()->AddColMesh(mTree->GetTree().c_ptr(), mTree->GetTree()->GetColMesh(), Scale);

		IPhyWorld::Instance()->AddNode(mNode, colObj);

		SetLeafDiffuse(LeafDiffuse);
		SetFrondDiffuse(FrondDiffuse);
		SetBranchDiffuse(BranchDiffuse);
	}
}

xObj * xTree::Clone()
{
	xTree * tree = (xTree *)xObjManager::Instance()->Create(GetTypeName().c_str());

	tree->SetPosition(Position);
	tree->SetOrientation(Orientation);
	tree->SetScale(Scale);
	tree->SetTreeFile(TreeFile);

	tree->SetLeafDiffuse(LeafDiffuse);
	tree->SetFrondDiffuse(FrondDiffuse);
	tree->SetBranchDiffuse(BranchDiffuse);

	return tree;
}

bool xTree::IsSceneNode(SceneNode * node)
{
	return mNode == node;
}

void xTree::SetPosition(const Vec3 & p)
{
	Position = p;
	mNode->SetPosition(Position);
}

void xTree::SetOrientation(const Quat & q)
{
	Orientation = q;
	mNode->SetOrientation(q);
}

void xTree::SetScale(float s)
{
	Scale = s;
	mNode->SetScale(Scale);

	IPhyWorld::Instance()->OnNodeScaleChanged(mNode);
}

Vec3 xTree::GetPosition()
{
	return Position;
}

Quat xTree::GetOrientation()
{
	return Orientation;
}

float xTree::GetScale()
{
	return Scale;
}

void xTree::SetLeafDiffuse(const Color4 & c)
{
	LeafDiffuse = c;
	if (mTree)
		mTree->SetLeafDiffuse(LeafDiffuse);
}

void xTree::SetFrondDiffuse(const Color4 & c)
{
	FrondDiffuse = c;
	if (mTree)
		mTree->SetFrondDiffuse(FrondDiffuse);
}

void xTree::SetBranchDiffuse(const Color4 & c)
{
	BranchDiffuse = c;
	if (mTree)
		mTree->SetBranchDiffuse(BranchDiffuse);
}

void xTree::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 1;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << TreeFile;
		Serializer << Position;
		Serializer << Orientation;
		Serializer << Scale;
		Serializer << LeafDiffuse;
		Serializer << FrondDiffuse;
		Serializer << BranchDiffuse;
	}
	else
	{
		Serializer >> version;
		if (version >= 0)
		{
			Serializer >> TreeFile;
			Serializer >> Position;
			Serializer >> Orientation;
			Serializer >> Scale;
		}
		
		if (version >= 1)
		{
			Serializer >> LeafDiffuse;
			Serializer >> FrondDiffuse;
			Serializer >> BranchDiffuse;
		}

		SetPosition(Position);
		SetOrientation(Orientation);
		SetScale(Scale);
		SetTreeFile(TreeFile);

		SetLeafDiffuse(LeafDiffuse);
		SetFrondDiffuse(FrondDiffuse);
		SetBranchDiffuse(BranchDiffuse);
	}
}

Aabb xTree::GetBound()
{
	return mNode->GetWorldAabb();
}

ColMesh * xTree::GetColMesh()
{
	if (mTree && mTree->GetTree() != NULL)
	{
		return mTree->GetTree()->GetColMesh();
	}

	return NULL;
}


bool xTree::OnPropertyChanged(const Property * p)
{
	if (p->name == "Name")
	{
		SetName(Name);
	}
	else if (p->name == "TreeFile")
	{
		SetTreeFile(TreeFile);
	}
	else if (p->name == "Position")
	{
		SetPosition(Position);
	}
	else if (p->name == "Scale")
	{
		SetScale(Scale);
	}
	else if (p->name == "LeafDiffuse")
	{
		SetLeafDiffuse(LeafDiffuse);
	}
	else if (p->name == "FrondDiffuse")
	{
		SetFrondDiffuse(FrondDiffuse);
	}
	else if (p->name == "BranchDiffuse")
	{
		SetBranchDiffuse(BranchDiffuse);
	}

	return true;
}










xTreeFactoryListener gListener;


void xTreeFactoryListener::_OnDragFile(void * param0, void * param1)
{
	Point2f pt = *(Point2f*)param0;
	TString128 ActorFile = (const char *)param1;

	ActorFile.Lower();
	ActorFile.Replace('/', '\\');

	TString128 externName;

	externName = File::GetExternName(ActorFile);

	if (externName != "spt")
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

	xTree * obj = (xTree *)xObjManager::Instance()->Create("Tree");

	d_assert (obj);

	Vec3 pos = xApp::Instance()->GetHitPosition(pt.x, pt.y);

	obj->SetPosition(pos);
	obj->SetTreeFile(testFile);

	xApp::Instance()->SetSelectedObj(obj);
}
