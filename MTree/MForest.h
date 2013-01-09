#pragma once

#include "MTreeEntry.h"
#include "MVegetation.h"
#include "MVegetationBlock.h"

namespace Myway {

	class MTREE_ENTRY MForest
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON (MForest);

		friend class MForestListener;

	public:
		MForest();
		~MForest();

		void AddVegetation(const TString128 & name, MVegetation::GeomType type,
			const TString128 & mesh, const TString128 & diffueMap,
			const TString128 & normalMap, const TString128 & specularMap);

		int GetVegetationCount() const;
		MVegetation * GetVegetation(int index);
		MVegetation * GetVegetationByName(const TString128 & name);

		void AddVegetationInst(MVegetation * veg, const Vec3 & position, float size);
		void RemoveVegetationInst(const RectF & rc, MVegetation * veg);

		void CreateVegetationBlocks(const RectF & rect, int xCount, int zCount);
		void RemoveAllVegetationBlock();

		void Unload();

		void _AddVisibleVegetationBlock(MVegetationBlock * block);

	protected:
		void Init();
		void Update();
		void Shutdown();

		void _render();
		void _preVisibleCull();


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
		void _render(void * param0, void * param1);
		void _preVisibleCull(void * param0, void * param1);

	protected:
		tEventListener<MForestListener> OnInit;
		tEventListener<MForestListener> OnShutdown;
		tEventListener<MForestListener> OnUpdate;
		tEventListener<MForestListener> OnPreVisibleCull;
		tEventListener<MForestListener> OnRender;

		MForest * mForest;
	};
}