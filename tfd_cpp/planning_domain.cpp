#include "planning_domain.h"

namespace tfd_cpp {

    PlanningDomain::PlanningDomain(const std::string& domainName) : 
        m_domainName(domainName) {}

    PlanningDomain::~PlanningDomain() {}

    void PlanningDomain::AddOperator(const std::string& taskName, const OperatorFunction& operatorFunc)
    {
        auto operators = m_operatorTable.find(taskName);

        if (operators == m_operatorTable.end())
        {
            m_operatorTable[taskName] = Operators{operatorFunc};
        }
        else
        {
            operators->second.push_back(operatorFunc);
        }
    }

    void PlanningDomain::AddMethod(const std::string& taskName, const MethodFunction& methodFunc)
    {
        auto methods = m_methodTable.find(taskName);

        if (methods == m_methodTable.end())
        {
            m_methodTable[taskName] = Methods{methodFunc};
        }
        else
        {
            methods->second.push_back(methodFunc);
        }
    }

    std::optional<OperatorsWithParams> PlanningDomain::GetApplicableOperators(const State& currentState, const Task& task) const
    {
        OperatorsWithParams operatorsWithParams;

        if (m_operatorTable.empty())
        {
            return std::nullopt;
        }

        for (const auto& element : m_operatorTable)
        {
            if (element.first == task.taskName)
            {
                for (const auto& _operator : element.second)
                {
                    if (_operator(currentState, task.parameters))
                    {
                        operatorsWithParams.emplace_back(task, _operator);
                    }
                }
                break;
            }
        }

        return operatorsWithParams;
    }

    std::optional<MethodsWithParams> PlanningDomain::GetRelevantMethods(const State& currentState, const Task& task) const
    {
        MethodsWithParams methodsWithParams;

        if (m_methodTable.empty())
        {
            return std::nullopt;
        }
        
        for (const auto& element : m_methodTable)
        {
            if (element.first == task.taskName)
            {
                for (const auto& method : element.second)
                {
                    if (method(currentState, task.parameters))
                    {
                        methodsWithParams.emplace_back(task, method);
                    }
                }
                break;
            }
        }

        return methodsWithParams;
    }

    bool PlanningDomain::TaskIsOperator(const std::string& taskName) const
    {
        return (m_operatorTable.find(taskName) != m_operatorTable.end());
    }

    bool PlanningDomain::TaskIsMethod(const std::string& taskName) const
    {
        return (m_methodTable.find(taskName) != m_methodTable.end());
    }

    std::ostream& operator<<(std::ostream& os, const Task& task)
    {
        os << task.taskName << " with " << task.parameters.size() << " parameters.";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const State& state)
    {
        os << "State variable for: " << state.domainName << "\n";
        return os;
    }

}
