#include <crab_cups.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Crab Cups")
{
    SECTION("Parse Input")
    {
        auto const cups = parseInput("32415");
        CHECK(cups == Cups{ 3, 2, 4, 1, 5 });
    }

    SECTION("Play Round")
    {
        auto const cups = parseInput("389125467");
        CHECK(cups == Cups{ 3, 8, 9, 1, 2, 5, 4, 6, 7 });

        auto const r1 = playRound(cups);
        CHECK(r1 == Cups{ 2, 8, 9, 1, 5, 4, 6, 7, 3 });
        auto const r2 = playRound(r1);
        CHECK(r2 == Cups{ 5, 4, 6, 7, 8, 9, 1, 3, 2 });
        auto const r3 = playRound(r2);
        CHECK(r3 == Cups{ 8, 9, 1, 3, 4, 6, 7, 2, 5 });
        auto const r4 = playRound(r3);
        CHECK(r4 == Cups{ 4, 6, 7, 9, 1, 3, 2, 5, 8 });
        auto const r5 = playRound(r4);
        CHECK(r5 == Cups{ 1, 3, 6, 7, 9, 2, 5, 8, 4 });
        auto const r6 = playRound(r5);
        CHECK(r6 == Cups{ 9, 3, 6, 7, 2, 5, 8, 4, 1 });
        auto const r7 = playRound(r6);
        CHECK(r7 == Cups{ 2, 5, 8, 3, 6, 7, 4, 1, 9 });
        auto const r8 = playRound(r7);
        CHECK(r8 == Cups{ 6, 7, 4, 1, 5, 8, 3, 9, 2 });
        auto const r9 = playRound(r8);
        CHECK(r9 == Cups{ 5, 7, 4, 1, 8, 3, 9, 2, 6 });
        CHECK(playRound(r9) == Cups{ 8, 3, 7, 4, 1, 9, 2, 6, 5 });
    }

    SECTION("Order")
    {
        auto cups = parseInput("389125467");
        CHECK(order(cups) == 25467389);
        for (int i = 0; i < 10; ++i) { cups = playRound(cups); }
        CHECK(order(cups) == 92658374);
        for (int i = 0; i < 90; ++i) { cups = playRound(cups); }
        CHECK(order(cups) == 67384529);
    }

    SECTION("Solve 1")
    {
        auto const cups = parseInput("389125467");
        CHECK(solve1(cups) == 67384529);
    }

    SECTION("Solve 2")
    {
        auto const cups = parseInput("389125467");
        auto const solved_cups = solve2(cups);
        CHECK(solved_cups[0] == 1);
        CHECK(solved_cups[1] == 934001);
        CHECK(solved_cups[2] == 159792);
    }
}
