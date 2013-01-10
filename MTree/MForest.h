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

		void LoadVeg(const TString128 & source);
		void SaveVeg(const TString128 & source);

		void AddVegetation(const TString32 & name, MVegetation::GeomType type,
			const TString128 & mesh, const TString128 & diffueMap,
			const TString128 & normalMap, const TString128 & specularMap);
		void RemoveVegetation(MVegetation * veg);
		int GetVegetationCount() const;
		void OnVegetationChanged(MVegetation * veg);

		MVegetation * GetVegetation(int index);
		MVegetation * GetVegetationByName(const TString32 & name);

		void AddVegetationInst(MVegetation * veg, const Vec3 & position, float size);
		void RemoveVegetationInst(const RectF & rc, MVegetation * veg);

		void CreateVegetationBlocks(const RectF & rect, int xCount, int zCount);
		MVegetationBlock * GetVegetationBlock(int x, int z);
		int GetVegetationBlockCount();
		void RemoveAllVegetationBlock();

		void UnloadVeg();

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

		void _OnVegRemoved(MVegetation * veg);

	protected:
		ShaderLib * mShaderLib;

		Array<MVegetation *> mVegetations;

		RectF mVegBlockRect;
		int mXVegBlockCount;
		int mZVegBlockCount;
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