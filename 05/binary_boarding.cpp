#include <binary_boarding.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4459)
#endif
#include <range/v3/action/sort.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <limits>
#include <regex>
#include <sstream>
#include <string>

std::vector<std::string> parseInput(std::string_view input)
{
    std::vector<std::string> ret;
    std::stringstream sstr{ std::string{input} };
    std::string line;
    while (std::getline(sstr, line)) {
        assert(line.size() == 10);
        assert(std::all_of(line.begin(), line.begin() + 7, [](char c) { return (c == 'F') || (c == 'B'); }));
        assert(std::all_of(line.begin() + 7, line.end(), [](char c) { return (c == 'L') || (c == 'R'); }));
        ret.emplace_back(std::move(line));
    }
    return ret;
}

Seat computeSeat(std::string const& boarding_pass)
{
    assert(boarding_pass.size() == 10);
    Seat ret;
    int min_row = 0;
    int max_row = 128;
    for (int i = 0; i < 7; ++i) {
        char const c = boarding_pass[i];
        int const half_range = (max_row - min_row) / 2;
        if (c == 'F') {
            max_row -= half_range;
        } else {
            assert(c == 'B');
            min_row += half_range;
        }
    }
    ret.row = min_row;

    int min_col = 0;
    int max_col = 8;
    for (int i = 0; i < 3; ++i) {
        char const c = boarding_pass[i + 7];
        int const half_range = (max_col - min_col) / 2;
        if (c == 'L') {
            max_col -= half_range;
        } else {
            assert(c == 'R');
            min_col += half_range;
        }
    }
    ret.column = min_col;
    return ret;
}

int computeSeatId(Seat const& seat)
{
    return (seat.row * 8) + seat.column;
}

int highestSeatId(std::vector<std::string> const& boarding_passes)
{
    auto const rng = boarding_passes |
            ranges::views::transform(computeSeat) |
            ranges::views::transform(computeSeatId);
    return *ranges::max_element(rng);
}

int findFreeSeat(std::vector<std::string> const& boarding_passes)
{
    auto const rng = boarding_passes |
                     ranges::views::transform(computeSeat) |
                     ranges::views::transform(computeSeatId) | ranges::to<std::vector<int>> |
                     ranges::actions::sort;
    return *std::adjacent_find(begin(rng), end(rng), [](int id1, int id2) -> bool { return (id1 + 1) != id2; }) + 1;
}
