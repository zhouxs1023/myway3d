#include "stdafx.h"

#include "xTree.h"

DF_PROPERTY_BEGIN(xTree)
	DF_PROPERTY(xTree, TreeFile, "General", "File", PT_TString, 128)
	DF_PROPERTY(xTree, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xTree, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xTree, Scale, "Transform", "Scale", PT_Float, 4)
DF_PROPERTY_END();

xTree::xTree(const TString128 & name)
	: xObj(name)
{
	TreeFile = "";
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = 1;

	mNode = World::Instance()->CreateSceneNode();
	mTree = MForest::Instance()->CreateTreeInstance(name);
	mNode->Attach(mTree);

	mNode->GetFlag().SetFlags(PICK_Flag);
}

xTree::~xTree()
{
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
	}
}

xObj * xTree::Clone()
{
	xTree * tree = (xTree *)xObjManager::Instance()->Create(GetTypeName().c_str());

	tree->SetPosition(Position);
	tree->SetScale(Scale);
	tree->SetTreeFile(TreeFile);

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
	mNode->SetPosition(Scale);
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

void xTree::Serialize(xSerializer & serializer)
{
}

Aabb xTree::GetBound()
{
	return mNode->GetWorldAabb();
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

	return true;
}










xTreeFactoryListener gListener;


void xTreeFactoryListener::_OnDragFile(void * param0, void * param1)
{
	Point2f pt = *(Point2f*)param0;
	TString128 meshFile = (const char *)param1;
	TString128 base, path;

	meshFile.Lower();
	meshFile.SplitFileNameR(base, path); 

	TString128 externName;

	externName = File::GetExternName(base);

	if (externName != "spt")
		return ;

	xTree * obj = (xTree *)xObjManager::Instance()->Create("Tree");

	d_assert (obj);

	Vec3 pos = xApp::Instance()->GetHitPosition(pt.x, pt.y);

	obj->SetPosition(pos);
	obj->SetTreeFile(base);

	xApp::Instance()->SetSelectedObj(obj);
}
