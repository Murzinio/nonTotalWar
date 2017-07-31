#pragma once
#include <map>
#include <memory>

#include "units\Unit.h"

class TaskManager
{
private:
    std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;

public:
    TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units);

    void HandleTasks();
};

