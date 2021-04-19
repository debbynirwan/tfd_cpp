#include "planning_domain.h"
#include "gtest/gtest.h"
#include <optional>
#include <any>
#include <functional>

struct PlanningDomainTest : public ::testing::Test
{
    PlanningDomainTest() : 
        internalState(false),
        planningDomain("TestDomain") {}

    ~PlanningDomainTest() {}

    bool internalState;
    tfd_cpp::PlanningDomain planningDomain;

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
};

TEST_F(PlanningDomainTest, EmptyOperatorTable)
{
    tfd_cpp::State state;
    tfd_cpp::Task task;
    task.taskName = "TestOperator";
    auto applicableOperators = planningDomain.GetApplicableOperators(state, task);

    ASSERT_EQ(std::nullopt, applicableOperators);
}

TEST_F(PlanningDomainTest, EmptyMethodTable)
{
    tfd_cpp::State state;
    tfd_cpp::Task task;
    task.taskName = "TestMethod";
    auto relevantMethods = planningDomain.GetRelevantMethods(state, task);

    ASSERT_EQ(std::nullopt, relevantMethods);
}

TEST_F(PlanningDomainTest, GetOperatorSucceed)
{
    tfd_cpp::State state;
    state.domainName = "TestDomain";
    state.data = internalState;

    tfd_cpp::Task task;
    task.taskName = "TestOperator";
    
    planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
    auto applicableOperators = planningDomain.GetApplicableOperators(state, task);

    ASSERT_TRUE(applicableOperators);
    ASSERT_EQ(false, applicableOperators.value().empty());
    ASSERT_EQ(1, applicableOperators.value().size());
    ASSERT_EQ(task.taskName, applicableOperators.value()[0].task.taskName);
}

TEST_F(PlanningDomainTest, GetOperatorWrongTaskName)
{
    tfd_cpp::State state;
    state.domainName = "TestDomain";
    state.data = internalState;

    tfd_cpp::Task task;
    task.taskName = "TestOperatorFail";
    
    planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
    auto applicableOperators = planningDomain.GetApplicableOperators(state, task);

    ASSERT_TRUE(applicableOperators);
    ASSERT_EQ(true, applicableOperators.value().empty());
}

TEST_F(PlanningDomainTest, GetMethodSucceed)
{
    tfd_cpp::State state;
    state.domainName = "TestDomain";
    state.data = internalState;

    tfd_cpp::Task task;
    task.taskName = "TestMethod";
    
    planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
    auto relevantMethods = planningDomain.GetRelevantMethods(state, task);

    ASSERT_TRUE(relevantMethods);
    ASSERT_EQ(false, relevantMethods.value().empty());
    ASSERT_EQ(1, relevantMethods.value().size());
    ASSERT_EQ(task.taskName, relevantMethods.value()[0].task.taskName);
}

TEST_F(PlanningDomainTest, GetMethodWrongTaskName)
{
    tfd_cpp::State state;
    state.domainName = "TestDomain";
    state.data = internalState;

    tfd_cpp::Task task;
    task.taskName = "TestMethodFail";
    
    planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
    auto relevantMethods = planningDomain.GetRelevantMethods(state, task);

    ASSERT_TRUE(relevantMethods);
    ASSERT_EQ(true, relevantMethods.value().empty());
}

TEST_F(PlanningDomainTest, TaskIsOperator)
{
    tfd_cpp::State state;
    state.domainName = "TestDomain";
    state.data = internalState;

    tfd_cpp::Task task;
    task.taskName = "TestOperator";
    
    planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
    planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
    auto isOperator = planningDomain.TaskIsOperator("TestOperator");
    ASSERT_TRUE(isOperator);
    isOperator = planningDomain.TaskIsOperator("TestMethod");
    ASSERT_FALSE(isOperator);
}

TEST_F(PlanningDomainTest, TaskIsMethod)
{
    tfd_cpp::State state;
    state.domainName = "TestDomain";
    state.data = internalState;

    tfd_cpp::Task task;
    task.taskName = "TestOperator";
    
    planningDomain.AddOperator("TestOperator", std::bind(&PlanningDomainTest::Operator, this, std::placeholders::_1, std::placeholders::_2));
    planningDomain.AddMethod("TestMethod", std::bind(&PlanningDomainTest::Method, this, std::placeholders::_1, std::placeholders::_2));
    auto isMethod = planningDomain.TaskIsMethod("TestMethod");
    ASSERT_TRUE(isMethod);
    isMethod = planningDomain.TaskIsMethod("TestOperator");
    ASSERT_FALSE(isMethod);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
