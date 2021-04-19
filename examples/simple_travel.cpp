#include "simple_travel_problem.h"
#include "tfd.h"
#include <iostream>

int main()
{
    std::cout << "Total-forward Decomposition Algorithm Demo: Simple Travel Problem" << std::endl;
    
    tfd_cpp::Task task;
    task.taskName = TRAVEL;
    task.parameters.push_back(simple_travel::SimpleTravelState::Object("me"));
    task.parameters.push_back(simple_travel::SimpleTravelState::Object("taxi"));
    task.parameters.push_back(simple_travel::SimpleTravelState::Location("home"));
    task.parameters.push_back(simple_travel::SimpleTravelState::Location("park"));
    
    tfd_cpp::TFD tfd(simple_travel::CreatePlanningProblem(task));
    tfd_cpp::TFD::Plan solutionPlan = tfd.TryToPlan();

    if (not solutionPlan.empty())
    {
        std::cout << "TFD found solution plan for Simple Travel Problem." << std::endl;
        for (const auto& _operator : solutionPlan)
        {
            std::cout <<  _operator.task.taskName << std::endl;
        }
    }
    else
    {
        std::cout << "TFD failed to plan for Simple Travel Problem." << std::endl;
    }

    return 0;
}