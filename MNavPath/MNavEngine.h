#pragma once

#include "MNavEntry.h"

#include "MNavPath.h"

namespace Myway {

	class MNAV_ENTRY NavEngine
	{
		DECLARE_ALLOC();

		DECLARE_SINGLETON(NavEngine);

	public:
		struct Node
		{
			int x, y;
			int s, g;

			Node * parent;
			Node * next;
		};

	public:
		NavEngine();
		virtual ~NavEngine();

		void Init();
		void Shutdown();

		bool Do(int x, int y, int dx, int dy);
		void Clear();

		bool IsValid(int x, int y);

		void SetMaxStep(int num) { mMaxStep = num; } 
		int GetMaxStep() const { return mMaxStep; }

		NavPath * GetNavPath() { return mNavPath; };

	protected:
		void PushOpen(int x, int y, int s, int g, Node * p);
		void PushClose(Node * n);
		void ClearClose(int x, int y);

		unsigned int CalcuH(int x, int y, int dx, int dy);

	protected:
		NavPath * mNavPath;

		int mMaxStep;

		Node * mOpen;
		Node * mClose;
		Allocator<Node, 128> mAlloc;
	};
}