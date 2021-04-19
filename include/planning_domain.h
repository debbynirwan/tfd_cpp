#pragma once

#include <string>
#include <any>
#include <vector>
#include <map>
#include <optional>
#include <iostream>
#include <functional>

namespace tfd_cpp
{
    using Parameters = std::vector<std::any>;
    
    struct State
    {
        std::string domainName;
        std::any data;
    };

    struct Task
    {
        std::string taskName;
        Parameters parameters;
    };

    typedef std::function<std::optional<State>(const State&, const Parameters&)> OperatorFunction;
    typedef std::function<std::optional<std::vector<Task>>(const State&, const Parameters&)> MethodFunction;

    struct OperatorWithParams
    {
        OperatorWithParams(const Task& task, const OperatorFunction& func) : 
            task(task),
            func(func) {}

        Task task;
        OperatorFunction func;
    };

    struct MethodWithParams
    {
        MethodWithParams(const Task& task, const MethodFunction& func) : 
            task(task),
            func(func) {}
        
        Task task;
        MethodFunction func;
    };

    using Operators = std::vector<OperatorFunction>;
    using Methods = std::vector<MethodFunction>;
    using OperatorsWithParams = std::vector<OperatorWithParams>;
    using MethodsWithParams = std::vector<MethodWithParams>;

    class PlanningDomain
    {
    public:
        PlanningDomain(const std::string& domainName);
        ~PlanningDomain();

        void AddOperator(const std::string& taskName, const OperatorFunction& operatorFunc);
        void AddMethod(const std::string& taskName, const MethodFunction& methodFunc);

        std::optional<OperatorsWithParams> GetApplicableOperators(const State& currentState, const Task& task) const;
        std::optional<MethodsWithParams> GetRelevantMethods(const State& currentState, const Task& task) const;

        bool TaskIsOperator(const std::string& taskName) const;
        bool TaskIsMethod(const std::string& taskName) const;
    
    private:
        std::string m_domainName;
        std::map<std::string, Operators> m_operatorTable;
        std::map<std::string, Methods> m_methodTable;
    };

    std::ostream& operator<<(std::ostream& os, const State& state);
    std::ostream& operator<<(std::ostream& os, const Task& task);
}
