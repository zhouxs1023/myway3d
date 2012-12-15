#pragma once

#include "xTextureExporter.h"

namespace MaxPlugin {

class xMesh;
struct xSubMesh;
class xSkeleton;

class xMeshExporter
{
	DECLARE_SINGLETON (xMeshExporter);

public:
	xMeshExporter(ExpInterface * ei, Interface * i);
	~xMeshExporter();

	void Export();

	ExpInterface * GetExpInterface() { return mExpInterface; }
	Interface * GetInterface() { return mInterface; }
	IGameScene * GetGameScene() { return mGameScene; }

	void WriteSubMesh(xSubMesh * mesh, File & file);
	void WriteSkel(xSkeleton * skel, File & file);

protected:
	ExpInterface * mExpInterface;
	Interface * mInterface;

	IGameScene * mGameScene;
};


}