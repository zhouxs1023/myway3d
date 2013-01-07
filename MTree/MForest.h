#pragma once

#include "MTreeEntry.h"
#include "MVegetation.h"
#include "MVegetationBlock.h"

namespace Myway {

	class MTREE_ENTRY MForest
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON (MForest);

	public:
		MForest();
		~MForest();

	protected:
		void Init();
		void Shutdown();

		void LoadVegetations(const TString128 & source);
		MVegetation * GetVegetation(const TString128 & name) const;

		void _render(void*, void*);
		void _preVisibleCull(void*, void*);


		void _drawMeshVeg();
		void _drawBillboardVeg();
		void _drawX2Veg();

	protected:
		ShaderLib * mShaderLib;

		Array<MVegetation *> mVegetations;
		Array<MVegetationBlock *> mVegetationBlocks;

		Array<MVegetationBlock *> mVisibleVegetationBlocks;

		Technique * mTech_VegMesh;
		Technique * mTech_VegBillboard;
		Technique * mTech_VegX2;
	};


	class MForestListener
	{
	public:
		MForestListener();
		~MForestListener();

		void _init(void * param0, void * param1);
		void _shutdown(void * param0, void * param1);
		void _update(void * param0, void * param1);

	protected:
		tEventListener<MForestListener> OnInit;
		tEventListener<MForestListener> OnShutdown;
		tEventListener<MForestListener> OnUpdate;

		MForest * mForest;
	};
}