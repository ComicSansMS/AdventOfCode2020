#ifndef ADVENT_OF_CODE_14_DOCKING_DATA_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_DOCKING_DATA_HPP_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

struct Mask {
    std::bitset<36> mask_0;
    std::bitset<36> mask_1;
};

struct Mov {
    int64_t address;
    int64_t value;
};

using Instruction = std::variant<Mask, Mov>;

std::vector<Instruction> parseInput(std::string_view input);

struct Memory {
    Mask current_mask;
    std::unordered_map<int64_t, std::bitset<36>> memory;
    int64_t sum_of_values;
};

Memory runProgram(std::vector<Instruction> const& instructions);

#endif
