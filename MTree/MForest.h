#pragma once

#include "MTreeEntry.h"

#include "MVegetation.h"
#include "MVegetationBlock.h"

#include "MTree.h"
#include "MTreeInstance.h"

namespace Myway {

	class MTREE_ENTRY MForest
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON (MForest);

		friend class MForestListener;

	public:
		MForest();
		~MForest();

		// Vegetation Methods.
		//
		void LoadVeg(const TString128 & source);
		void SaveVeg(const TString128 & source);
		void UnloadVeg();

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

		void _AddVisibleVegetationBlock(MVegetationBlock * block);


		// Tree Methods.
		//
		MTreePtr LoadTree(const TString128 & source);
		void DeleteTree(MTree * tree);

		MTreeInstance * CreateTreeInstance(const TString128 & name, const TString128 & source);
		MTreeInstance * CreateTreeInstance(const TString128 & name);
		MTreeInstance * GetTreeInstance(const TString128 & name);
		bool RenameTreeInstance(const TString128 & name, MTreeInstance * inst);
		void DestroyInstance(MTreeInstance * tree);

		void _AddVisibleTreeInstance(MTreeInstance * tree);

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

		void _drawBranch();

	protected:
		ShaderLib * mShaderLib;

		// Vegetation
		Array<MVegetation *> mVegetations;

		RectF mVegBlockRect;
		int mXVegBlockCount;
		int mZVegBlockCount;
		Array<MVegetationBlock *> mVegetationBlocks;
		Array<MVegetationBlock *> mVisibleVegetationBlocks;

		Technique * mTech_VegMesh;
		Technique * mTech_VegBillboard;
		Technique * mTech_VegX2;

		// Tree
		MTreePtr mDefaultTree;
		Array<MTree *> mTrees;
		Array<MTreeInstance *> mTreeInstances;
		Array<MTreeInstance *> mVisbleTreeInstances;

		Technique * mTech_Branch;
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