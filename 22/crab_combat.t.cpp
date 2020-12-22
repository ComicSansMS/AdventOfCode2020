#include <crab_combat.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Crab Combat")
{
    char const sample_input[] = "Player 1:\n"
                                "9\n"
                                "2\n"
                                "6\n"
                                "3\n"
                                "1\n"
                                "\n"
                                "Player 2:\n"
                                "5\n"
                                "8\n"
                                "4\n"
                                "7\n"
                                "10\n";

    SECTION("Parse Input")
    {
        auto const game = parseInput(sample_input);
        CHECK(game.player1 == Deck{ 9, 2, 6, 3, 1 });
        CHECK(game.player2 == Deck{ 5, 8, 4, 7, 10 });
    }

    SECTION("Play Round")
    {
        auto const game = parseInput(sample_input);

        auto const r1 = playRound(game);
        CHECK(r1.player1 == Deck{ 2, 6, 3, 1, 9, 5});
        CHECK(r1.player2 == Deck{ 8, 4, 7, 10 });

        auto const r2 = playRound(r1);
        CHECK(r2.player1 == Deck{ 6, 3, 1, 9, 5 });
        CHECK(r2.player2 == Deck{ 4, 7, 10, 8, 2 });

        auto const r3 = playRound(r2);
        CHECK(r3.player1 == Deck{ 3, 1, 9, 5, 6, 4 });
        CHECK(r3.player2 == Deck{ 7, 10, 8, 2 });

        auto const r4 = playRound(r3);
        CHECK(r4.player1 == Deck{ 1, 9, 5, 6, 4 });
        CHECK(r4.player2 == Deck{ 10, 8, 2, 7, 3 });
    }

    SECTION("Play Game")
    {
        auto const game = parseInput(sample_input);
        auto const result = playGame(game);

        CHECK(result.g.player1.empty());
        CHECK(result.g.player2 == Deck{ 3, 2, 10, 6, 8, 5, 9, 4, 7, 1 });
        CHECK(result.score == 306);
    }

    SECTION("Play Game Recursive")
    {
        auto const game = parseInput(sample_input);
        auto const result = playGameRecursive(game);

        CHECK(result.g.player1.empty());
        CHECK(result.g.player2 == Deck{ 7, 5, 6, 2, 4, 1, 10, 8, 9, 3 });
        CHECK(result.score == 291);
    }
}
