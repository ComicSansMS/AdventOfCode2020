#ifndef ADVENT_OF_CODE_10_ADAPTER_ARRAY_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_ADAPTER_ARRAY_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

std::vector<int> parseInput(std::string_view input);

struct AdapterChain {
    int builtin_joltage;
    int jolt_diff_1;
    int jolt_diff_3;
};

AdapterChain chainAdapters(std::vector<int> const& adapters);

int64_t countValidChains(std::vector<int> const& adapters);

#endif
