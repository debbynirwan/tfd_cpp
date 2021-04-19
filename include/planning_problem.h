#pragma once

#include "planning_domain.h"
#include <vector>
#include <memory>

namespace tfd_cpp
{
    class PlanningProblem
    {
    public:
        using RelevantMethods = MethodsWithParams;
        using ApplicableOperators = OperatorsWithParams;
        
        PlanningProblem(const PlanningDomain& domain, const State& initialState, const Task& topLevelTask);
        ~PlanningProblem();

        bool TaskIsOperator(const std::string& taskName) const;
        bool TaskIsMethod(const std::string& taskName) const;
        RelevantMethods GetMethodsForTask(const Task& task, const State& currentState) const;
        ApplicableOperators GetOperatorsForTask(const Task& task, const State& currentState) const;
        State GetInitialState() const;
        Task GetTopLevelTask() const;

    private:
        const PlanningDomain m_planningDomain;
        const State m_initialState;
        const Task m_topLevelTask;
    };
}
