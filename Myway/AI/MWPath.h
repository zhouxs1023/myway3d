#pragma once

#include "MWMath.h"

namespace Myway
{

class MW_ENTRY Path : public AllocObj
{
public:
    enum PathRet
    {
        PR_UNKNOWN,        // 未知
        PR_INVALID,        // 不合法
        PR_FINISHED,        // 完成了
        PR_UNFINISHED,      // 尚未完成
        PR_REACHED,        // 已经到了
        PR_NOPATH,         // 没有路径

        PR_FORCE_DWORD = 0x7FFFFFFF
    };

    struct Result
    {
        PathRet flag;
        int x, y;
        int dx, dy;
    };
    
protected:
    struct Node
    {
        int x, y;
        Node * parent;
        Node * next;
    };

    struct Queue
    {
        Node * beg;
        Node * end;
    };

public:
    Path(int width, int height);
    ~Path();

    void Set(int x, int y, char flag);
    void Clear(int x, int y, char flag);
    char Get(int x, int y) const;

    bool IsSet(int x, int y, char flag) const;

    void Reset();

    char * GetData();
    const char * GetData() const;

    int GetWidth() const;
    int GetHeight() const;
    int GetSize() const;

    void SetMaxStep(int num);
    int GetMaxStep() const;

    void FindPath(int x, int y, int dx, int dy, char stop, Result & result);

protected:
    void ResetColor();
    void SetColor(int x, int y, char color);
    char GetColor(int x, int y) const;

    void PushOpen(int x, int y, Node * parent);
    Node * PopOpen();
    bool EmptyOpen() const;

    void PushClose(Node * n);
    Node * PopClose();
    bool EmptyClose() const;

    Node * AllocNode();
    void FreeNode(Node * n);

    bool DrawLine(int sx, int sy, int ex, int ey, int stop);
    bool TestLine(int sx, int sy, int ex, int ey, int stop, int & dx, int & dy);
    void CalcuDest(int sx, int sy, int ex, int ey, int & ox, int & oy);
    bool CalcuNext(int x, int y, int ex, int ey, int stop, int & ox, int & oy);

protected:
    int mWidth;
    int mHeight;

    int mMaxStep;

    char * mData;
    char * mColor;

    Allocator<Node, 128> m_alloc;
    Queue m_qOpen;
    Queue m_qClose;
};

}