//////////////////////////////////////////////////////////////////////////
//
// SpeedTreeRT Integration.
//   SpeedTreeRT engine: http://www.idvinc.com
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "MWMesh.h"

namespace Myway {

	struct MVegetation
	{
		DECLARE_ALLOC();

		enum GeomType
		{
			GT_BillBoard,
			GT_X2,
			GT_Mesh
		};

		TString32 Name;
		GeomType Type;
		MeshPtr pMesh;
		TexturePtr DiffuseMap;
		TexturePtr NormalMap;
		TexturePtr SpecularMap;
	};
	

	
	class MVegetationLoader
	{
	public:
		MVegetationLoader();
		~MVegetationLoader();

		void Load(Array<MVegetation*> * Vegs, const TString128 & source);

	protected:
		void _loadType(xml_node * nd);
		void _loadMesh(xml_node * nd);
		void _loadDiffuseMap(xml_node * nd);

	protected:
		Array<MVegetation*> * mVegs;
		MVegetation * mCurVeg;
		TString128 mSource;
	};
}