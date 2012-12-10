#pragma once

class xMeshExporter : public ITreeEnumProc
{
public:
	xMeshExporter(ExpInterface * ei, Interface * i);
	~xMeshExporter();

	void Build();

protected:
	int callback(INode *node);

protected:
	ExpInterface * mExpInterface;
	Interface * mInterface;

	IGameScene * mGameScene;
	Tab<INode*> mNodeTab;
	Map<TString128, int> mBoneIndexMap;
	int mBoneIndex;
};