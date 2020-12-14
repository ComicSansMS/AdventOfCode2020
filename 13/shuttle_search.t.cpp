#include <shuttle_search.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Shuttle Search")
{
    char const sample_input[] = "939\n7,13,x,x,59,x,31,19\n";

    SECTION("Parse Input")
    {
        auto const timetable = parseInput(sample_input);
        CHECK(timetable.arrival_time == 939);
        CHECK(timetable.departure_intervals == std::vector<int64_t>{ 7, 13, -1, -1, 59, -1, 31, 19 });
    }

    SECTION("Find First Bus")
    {
        auto const r = findFirstBus(parseInput(sample_input));
        CHECK(r.bus_id == 59);
        CHECK(r.departure == 944);
        CHECK(r.answer == 295);
    }

    SECTION("Mod Chain")
    {
        modChain(parseInput(sample_input));
    }
}
