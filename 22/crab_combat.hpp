#ifndef ADVENT_OF_CODE_22_CRAB_COMBAT_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_22_CRAB_COMBAT_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using Card = std::int64_t;

using Deck = std::deque<Card>;

struct Game {
    Deck player1;
    Deck player2;

    friend bool operator==(Game const&, Game const&) = default;
};

Game parseInput(std::string_view input);

Game playRound(Game g);

struct Result {
    Game g;
    int64_t score;
};

Result playGame(Game g);

Result playGameRecursive(Game const& base_game);

#endif
