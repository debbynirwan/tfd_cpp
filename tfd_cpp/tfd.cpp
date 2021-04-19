#include "tfd.h"
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace tfd_cpp
{
    namespace logging = boost::log;

    TFD::TFD(const PlanningProblem& planningProblem) : 
        m_planningProblem(planningProblem)
    {
        logging::add_file_log("tfd_cpp.log");

        logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::trace
        );
    }

    TFD::~TFD() {}

    TFD::Plan TFD::TryToPlan()
    {
        std::vector<Task> tasks;
        Plan solutionPlan;
        tasks.push_back(m_planningProblem.GetTopLevelTask());

        BOOST_LOG_TRIVIAL(info) << "TryToPlan for: " << tasks.back().taskName;

        return SeekPlan(tasks, m_planningProblem.GetInitialState(), solutionPlan);
    }

    TFD::Plan TFD::SeekPlan(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        if (tasks.empty())
        {
            BOOST_LOG_TRIVIAL(info) << "SeekPlan: No more tasks, returning current plan.";
            if (not currentPlan.empty())
            {
                BOOST_LOG_TRIVIAL(info) << "TFD found solution plan." << std::endl;
                for (const auto& operatorWithParams : currentPlan)
                {
                    BOOST_LOG_TRIVIAL(info) << operatorWithParams.task.taskName;
                }
            }
            return currentPlan;
        }

        if (m_planningProblem.TaskIsOperator(tasks.back().taskName))
        {
            BOOST_LOG_TRIVIAL(trace) << "SeekPlan: Task is operator type.";
            return SearchOperators(tasks, currentState, currentPlan);
        }

        if (m_planningProblem.TaskIsMethod(tasks.back().taskName))
        {
            BOOST_LOG_TRIVIAL(trace) << "SeekPlan: Task is method type.";
            return SearchMethods(tasks, currentState, currentPlan);
        }

        return {};
    }

    TFD::Plan TFD::SearchMethods(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        BOOST_LOG_TRIVIAL(trace) << "SearchMethods for " << tasks.back().taskName;
        RelevantMethods relevantMethods = m_planningProblem.GetMethodsForTask(tasks.back(), currentState);
        
        if (not relevantMethods.empty())
        {
            BOOST_LOG_TRIVIAL(trace) << "SearchMethods: " << relevantMethods.size() <<  " relevant methods found.";

            for (const auto& relevantMethod : relevantMethods)
            {
                auto subTasks = relevantMethod.func(currentState, relevantMethod.task.parameters);
                if (subTasks and not subTasks.value().empty())
                {
                    std::vector<Task> newTasks(tasks);
                    newTasks.pop_back();
                    newTasks.insert(newTasks.end(), subTasks.value().begin(), subTasks.value().end());

                    return SeekPlan(newTasks, currentState, currentPlan);
                }
            }            
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << "SearchMethods: No relevant methods found.";
        }

        BOOST_LOG_TRIVIAL(warning) << "SearchMethods: Failed to plan";
        return {};
    }

    TFD::Plan TFD::SearchOperators(const std::vector<Task>& tasks, const State& currentState, Plan& currentPlan)
    {
        BOOST_LOG_TRIVIAL(trace) << "SearchOperators for " << tasks.back().taskName;
        ApplicableOperators applicableOperators = m_planningProblem.GetOperatorsForTask(tasks.back(), currentState);
        
        if (not applicableOperators.empty())
        {
            const auto chosenOperator = applicableOperators.begin();
            const auto newState = chosenOperator->func(currentState, chosenOperator->task.parameters);
            
            if (newState)
            {
                std::vector<Task> newTasks(tasks);
                newTasks.pop_back();
                currentPlan.push_back(*chosenOperator);

                return SeekPlan(newTasks, newState.value(), currentPlan);
            }
            //TODO: if an operator fails, we may want to nondeterministically try other options 
        }
        else
        {
            BOOST_LOG_TRIVIAL(warning) << "SearchOperators: No applicable operator found.";
        }

        return {};
    }
}
