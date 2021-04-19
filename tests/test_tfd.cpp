#include "tfd.h"
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
        task.parameters.push_back(true);
        std::vector<tfd_cpp::Task> subtasks{task};

        return subtasks;
    }
}

struct TFDTest : public ::testing::Test
{
    TFDTest() : 
        planningDomain("TestDomain"),
        initialState{"TestDomain", false},
        topLevelTask{"TestMethod", {}}
    {
    }

    ~TFDTest() {}

    tfd_cpp::PlanningDomain planningDomain;
    tfd_cpp::State initialState; 
    tfd_cpp::Task topLevelTask;
};

TEST_F(TFDTest, TryToPlanSucceed)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, topLevelTask);
    tfd_cpp::TFD tfd(planningProblem);

    auto solutionPlan = tfd.TryToPlan();

    // check operator
    ASSERT_FALSE(solutionPlan.empty());
    ASSERT_EQ(1, solutionPlan.size());
    ASSERT_EQ("TestOperator", solutionPlan[0].task.taskName);

    // check parameter
    ASSERT_EQ(1, solutionPlan[0].task.parameters.size());
    ASSERT_NO_THROW(std::any_cast<bool>(solutionPlan[0].task.parameters[0]));
    ASSERT_EQ(true, std::any_cast<bool>(solutionPlan[0].task.parameters[0]));
}

TEST_F(TFDTest, TryToPlanFail)
{
    planningDomain.AddOperator("TestOperator", Operator);
    planningDomain.AddMethod("TestMethod", Method);

    tfd_cpp::Task task;
    task.taskName = "Random";

    tfd_cpp::PlanningProblem planningProblem(planningDomain, initialState, task);
    tfd_cpp::TFD tfd(planningProblem);

    auto solutionPlan = tfd.TryToPlan();
    ASSERT_TRUE(solutionPlan.empty());
}
