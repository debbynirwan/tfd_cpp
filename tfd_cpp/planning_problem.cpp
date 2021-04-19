#include "planning_problem.h"

namespace tfd_cpp
{
    PlanningProblem::PlanningProblem(const PlanningDomain& domain, 
                                     const State& initialState, 
                                     const Task& topLevelTask) : 
        m_planningDomain(domain),
        m_initialState(initialState),
        m_topLevelTask(topLevelTask)
    {
    }

    PlanningProblem::~PlanningProblem() 
    {
    }

    bool PlanningProblem::TaskIsOperator(const std::string& taskName) const
    {
        return m_planningDomain.TaskIsOperator(taskName);
    }

    bool PlanningProblem::TaskIsMethod(const std::string& taskName) const
    {
        return m_planningDomain.TaskIsMethod(taskName);
    }

    PlanningProblem::RelevantMethods PlanningProblem::GetMethodsForTask(const Task& task, const State& currentState) const
    {
        const auto relevantMethods = m_planningDomain.GetRelevantMethods(currentState, task);
        if (relevantMethods)
        {
            return relevantMethods.value();
        }
        else
        {
            return {};
        }
    }

    PlanningProblem::ApplicableOperators PlanningProblem::GetOperatorsForTask(const Task& task, const State& currentState) const
    {
        const auto applicableOperators = m_planningDomain.GetApplicableOperators(currentState, task);
        if (applicableOperators)
        {
            return applicableOperators.value();
        }
        else
        {
            return {};
        }
    }

    State PlanningProblem::GetInitialState() const
    {
        return m_initialState;
    }

    Task PlanningProblem::GetTopLevelTask() const
    {
        return m_topLevelTask;
    }
}
