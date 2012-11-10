#include "MWTaskManager.h"

using namespace Myway;

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::AddTask(const TaskProcInfo & info)
{
    debug_assert(!Find(info.flag), "This Flag Is Exist.");

    Array<TaskProcInfo>::Iterator iter;
    Array<TaskProcInfo>::Iterator end;

    iter = mProcs.Begin();
    end = mProcs.End();

    while (iter != end && iter->level <= info.level)
    {

        ++iter;
    }

    mProcs.Insert(iter, info);
}

void TaskManager::Remove(int flag)
{
    Array<TaskProcInfo>::Iterator iter;
    Array<TaskProcInfo>::Iterator end;

    iter = mProcs.Begin();
    end = mProcs.End();

    while (iter != end && iter->flag != flag)
    {
        ++iter;
    }

    assert(iter != end);

    mProcs.Erase(iter);
}

void TaskManager::RemoveAll()
{
    mProcs.Clear();
}

void TaskManager::Update()
{
    Array<TaskProcInfo>::Iterator iter;
    Array<TaskProcInfo>::Iterator end;

    iter = mProcs.Begin();
    end = mProcs.End();

    while (iter != end)
    {
        iter->proc();
        ++iter;
    }

}

void TaskManager::Process(int flag)
{
    Array<TaskProcInfo>::Iterator iter;
    Array<TaskProcInfo>::Iterator end;

    iter = mProcs.Begin();
    end = mProcs.End();

    while (iter != end && iter->flag != flag)
    {
        ++iter;
    }

    assert(iter != end);

    iter->proc();
}

TaskProcInfo * TaskManager::Find(int flag)
{
    Array<TaskProcInfo>::Iterator iter;
    Array<TaskProcInfo>::Iterator end;

    iter = mProcs.Begin();
    end = mProcs.End();

    while (iter != end && iter->flag != flag)
    {
        ++iter;
    }

    assert(iter != end);

    return &(*iter);
}