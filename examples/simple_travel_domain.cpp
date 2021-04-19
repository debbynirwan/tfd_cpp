#include "simple_travel_domain.h"
#include <cassert>
#include <sstream>

namespace simple_travel
{
    SimpleTravelState::SimpleTravelState(const PersonLocationTable& personLocationTable,
                                         const PersonCashTable& personCashTable,
                                         const PersonOweTable& personOweTable,
                                         const DistanceTable& distanceTable) : 
        m_personLocationTable(personLocationTable),
        m_personCashTable(personCashTable),
        m_personOweTable(personOweTable),
        m_distanceTable(distanceTable)
    {
    }

    SimpleTravelState::~SimpleTravelState()
    {
    }

    std::optional<SimpleTravelState::Location> SimpleTravelState::LocationOf(const Object& person) const
    {
        if (m_personLocationTable.find(person) == m_personLocationTable.end())
        {
            return std::nullopt;
        }
        else
        {
            return m_personLocationTable.at(person);
        }
    }

    void SimpleTravelState::SetLocationOf(const Object& person, const Location& location)
    {
        if (m_personLocationTable.find(person) != m_personLocationTable.end())
        {
            m_personLocationTable[person] = location;
        }
    }

    std::optional<SimpleTravelState::Cash> SimpleTravelState::CashOwnedBy(const Object& person) const
    {
        if (m_personCashTable.find(person) == m_personCashTable.end())
        {
            return std::nullopt;
        }
        else
        {
            return m_personCashTable.at(person);
        }
    }

    void SimpleTravelState::SetCashOwnedBy(const Object& person, const Cash& cash)
    {
        if (m_personCashTable.find(person) != m_personCashTable.end())
        {
            m_personCashTable[person] = cash;
        }
    }

    std::optional<SimpleTravelState::Cash> SimpleTravelState::Owe(const Object& person) const
    {
        if (m_personOweTable.find(person) == m_personOweTable.end())
        {
            return std::nullopt;
        }
        else
        {
            return m_personOweTable.at(person);
        }
    }

    void SimpleTravelState::SetOwe(const Object& person, const Cash& cash)
    {
        if (m_personOweTable.find(person) != m_personOweTable.end())
        {
            m_personOweTable[person] = cash;
        }
    }
    
    std::optional<SimpleTravelState::Distance> SimpleTravelState::DistanceBetween(const Location& location1, const Location& location2) const
    {
        if (m_distanceTable.find(location1) == m_distanceTable.end())
        {
            return std::nullopt;
        }
        else
        {
            const auto& destination_table = m_distanceTable.at(location1);
            if (destination_table.find(location2) == destination_table.end())
            {
                return std::nullopt;
            }
            else
            {
                return destination_table.at(location2);
            }
        }
    }

    SimpleTravelState::Cash SimpleTravelState::TaxiRate(const Distance& distance) const
    {
        return (1.5 + 0.5 * distance);
    }

    // Operators
    std::optional<tfd_cpp::State> Walk(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        assert(parameters.size() == 3);
        assert(state.domainName == DOMAIN_NAME);

        try 
        {
            const auto& person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
            const auto& src = std::any_cast<SimpleTravelState::Location>(parameters[1]);
            const auto& dst = std::any_cast<SimpleTravelState::Location>(parameters[2]);
            auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

            auto currentLocation = simpleTravelState.LocationOf(person); 
            if (currentLocation && currentLocation.value() == src)
            {
                tfd_cpp::State newState(state);
                simpleTravelState.SetLocationOf(person, dst);
                newState.data = simpleTravelState;
                
                return newState;
            }
        }
        catch(const std::bad_any_cast& e) 
        {
            std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
        }

        return std::nullopt;
    }

    std::optional<tfd_cpp::State> CallTaxi(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        assert(parameters.size() == 2);
        assert(state.domainName == DOMAIN_NAME);

        try
        {
            tfd_cpp::State newState(state);
            auto simpleTravelState = std::any_cast<SimpleTravelState>(newState.data);
            const auto& person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
            const auto& taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);

            auto personLocation = simpleTravelState.LocationOf(person);
            if (personLocation)
            {
                simpleTravelState.SetLocationOf(taxi, personLocation.value());
            }
            newState.data = simpleTravelState;
            
            return newState;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
        }

        return std::nullopt;
    }

    std::optional<tfd_cpp::State> RideTaxi(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        assert(parameters.size() == 4);
        assert(state.domainName == DOMAIN_NAME);

        try
        {
            const auto& person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
            const auto& taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
            const auto& src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
            const auto& dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
            auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

            auto currentPersonLocation = simpleTravelState.LocationOf(person);
            auto currentTaxiLocation = simpleTravelState.LocationOf(taxi);

            if (currentPersonLocation and currentTaxiLocation)
            {
                if ((currentPersonLocation.value() == src) and (currentTaxiLocation.value() == src))
                {
                    tfd_cpp::State newState(state);
                    auto distance = simpleTravelState.DistanceBetween(src, dst);
                    
                    if (distance)
                    {
                        simpleTravelState.SetLocationOf(person, dst);
                        simpleTravelState.SetLocationOf(taxi, dst);
                        simpleTravelState.SetOwe(person, simpleTravelState.TaxiRate(distance.value()));
                        newState.data = simpleTravelState;
                    }

                    return newState;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
        }

        return std::nullopt;
    }

    std::optional<tfd_cpp::State> PayDriver(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        assert(parameters.size() == 1);
        assert(state.domainName == DOMAIN_NAME);

        try
        {
            const auto& person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
            auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

            auto cashOwned = simpleTravelState.CashOwnedBy(person);
            auto owe = simpleTravelState.Owe(person);

            if (cashOwned and owe)
            {
                if (cashOwned.value() >= owe.value())
                {
                    tfd_cpp::State newState(state);
                    
                    simpleTravelState.SetCashOwnedBy(person, cashOwned.value() - owe.value());
                    simpleTravelState.SetOwe(person, 0);
                    newState.data = state;

                    return newState;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
        }

        return std::nullopt;
    }

    // Methods
    std::optional<std::vector<tfd_cpp::Task>> TravelByFoot(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        assert(parameters.size() == 4);
        assert(state.domainName == DOMAIN_NAME);

        try
        {
            const auto& person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
            const auto& taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
            const auto& src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
            const auto& dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
            auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

            auto distance = simpleTravelState.DistanceBetween(src, dst);
            if (distance && distance.value() <= WALKING_DISTANCE)
            {
                tfd_cpp::Task task;
                task.taskName = WALK;
                task.parameters.push_back(person);
                task.parameters.push_back(src);
                task.parameters.push_back(dst);
                std::vector<tfd_cpp::Task> subtasks{task};

                return subtasks;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
        }
        
        return std::nullopt;
    }

    std::optional<std::vector<tfd_cpp::Task>> TravelByTaxi(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters)
    {
        assert(parameters.size() == 4);
        assert(state.domainName == DOMAIN_NAME);

        try
        {
            const auto& person = std::any_cast<SimpleTravelState::Object>(parameters[0]);
            const auto& taxi = std::any_cast<SimpleTravelState::Object>(parameters[1]);
            const auto& src = std::any_cast<SimpleTravelState::Location>(parameters[2]);
            const auto& dst = std::any_cast<SimpleTravelState::Location>(parameters[3]);
            auto simpleTravelState = std::any_cast<SimpleTravelState>(state.data);

            auto cash = simpleTravelState.CashOwnedBy(person);
            auto distance = simpleTravelState.DistanceBetween(src, dst);
            auto currentPersonLocation = simpleTravelState.LocationOf(person);
            if (cash and distance and currentPersonLocation)
            {
                if (currentPersonLocation.value() == src)
                {
                    if (cash.value() >= simpleTravelState.TaxiRate(distance.value()))
                    {
                        tfd_cpp::Task callTaxi;
                        callTaxi.taskName = CALL_TAXI;
                        callTaxi.parameters.push_back(person);
                        callTaxi.parameters.push_back(taxi);

                        tfd_cpp::Task rideTaxi;
                        rideTaxi.taskName = RIDE_TAXI;
                        rideTaxi.parameters.push_back(person);
                        rideTaxi.parameters.push_back(taxi);
                        rideTaxi.parameters.push_back(src);
                        rideTaxi.parameters.push_back(dst);

                        tfd_cpp::Task payDriver;
                        payDriver.taskName = PAY_DRIVER;
                        payDriver.parameters.push_back(person);

                        std::vector<tfd_cpp::Task> subtasks{payDriver, rideTaxi, callTaxi};
                        
                        return subtasks;
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " in " << __PRETTY_FUNCTION__ << '\n';
        }
        
        return std::nullopt;
    }

    tfd_cpp::PlanningDomain CreatePlanningDomain()
    {
        tfd_cpp::PlanningDomain planningDomain(DOMAIN_NAME);

        // Add operators
        planningDomain.AddOperator(WALK, Walk);
        planningDomain.AddOperator(CALL_TAXI, CallTaxi);
        planningDomain.AddOperator(RIDE_TAXI, RideTaxi);
        planningDomain.AddOperator(PAY_DRIVER, PayDriver);

        // Add methods
        planningDomain.AddMethod(TRAVEL, TravelByFoot);
        planningDomain.AddMethod(TRAVEL, TravelByTaxi);

        return planningDomain;
    }

    std::ostream& operator<<(std::ostream& os, const SimpleTravelState& state)
    {
        os << "SimpleTravelState";
        return os;
    }
}
