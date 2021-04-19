#pragma once

#include "simple_travel_domain.h"
#include "planning_problem.h"

#include <memory>
#include <vector>

namespace simple_travel {
    tfd_cpp::PlanningProblem CreatePlanningProblem(const tfd_cpp::Task& topLevelTask);
}
