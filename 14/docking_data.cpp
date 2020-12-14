#include <docking_data.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>


std::vector<Instruction> parseInput(std::string_view input)
{
    std::vector<Instruction> ret;
    std::regex rx_line(R"((mask|mem)(\[(\d+)\])? = ([0-9X]+)[\r\n])");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Instruction {
            if(match[1] == "mask") {
                auto const mask_str = match[4];
                auto const dist = std::distance(mask_str.first, mask_str.second);
                assert(std::distance(mask_str.first, mask_str.second) == 36);
                Mask mask{};
                for(std::size_t i = 0; i < 36; ++i) {
                    char const c = *(mask_str.first + (35 - i));
                    if (c == '0') {
                        mask.mask_0.set(i);
                    } else if (c == '1') {
                        mask.mask_1.set(i);
                    } else {
                        assert(c == 'X');
                    }
                }
                return mask;
            } else {
                assert(match[1] == "mem");
                Mov mov{};
                auto res = std::from_chars(&(*match[3].first), &(*match[3].second), mov.address);
                assert(res.ec == std::errc{});
                res = std::from_chars(&(*match[4].first), &(*match[4].second), mov.value);
                assert(res.ec == std::errc{});
                return mov;
            }
    });
    return ret;
}

void execute(Mask const& mask, Memory& mem)
{
    mem.current_mask = mask;
}

void execute(Mov const& mov, Memory& mem)
{
    auto it = mem.memory.find(mov.address);
    if (it == end(mem.memory)) {
        auto const [it_ret, b_ret] = mem.memory.insert(std::make_pair(mov.address, 0));
        assert(b_ret);
        it = it_ret;
    }
    assert(it != end(mem.memory));

    std::bitset<36>& memory_cell = it->second;
    std::bitset<36> value = mov.value;
    value |= mem.current_mask.mask_1;
    value &= (~mem.current_mask.mask_0);
    memory_cell = value;
}


Memory runProgram(std::vector<Instruction> const& instructions)
{
    Memory mem{};

    for(auto const& instruction : instructions) {
        std::visit([&mem](auto&& op) { execute(op, mem); }, instruction);
    }

    mem.sum_of_values = std::accumulate(begin(mem.memory), end(mem.memory), int64_t(0),
        [](int64_t acc, auto const& p) -> int64_t {
            return acc + static_cast<int64_t>(p.second.to_ullong());
        });

    return mem;
}
