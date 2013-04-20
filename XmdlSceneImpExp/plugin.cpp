#include "stdafx.h"

#include "plugin.h"
#include "xmdl.h"
#include "zone.h"

namespace xmdl {

	t_plugin::t_plugin()
	{
	}

	t_plugin::~t_plugin()
	{
	}

	void t_plugin::Import(const char * filename)
	{
		_convertXMDL(filename);

		t_scene scene;

		scene.load(filename);
	}

	void __convert(const TString128 & ofile, const TString128 & ifile)
	{
		xmdl::t_xmdl mdl;

		mdl.load(ifile.c_str());
		mdl.save(ofile.c_str());
	}

	void __copyTex(const TString128 & ofile, const TString128 & ifile)
	{
		DataStreamPtr file = ResourceManager::Instance()->OpenResource(ifile.c_str(), true);

		File fp;

		fp.Open(ofile.c_str(), OM_WRITE_BINARY);
		fp.Write(file->GetData(), file->Size());

		fp.Close();
	}

	void t_plugin::_convertXMDL(const char * filename)
	{
		TString128 path = filename;

		int count = 0;
		int len = path.Length();

		while (count < 2)
		{
			if (path[len - 1] == '\\' || path[len - 1] == '/')
				++count;

			path[len - 1] = 0;

			--len;
		}

		{
			TString128 dirtyFile = path + "\\actor\\dirty.txt";

			if (File::Exist(dirtyFile))
			{
				return ;
			}

			File fp;

			fp.Open(dirtyFile.c_str());

			fp.Close();
		}

		FileSystem fs(path + "\\xmdl\\宠物");

		fs.Load();

		Archive::FileInfoVisitor v = fs.GetFileInfos();

		while (!v.eof())
		{
			if (v.Cursor()->second.type != Archive::FILE_DIRECTORY)
			{
				TString128 _xmdl = v.Cursor()->second.name;

				if (File::GetExternName(_xmdl) == "xmdl")
				{
					TString128 ifilename = path + "\\xmdl\\宠物\\" + _xmdl;
					TString128 ofilename = path + "\\actor\\宠物\\" + File::RemoveExternName(_xmdl) + ".mesh";

					__convert(ofilename, ifilename);
				}
				
				else if (File::GetExternName(_xmdl) == "dds")
				{
					TString128 ifilename = path + "\\xmdl\\宠物\\" + _xmdl;
					TString128 ofilename = path + "\\actor\\texture\\" + File::GetBaseName(_xmdl);

					__copyTex(ofilename, ifilename);
				}
			}

			++v;
		}
	}
}