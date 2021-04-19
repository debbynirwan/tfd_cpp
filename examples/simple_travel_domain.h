// Simple Travel Planning Domain
#pragma once

#include "planning_domain.h"
#include <functional>

namespace simple_travel
{
    #define DOMAIN_NAME         "simple_travel"
    #define WALKING_DISTANCE    2

    // Tasks
    #define TRAVEL              "Travel"
    #define WALK                "Walk"
    #define CALL_TAXI           "CallTaxi"
    #define RIDE_TAXI           "RideTaxi"
    #define PAY_DRIVER          "PayDriver"

    class SimpleTravelState
    {
    public:

        using Object = std::string;
        using Location = std::string;
        using Cash = std::size_t;
        using Distance = std::size_t;
        
        using PersonLocationTable = std::map<Object, Location>;
        using PersonCashTable = std::map<Object, Cash>;
        using PersonOweTable = std::map<Object, Cash>;
        using DistanceTable = std::map<Location, std::map<Location, Distance>>;

        SimpleTravelState(const PersonLocationTable& personLocationTable,
                          const PersonCashTable& personCashTable,
                          const PersonOweTable& personOweTable,
                          const DistanceTable& distanceTable);
        virtual ~SimpleTravelState();
        
        std::optional<Location> LocationOf(const Object& person) const;
        std::optional<Cash> CashOwnedBy(const Object& person) const;
        std::optional<Cash> Owe(const Object& person) const;
        std::optional<Distance> DistanceBetween(const Location& location1, const Location& location2) const;

        void SetLocationOf(const Object& person, const Location& location);
        void SetCashOwnedBy(const Object& person, const Cash& cash);
        void SetOwe(const Object& person, const Cash& cash);

        Cash TaxiRate(const Distance& distance) const;

    private:

        friend std::ostream& operator<<(std::ostream& os, const SimpleTravelState& state);
        
        PersonLocationTable m_personLocationTable;
        PersonCashTable m_personCashTable;
        PersonOweTable m_personOweTable;
        DistanceTable m_distanceTable;
    };

    // Operators
    std::optional<tfd_cpp::State> Walk(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters);
    std::optional<tfd_cpp::State> CallTaxi(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters);
    std::optional<tfd_cpp::State> RideTaxi(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters);
    std::optional<tfd_cpp::State> PayDriver(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters);

    // Methods
    std::optional<std::vector<tfd_cpp::Task>> TravelByFoot(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters);
    std::optional<std::vector<tfd_cpp::Task>> TravelByTaxi(const tfd_cpp::State& state, const tfd_cpp::Parameters& parameters);

    tfd_cpp::PlanningDomain CreatePlanningDomain();

    std::ostream& operator<<(std::ostream& os, const SimpleTravelState& state);
}
