#include <conway_cubes.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Ticket Translation")
{
    char const sample_input[] = R"(.#.
..#
###
)";

    SECTION("Parse Input")
    {
        World const w = parseInput(sample_input);
        CHECK(w.min == Vec4{ 0, 0, 0, 0 });
        CHECK(w.max == Vec4{ 3, 3, 1, 1 });
        CHECK(w.map.contains(Vec4{ 1, 0, 0, 0 }));
        CHECK(w.map.contains(Vec4{ 2, 1, 0, 0 }));
        CHECK(w.map.contains(Vec4{ 0, 2, 0, 0 }));
        CHECK(w.map.contains(Vec4{ 1, 2, 0, 0 }));
        CHECK(w.map.contains(Vec4{ 2, 2, 0, 0 }));
        CHECK(w.map.size() == 5);
    }

    SECTION("Compute Step")
    {
        World const w = parseInput(sample_input);
        World const w1 = step3(w);
        CHECK(w1.map.size() == 11);
        World const w2 = step3(w1);
        CHECK(w2.map.size() == 21);
        World const w3 = step3(w2);
        CHECK(w3.map.size() == 38);
        World const w4 = step3(w3);
        World const w5 = step3(w4);
        World const w6 = step3(w5);
        CHECK(w6.map.size() == 112);
    }

    SECTION("Boot Cycle")
    {
        World const w = parseInput(sample_input);
        CHECK(bootCycle3(w) == 112);
        CHECK(bootCycle4(w) == 848);
    }
}
