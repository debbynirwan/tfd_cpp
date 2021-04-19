#include "simple_travel_problem.h"

namespace simple_travel {

    // initial state
    static const SimpleTravelState::PersonLocationTable s_initPersonLocationTable = {{"me", "home"}, {"taxi", "park"}};
    static const SimpleTravelState::PersonCashTable s_initPersonCashTable = {{"me", 20}};
    static const SimpleTravelState::PersonOweTable s_initPersonOweTable = {{"me", 0}};
    static const SimpleTravelState::DistanceTable s_initDistanceTable = {{"home", {{"park", 8}}}, {"park", {{"home", 8}}}};

    static const SimpleTravelState s_initialState(s_initPersonLocationTable,
                                                  s_initPersonCashTable,
                                                  s_initPersonOweTable,
                                                  s_initDistanceTable);

    tfd_cpp::PlanningProblem CreatePlanningProblem(const tfd_cpp::Task& topLevelTask)
    {
        auto planningDomain = CreatePlanningDomain();
        tfd_cpp::State state = {DOMAIN_NAME, s_initialState};

        return tfd_cpp::PlanningProblem(planningDomain, state, topLevelTask);
    }
}
