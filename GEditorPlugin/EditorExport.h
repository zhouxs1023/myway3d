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


class ServerExport
{
	static const int K_FILE_MAGIC = 'SVMP';
	static const int K_FILE_VERSION = 1;

	static const int FCI_Helper = 'HPR';
	static const int K_Helper_Version = 1;

public:
	void Export(const char * filename);

protected:
	void SaveHead(File & file);
	void SaveHelper(File & file);
};