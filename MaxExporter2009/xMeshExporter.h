#pragma once


namespace MaxPlugin {

class xMesh;

class xMeshExporter : public ITreeEnumProc
{
public:
	xMeshExporter(ExpInterface * ei, Interface * i);
	~xMeshExporter();

	void Build();

protected:
	int callback(INode *node);

	void WriteSubMesh(xMesh * mesh, File & file);

protected:
	ExpInterface * mExpInterface;
	Interface * mInterface;

	IGameScene * mGameScene;
	Tab<INode*> mNodeTab;
	Map<TString128, int> mBoneIndexMap;
	int mBoneIndex;
};


}