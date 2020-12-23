#ifndef ADVENT_OF_CODE_23_CRAB_CUPS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_23_CRAB_CUPS_HPP_INCLUDE_GUARD

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

using Cups = std::vector<int>;

Cups parseInput(std::string_view input);

Cups playRound(Cups c);

int64_t order(Cups c);

int64_t solve1(Cups const& c);

Cups solve2(Cups const& cups_small);

#endif
