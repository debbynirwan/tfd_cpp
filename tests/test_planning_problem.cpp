#include "planning_problem.h"
#include "gtest/gtest.h"
#include <optional>
#include <any>
#include <functional>

namespace {
    std::optional<tfd_cpp::State> Operator(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        tfd_cpp::State newState(state);
        bool status = ! std::any_cast<bool>(state.data);
        newState.data = status;
        
        return newState;
    }

    std::optional<std::vector<tfd_cpp::Task>> Method(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        tfd_cpp::Task task;
        task.taskName = "TestOperator";
        std::vector<tfd_cpp::Task> subtasks{task};

        return subtasks;
    }
}

struct PlanningProblemTest : public ::testing::Test
{
    PlanningProblemTest() : 
        planningDomain("TestDomain"),
        initialState{"TestDomain", false},
        topLevelTask{"TestMethod", {}}{}

    ~PlanningProblemTest() {}

    tfd_cpp::PlanningDomain planningDomain;
    tfd_cpp::State initialState; 
    tfd_cpp::Task topLevelTask;
};

TEST_F(PlanningProblemTest, TaskIsMethod)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

    auto isMethod = planningProblem.TaskIsMethod("TestMethod");
    ASSERT_TRUE(isMethod);
    isMethod = planningProblem.TaskIsMethod("TestOperator");
    ASSERT_FALSE(isMethod);
}

TEST_F(PlanningProblemTest, TaskIsOperator)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

    auto isOperator = planningProblem.TaskIsOperator("TestOperator");
    ASSERT_TRUE(isOperator);
    isOperator = planningProblem.TaskIsOperator("TestMethod");
    ASSERT_FALSE(isOperator);
}

TEST_F(PlanningProblemTest, GetMethodsForTask)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

    auto relevantMethods = planningProblem.GetMethodsForTask(topLevelTask, initialState);
    ASSERT_EQ(false, relevantMethods.empty());
    ASSERT_EQ(1, relevantMethods.size());
    ASSERT_EQ(topLevelTask.taskName, relevantMethods[0].task.taskName);

    tfd_cpp::Task task;
    task.taskName = "TestMethodFail";

    relevantMethods = planningProblem.GetMethodsForTask(task, initialState);
    ASSERT_EQ(true, relevantMethods.empty());
}

TEST_F(PlanningProblemTest, GetOperatorsForTask)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);

    auto applicableOperators = planningProblem.GetOperatorsForTask(topLevelTask, initialState);
    ASSERT_EQ(true, applicableOperators.empty());

    tfd_cpp::Task task;
    task.taskName = "TestOperator";
    applicableOperators = planningProblem.GetOperatorsForTask(task, initialState);
    ASSERT_EQ(false, applicableOperators.empty());
    ASSERT_EQ(1, applicableOperators.size());
    ASSERT_EQ(task.taskName, applicableOperators[0].task.taskName);
}

TEST_F(PlanningProblemTest, GetInitialState)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
    ASSERT_EQ(initialState.domainName, planningProblem.GetInitialState().domainName);
    ASSERT_EQ(std::any_cast<bool>(initialState.data), std::any_cast<bool>(planningProblem.GetInitialState().data));
}

TEST_F(PlanningProblemTest, GetTopLevelTask)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
    ASSERT_EQ(topLevelTask.taskName, planningProblem.GetTopLevelTask().taskName);
    ASSERT_EQ(topLevelTask.parameters.size(), planningProblem.GetTopLevelTask().parameters.size());
}
