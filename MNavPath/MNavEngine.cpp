#include "MNavEngine.h"

namespace Myway {

	static NavEngine::Node k_end = { -1, -1, INT_MAX, NULL};

	static NavEngine::Node * n_end = &k_end;

	IMP_SLN(NavEngine);

	NavEngine::NavEngine()
	{
		INIT_SLN;
	}

	NavEngine::~NavEngine()
	{
		SHUT_SLN;
	}

	bool NavEngine::IsValid(int x, int y)
	{
		return mNavPath->Get(x, y) == 0;
	}

	void NavEngine::Init()
	{
		mNavPath = new NavPath();
	}

	void NavEngine::Shutdown()
	{
		safe_delete (mNavPath);
	}

	bool NavEngine::Do(int x, int y, int dx, int dy)
	{
		Clear();

		const Size2 & size = mNavPath->GetSize();

		if (dx < 0 || dx >= size.w ||
			dy < 0 || dy >= size.h)
			return false;

		if (!IsValid(dx, dy))
			return false;

		if (x < 0 || x >= size.w ||
			y < 0 || y >= size.h)
			return false;

		// current position is invalid.
		if (!IsValid(x, y))
		{
			// left
			if (x - 1 >= 0 && IsValid(x, y))
			{
				x = x - 1;
			}
			// right
			else if (x + 1 < size.w && IsValid(x, y))
			{
				x = x + 1;
			}
			// top
			else if (y - 1 >= 0 && IsValid(x, y - 1))
			{
				y = y - 1;
			}
			// bottom
			else if (y + 1 < size.h && IsValid(x, y + 1))
			{
				y = y + 1;
			}
			else
			{
				return false;
			}
		}

		if (x == dx && y == dy)
			return true;

		PushOpen(x, y, 0, CalcuH(x, y, dx, dy), NULL);

		int step = 0;

		while (mOpen != n_end && step < mMaxStep)
		{
			Node * node = mOpen;
			mOpen->next = node->next;

			int _x = node->x;
			int _y = node->y;

			PushClose(node);

			if (_x == dx && _y == dy)
			{
				break;
			}

			int s = node->s + 1;

			if(_x - 1 >= 0 && IsValid(_x - 1, y))
			{
				PushOpen(_x - 1, _y, s, s + CalcuH(_x - 1, _y, dx, dy), node);
			}

			// right
			if(_x + 1 < size.w && IsValid(_x + 1, _y))
			{
				PushOpen(_x + 1, _y, s, s + CalcuH(_x + 1, _y, dx, dy), node);
			}

			// top
			if(_y - 1 >= 0 && IsValid(_x, _y - 1))
			{
				PushOpen(_x, _y - 1, s, s + CalcuH(_x, _y - 1, dx, dy), node);
			}

			// bottom
			if(_y + 1 < size.h && IsValid(_x, _y + 1))
			{
				PushOpen(_x, _y + 1, s, s + CalcuH(_x, _y + 1, dx, dy), node);
			}

			++step;
		}

		if (step > mMaxStep)
			 return false;

		return true;
	}

	void NavEngine::Clear()
	{
		mAlloc.Shutdown();
		mOpen = n_end;
	}

	unsigned int NavEngine::CalcuH(int x, int y, int dx, int dy)
	{
		int _x = Math::Abs(dx - x);
		int _y = Math::Abs(dy - y);

		return _x + _y;
	}

	void NavEngine::PushOpen(int x, int y, int s, int g, Node * p)
	{
		ClearClose(x, y);

		Node * node = mOpen;
		Node * prev = NULL;

		bool exist = false;

		while (node != n_end)
		{
			if (node->x == x && node->y == y)
			{
				exist = true;

				if (node->g <= g)
					return ;

				if (prev)
					prev->next = node->next;
				else
					mOpen = node->next;

				node->next = NULL;
			}

			prev = node;
			node = node->next;
		}

		Node * newNode = node;

		if (!exist)
		{
			newNode = mAlloc.Alloc();
			newNode->x = x;
			newNode->y = y;
			newNode->next = NULL;
		}

		newNode->parent = p;
		newNode->s = s;
		newNode->g = g;

		node = mOpen;
		prev = NULL;

		while (node)
		{
			if (node->g >= newNode->g)
			{
				newNode->next = node;

				if (prev)
					prev->next = newNode;
				else
					mOpen = newNode;

				return ;
			}

			prev = node;
			node = node->next;
		}
	}

	void NavEngine::PushClose(Node * n)
	{
		n->next = mClose;
		mClose = n;
	}

	void NavEngine::ClearClose(int x, int y)
	{
		Node * node = NULL;
		Node * prev = NULL;

		while (node)
		{
			if (node->x == x && node->y == y)
			{
				if (prev)
				{
					prev->next = node->next;
				}
				else
				{
					mClose = node->next;
				}

				mAlloc.Free(node);

				return ;
			}

			prev = node;
			node = node->next;
		}
	}
}