#pragma once
#include <map>
#include <memory>

#include "units\Unit.h"
#include "Input.h"

class TaskManager
{
private:
    std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;
    SDL_Point m_unitSize;

public:
    TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, SDL_Point unitSize);

    void HandleTasks();

    void Rotate(std::shared_ptr<nonTotalWar::Unit> unit);
};

