#include <rambunctious_recitation.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Rambunctious Recitation")
{
    SECTION("Parse Input")
    {
        auto const numbers = parseInput("0,3,6\n");
        CHECK(numbers == std::vector<int64_t>{ 0, 3, 6 });
    }

    SECTION("Play Game")
    {
        auto const numbers = parseInput("0,3,6\n");
        auto const game = playGame(numbers, 2020);
        REQUIRE(game.turns.size() == 2020);
        CHECK(game.turns[0] == 0);
        CHECK(game.turns[1] == 3);
        CHECK(game.turns[2] == 6);

        CHECK(game.turns[3] == 0);
        CHECK(game.turns[4] == 3);
        CHECK(game.turns[5] == 3);
        CHECK(game.turns[6] == 1);
        CHECK(game.turns[7] == 0);
        CHECK(game.turns[8] == 4);
        CHECK(game.turns[9] == 0);

        CHECK(game.turns.back() == 436);

        CHECK(playGame(std::vector<int64_t>{ 1, 3, 2 }, 2020).turns.back() == 1);
        CHECK(playGame(std::vector<int64_t>{ 2, 1, 3 }, 2020).turns.back() == 10);
        CHECK(playGame(std::vector<int64_t>{ 1, 2, 3 }, 2020).turns.back() == 27);
        CHECK(playGame(std::vector<int64_t>{ 2, 3, 1 }, 2020).turns.back() == 78);
        CHECK(playGame(std::vector<int64_t>{ 3, 2, 1 }, 2020).turns.back() == 438);
        CHECK(playGame(std::vector<int64_t>{ 3, 1, 2 }, 2020).turns.back() == 1836);
    }

    SECTION("Play Game Fast")
    {
        CHECK(playGameFast(std::vector<int64_t>{ 0, 3, 6 }, 30000000) == 175594);
        CHECK(playGameFast(std::vector<int64_t>{ 1, 3, 2 }, 30000000) == 2578);
        CHECK(playGameFast(std::vector<int64_t>{ 2, 1, 3 }, 30000000) == 3544142);
        CHECK(playGameFast(std::vector<int64_t>{ 1, 2, 3 }, 30000000) == 261214);
        CHECK(playGameFast(std::vector<int64_t>{ 2, 3, 1 }, 30000000) == 6895259);
        CHECK(playGameFast(std::vector<int64_t>{ 3, 2, 1 }, 30000000) == 18);
        CHECK(playGameFast(std::vector<int64_t>{ 3, 1, 2 }, 30000000) == 362);
    }
}
