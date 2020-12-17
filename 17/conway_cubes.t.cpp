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
        CHECK(w.min == Vec3{ 0, 0, 0 });
        CHECK(w.max == Vec3{ 3, 3, 1 });
        CHECK(w.map.contains(Vec3{ 1, 0, 0 }));
        CHECK(w.map.contains(Vec3{ 2, 1, 0 }));
        CHECK(w.map.contains(Vec3{ 0, 2, 0 }));
        CHECK(w.map.contains(Vec3{ 1, 2, 0 }));
        CHECK(w.map.contains(Vec3{ 2, 2, 0 }));
        CHECK(w.map.size() == 5);
    }

    SECTION("Compute Step")
    {
        World const w = parseInput(sample_input);
        World const w1 = step(w);
        CHECK(w1.map.size() == 11);
        World const w2 = step(w1);
        CHECK(w2.map.size() == 21);
        World const w3 = step(w2);
        CHECK(w3.map.size() == 38);
        World const w4 = step(w3);
        World const w5 = step(w4);
        World const w6 = step(w5);
        CHECK(w6.map.size() == 112);
    }

    SECTION("Boot Cycle")
    {
        World const w = parseInput(sample_input);
        CHECK(bootCycle(w) == 112);
    }
}
