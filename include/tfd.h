// Total-order Forward Decomposition Algorithm
#pragma once

#include "planning_problem.h"

#include <vector>
#include <utility>

namespace tfd_cpp
{
    class TFD
    {
    public:
        using Plan = OperatorsWithParams;
        using RelevantMethods = PlanningProblem::RelevantMethods;
        using ApplicableOperators = PlanningProblem::ApplicableOperators;

        TFD(const PlanningProblem& planningProblem);
        ~TFD();

        Plan TryToPlan();

    private:
        Plan SeekPlan(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan);
        Plan SearchMethods(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan);
        Plan SearchOperators(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan);

        const PlanningProblem m_planningProblem;
    };
}
