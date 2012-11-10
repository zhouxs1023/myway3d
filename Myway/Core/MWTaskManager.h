#pragma once

#include "MWAllocObj.h"
#include "MWArray.h"

namespace Myway
{

typedef void (*TaskProc)();
struct TaskProcInfo
{
    int         flag;
    int         level;
    TaskProc    proc;
};


class MW_ENTRY TaskManager : public AllocObj
{
public:
    TaskManager();
    ~TaskManager();

    void AddTask(const TaskProcInfo & info);
    void Remove(int flag);
    void RemoveAll();
    void Update();
    void Process(int flag);

    TaskProcInfo * Find(int flag);

protected:
    Array<TaskProcInfo>    mProcs;
};


}
