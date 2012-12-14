#pragma once

#include "xTextureExporter.h"

namespace MaxPlugin {

class xMesh;
struct xSubMesh;

class xMeshExporter : public ITreeEnumProc
{
	DECLARE_SINGLETON (xMeshExporter);

public:
	xMeshExporter(ExpInterface * ei, Interface * i);
	~xMeshExporter();

	void Export();

	ExpInterface * GetExpInterface() { return mExpInterface; }
	Interface * GetInterface() { return mInterface; }
	IGameScene * GetGameScene() { return mGameScene; }

protected:
	int callback(INode *node);

	void WriteSubMesh(xSubMesh * mesh, File & file);

protected:
	ExpInterface * mExpInterface;
	Interface * mInterface;

	IGameScene * mGameScene;
	Tab<INode*> mNodeTab;
	Map<TString128, int> mBoneIndexMap;
	int mBoneIndex;
};


}