#pragma once

#include "GameCore.h"

class EditorExport
{
public:
	void Export(const char * filename);

protected:
	void SaveHead(File & file);
	void SaveEnvironment(File & file);
	void SaveTerrain(File & file);
	void SaveMesh(File & file);
	void SaveForest(File & file);
	void SaveParticle(File & file);
	void SaveSound(File & file);
};