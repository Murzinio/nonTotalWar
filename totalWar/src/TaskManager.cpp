#include "..\headers\TaskManager.h"



TaskManager::TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units) : m_units(units)
{
}

void TaskManager::HandleTasks()
{
    for (auto & x : m_units)
    {
        auto task = x.second->GetTask();
        if (task == nonTotalWar::UnitTask::NONE)
            continue;
    }
}
