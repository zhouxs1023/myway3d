#pragma once

#include "MSpeedTreeEntry.h"
#include "MTree.h"
#include "MWind.h"
#include "MTreeInstance.h"

namespace Myway {

	class MTREE_ENTRY MForest
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON (MForest);

	public:
		MForest();
		~MForest();

		void Advance();

		MTreeInstance * CreateTreeInstance(const TString128 & name);
		MTreeInstance * GetTreeInstance(const TString128 & name);
		void DestroyTreeInstance(MTreeInstance * inst);

		MTreePtr LoadTree(const TString128 & sourceName);
		void DestroyTree(MTree * tree);
		MTreePtr GetTree(const TString128 & sourceName);

		void Clear();

		void _addVisibleTree(MTreeInstance * inst);

	protected:
		void Init();
		void Shutdown();

		void _render(void*, void*);
		void _preVisibleCull(void*, void*);

		void _renderBranch(MTreeInstance * inst);

	protected:
		struct BranchParam {
			ShaderParam * uWindMatOff;
			ShaderParam * uWindMatrix;

			ShaderParam * uPosition;
			ShaderParam * uMatRotation;

			void Bind(Technique * tech)
			{
				uWindMatOff = tech->GetVertexShaderParamTable()->GetParam("gWindMatrixOffset");
				uWindMatrix = tech->GetVertexShaderParamTable()->GetParam("gWindMatrices");
				
				uPosition = tech->GetVertexShaderParamTable()->GetParam("gTreePosition");
				uMatRotation = tech->GetVertexShaderParamTable()->GetParam("gMatRotation");
			}
		};

	protected:
		MTreeWind mWind;

		Array<MTree *> mTrees;
		Array<MTreeInstance *> mTreeInstances;
		Array<MTreeInstance *> mVisibleInsts;

		ShaderLib * mShaderLib;
		Technique * mTech_Branch;
		Technique * mTech_Frond;
		Technique * mTech_Leaf;
		BranchParam mParam_Branch;

		Mat4 mMatWind[MTreeGlobal::K_NumWindMatrices];

		tEventListener<MForest> OnPreVisibleCull;
		tEventListener<MForest> OnRender;
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