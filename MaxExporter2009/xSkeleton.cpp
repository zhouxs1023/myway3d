#include "stdafx.h"

#include "xSkeleton.h"
#include "xMeshExporter.h"

void Warn( LPCTSTR sz )
{
	MessageBox( NULL, sz, TEXT("Warn"), MB_OK | MB_ICONSTOP | MB_TOPMOST );
}

namespace MaxPlugin {

	xSkeleton::xSkeleton()
	{
	}

	xSkeleton::~xSkeleton()
	{
		for (int i = 0; i < mBones.Size(); ++i)
		{
			delete mBones[i];
		}

		mBones.Clear();
	}
	void _dumpSampleKeys(IGameControl * pGameControl, xBone * bone)
	{
		IGameKeyTab Key;
		if(pGameControl->GetFullSampledKeys(Key, 1, IGameControlType(IGAME_TM), true) )
		{
			for(int i=0;i<Key.Count();i++)
			{
				if (Key[i].t>=0)
				{
					xKeyFrame k;

					k.time = Key[i].t / xMeshExporter::Instance()->GetGameScene()->GetSceneTicks() / GetFrameRate();

					Point3 scl = Key[i].sampleKey.gval.Scaling();

					k.position = Key[i].sampleKey.gval.Translation();
					k.orientation = Key[i].sampleKey.gval.Rotation();
					k.scale = scl.x;
				}
			}
		}
	}

	bool _dumpPos(IGameControl * pGameControl, xBone * bone)
	{
		return true;
	}

	bool _dumpRot(IGameControl * pGameControl, xBone * bone)
	{
		return true;
	}

	bool _dumpScale(IGameControl * pGameControl, xBone * bone)
	{
		return true;
	}

	bool _dumpAnim(IGameControl * pGameControl, xBone * bone)
	{
		if ((pGameControl->IsAnimated(IGAME_POS)) || pGameControl->IsAnimated(IGAME_ROT) || pGameControl->IsAnimated(IGAME_SCALE))
		{
			if(pGameControl->GetControlType(IGAME_POS)==IGameControl::IGAME_BIPED||
				pGameControl->GetControlType(IGAME_ROT)==IGameControl::IGAME_BIPED||
				pGameControl->GetControlType(IGAME_SCALE)==IGameControl::IGAME_BIPED)
			{
				_dumpSampleKeys(pGameControl, bone);
				return true;
			}

			if(pGameControl->GetControlType(IGAME_POS)==IGameControl::IGAME_LIST||
				pGameControl->GetControlType(IGAME_ROT)==IGameControl::IGAME_LIST||
				pGameControl->GetControlType(IGAME_SCALE)==IGameControl::IGAME_LIST)
			{
				int subNum = pGameControl->GetNumOfListSubControls(IGAME_TM);
				if(subNum)
				{
					for(int i=0;i<subNum;i++)
					{
						IGameControl * sub = pGameControl->GetListSubControl(i,IGAME_TM);
						if (_dumpAnim(sub, bone))
							return true;
					}
				}
			}

			if (pGameControl->IsAnimated(IGAME_POS))
				_dumpPos(pGameControl, bone);
			if (pGameControl->IsAnimated(IGAME_ROT))
				_dumpRot(pGameControl, bone);
			if (pGameControl->IsAnimated(IGAME_SCALE))
				_dumpScale(pGameControl, bone);
		}
		else
		{
			return false;
		}

		return true;
	}

	int xSkeleton::_getBoneId(const char * name)
	{
		for (int i = 0; i < mBones.Size(); ++i)
		{
			if (mBones[i]->name == name)
				return i;
		}

		return -1;
	}

	void xSkeleton::_dumpBone(IGameNode * node)
	{
		IGameObject * obj = node->GetIGameObject();
		IGameObject::MaxType T = obj->GetMaxType();
		
		switch(obj->GetIGameType())
		{
		case IGameObject::IGAME_BONE:
		case IGameObject::IGAME_HELPER:
			{
				xBone * bone = new xBone();

				bone->name = node->GetName();

				if (node->GetNodeParent())
				{
					bone->parent = _getBoneId(node->GetNodeParent()->GetName());
				}
				else
				{
					INode* pNode = node->GetMaxNode()->GetParentNode();
					if (pNode)
						bone->parent =  _getBoneId(pNode->GetName());
				}
				
				IGameControl * pGameControl = node->GetIGameControl();
				// base matrix
				{
					GMatrix matWorld = node->GetWorldTM();
					bone->position = matWorld.Translation();
					bone->orientation = matWorld.Rotation();
					bone->scale = matWorld.Scaling();
				}

				if (false == _dumpAnim(pGameControl, bone))
				{
				}

				mBones.PushBack(bone);
			}
			break;
		}
	}


	void xSkeleton::Extrat(IGameNode * node)
	{
		if(!node->IsGroupOwner())
			_dumpBone(node);

		for(int i=0;i<node->GetChildCount();i++)
		{
			IGameNode * child = node->GetNodeChild(i);
			// we deal with targets in the light/camera section
			if(child->IsTarget())
				continue;

			Extrat(child);
		}

		node->ReleaseIGameObject();
	}

	void xSkeleton::Push(IGameNode * node)
	{
		for (int i = 0; i < mMaxBones.Size(); ++i)
		{
			if (node == mMaxBones[i])
				return ;
		}

		mMaxBones.PushBack(node);
	}
}