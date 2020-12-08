#ifndef ADVENT_OF_CODE_08_HANDHELD_HALTING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_08_HANDHELD_HALTING_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

struct Gameboy;

struct Nop {
    int64_t arg;
    void execute(Gameboy& gb) const;
};

struct Jmp {
    int64_t arg;
    void execute(Gameboy& gb) const;
};

struct Acc {
    int64_t arg;
    void execute(Gameboy& gb) const;
};

using Op = std::variant<Nop, Jmp, Acc>;

struct Gameboy {
    int64_t acc;
    int64_t pc;
    std::vector<Op> ops;
};

Gameboy parseInput(std::string_view input);

int64_t step(Gameboy& gb);

int64_t runUntilLoop(Gameboy& gb);

int64_t fixAndRun(Gameboy const& gb);


#endif
