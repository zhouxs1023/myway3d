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

	void __convert(const TString128 & dir, const TString128 & file)
	{
		TString128 filename = dir + "\\xmdl\\scene\\" + file;
		TString128 ofilename = dir + "\\mesh\\" + File::RemoveExternName(file) + ".mesh";

		xmdl::t_xmdl mdl;

		mdl.load(filename.c_str());
		mdl.save(ofilename.c_str());
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
			TString128 dirtyFile = path + "\\mesh\\dirty.txt";

			if (File::Exist(dirtyFile))
			{
				return ;
			}

			File fp;

			fp.Open(dirtyFile.c_str());

			fp.Close();
		}

		FileSystem fs(path + "\\xmdl\\scene");

		fs.Load();

		Archive::FileInfoVisitor v = fs.GetFileInfos();

		while (!v.eof())
		{
			if (v.Cursor()->second.type != Archive::FILE_DIRECTORY)
			{
				TString128 _xmdl = v.Cursor()->second.name;

				if (File::GetExternName(_xmdl) == "xmdl")
				{
					__convert(path, _xmdl);
				}
			}

			++v;
		}
	}
}