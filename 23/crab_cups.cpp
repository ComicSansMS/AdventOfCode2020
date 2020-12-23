#include <crab_cups.hpp>

#include <fmt/printf.h>

#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>

Cups parseInput(std::string_view input)
{
    Cups ret;
    for (auto const& c : input) {
        if(c == '\n') { break; }
        assert((c >= '0') && (c <= '9'));
        ret.push_back(c - '0');
    }
    return ret;
}

Cups playRound(Cups c)
{
    auto const n_cups = c.size();
    std::array<int, 3> picked_up{ c[1], c[2], c[3] };
    c.erase(c.begin() + 1, c.begin() + 4);

    int destination_label = c[0] - 1;
    auto it_dest = end(c);
    for (; it_dest == end(c); --destination_label) {
        if (destination_label <= 0) { destination_label += static_cast<int>(n_cups); }
        it_dest = std::find(begin(c), end(c), destination_label);
    }
    assert(it_dest != end(c));

    c.insert(it_dest + 1, begin(picked_up), end(picked_up));
    std::rotate(begin(c), begin(c) + 1, end(c));

    return c;
}

int64_t order(Cups c)
{
    std::rotate(begin(c), std::find(begin(c), end(c), 1), end(c));
    int64_t acc = 0;
    for (auto it = begin(c) + 1, it_end = end(c); it != it_end; ++it) {
        auto const& cup = *it;
        acc *= 10;
        acc += cup;
    }
    return acc;
}

int64_t solve1(Cups const& c)
{
    Cups it = c;
    for (int i = 0; i < 100; ++i) { it = playRound(it); }
    return order(it);
}

Cups solve2(Cups const& cups_small)
{
    Cups c = cups_small;
    for (int i = *std::max_element(begin(c), end(c)); i < 1'000'000; ++i) {
        c.push_back(i + 1);
    }

    int i_prnt = 0;
    for (int i = 0; i < 10'000'000; ++i) {
        if (i_prnt++ == 100'000) { fmt::print("."); i_prnt = 0; }
        c = playRound(std::move(c));
    }

    std::rotate(begin(c), std::find(begin(c), end(c), 1), end(c));

    return c;
}
