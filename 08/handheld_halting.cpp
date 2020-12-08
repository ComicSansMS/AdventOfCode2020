#include <handheld_halting.hpp>

#include <fmt/printf.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>


Gameboy parseInput(std::string_view input)
{
    Gameboy ret;
    ret.acc = 0;
    ret.pc = 0;
    std::regex rx_line(R"((\w\w\w) ([\+,\-])(\d+)[\r\n]?)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::transform(it_begin, it_end, std::back_inserter(ret.ops),
                   [](std::match_results<std::string_view::iterator> const& match) -> Op {
                       int64_t const arg = std::stoi(match[3]) * ((match[2] == "-") ? -1 : 1);
                       if (match[1] == "jmp") {
                           return Jmp{ arg };
                       } else if (match[1] == "acc") {
                           return Acc{ arg };
                       } else {
                           assert(match[1] == "nop");
                           return Nop{ arg };
                       }
                   });
    return ret;
}

void Nop::execute(Gameboy& gb) const
{
    ++gb.pc;
}

void Acc::execute(Gameboy& gb) const
{
    gb.acc += arg;
    ++gb.pc;
}

void Jmp::execute(Gameboy& gb) const
{
    gb.pc += arg;
}

int64_t step(Gameboy& gb)
{
    Op const& step_op = gb.ops[gb.pc];
    std::visit([&gb](auto op) { op.execute(gb); }, step_op);
    return gb.pc;
}

int64_t runUntilLoop(Gameboy& gb)
{
    std::vector<bool> visited;
    visited.resize(gb.ops.size(), false);

    while (!visited[gb.pc]) {
        visited[gb.pc] = true;
        if (step(gb) >= static_cast<int64_t>(gb.ops.size())) { return -1; }
    }

    return gb.acc;
}

int64_t fixAndRun(Gameboy const& gb)
{
    for (std::size_t i = 0; i < gb.ops.size(); ++i) {
        if (gb.ops[i].index() != 2) {
            Gameboy gbi = gb;
            gbi.acc = 0;
            gbi.pc = 0;
            if (gbi.ops[i].index() == 0) {
                auto const arg = std::get<Nop>(gbi.ops[i]).arg;
                gbi.ops[i] = Jmp{ arg };
            } else {
                auto const arg = std::get<Jmp>(gbi.ops[i]).arg;
                gbi.ops[i] = Nop{ arg };
            }
            if (runUntilLoop(gbi) == -1) {
                return gbi.acc;
            }
        }
    }
    return -1;
}
