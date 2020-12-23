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
#include <forward_list>
#include <iterator>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <memory_resource>

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

std::pmr::forward_list<int> pick_up(std::pmr::forward_list<int>& c, std::pmr::forward_list<int>::iterator it_first)
{
    auto it_end = it_first;
    int i = 0;
    for (; i < 4; ++i) {
        ++it_end;
        if (it_end == end(c)) { break; }
    }
    std::pmr::forward_list<int> picked_elements(c.get_allocator());
    picked_elements.splice_after(picked_elements.before_begin(), c, it_first, it_end);
    if (i != 4) {
        auto it_begin = c.before_begin();
        it_end = it_begin;
        for (; i < 4; ++i) {
            ++it_end;
        }
        auto it_dest = (picked_elements.empty()) ? picked_elements.before_begin() : picked_elements.begin();
        for (auto it = it_dest; it != end(picked_elements); ++it) {
            it_dest = it;
        }
        picked_elements.splice_after(it_dest, c, it_begin, it_end);
    }
    return picked_elements;
}

void playRound(std::pmr::forward_list<int>& c, std::pmr::forward_list<int>::iterator& it_first,
               std::vector<std::pmr::forward_list<int>::iterator> const& direct_iterators, std::size_t n_cups)
{
    std::pmr::forward_list<int> picked_up = pick_up(c, it_first);

    int destination_label = *it_first - 1;
    if (destination_label <= 0) { destination_label += static_cast<int>(n_cups); }
    while (std::find(begin(picked_up), end(picked_up), destination_label) != end(picked_up)) {
        --destination_label;
        if (destination_label <= 0) { destination_label += static_cast<int>(n_cups); }
    }
    auto it_dest = direct_iterators[destination_label];
    assert(it_dest != end(c));

    c.splice_after(it_dest, picked_up);
    ++it_first;
    if (it_first == end(c)) { it_first = begin(c); }
}

Cups playRound(Cups c)
{
    auto const n_cups = c.size();
    std::pmr::forward_list<int> cl(begin(c), end(c));
    std::vector<std::forward_list<int>::iterator> direct_iter(n_cups + 1);
    for (std::forward_list<int>::iterator it = begin(cl), it_end = end(cl); it != it_end; ++it) {
        direct_iter[*it] = it;
    }
    std::pmr::forward_list<int>::iterator it_first = cl.begin();
    playRound(cl, it_first, direct_iter, n_cups);

    Cups ret{ cl.begin(), cl.end() };
    std::rotate(begin(ret), begin(ret) + 1, end(ret));
    return ret;
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

int64_t solve1(Cups const& cups)
{
    std::pmr::monotonic_buffer_resource mrc(1'000'000 * 24);
    std::pmr::forward_list<int> c(begin(cups), end(cups), &mrc);
    std::vector<std::pmr::forward_list<int>::iterator> direct_iterators(1'000'001);
    for (std::forward_list<int>::iterator it = begin(c), it_end = end(c); it != it_end; ++it) {
        direct_iterators[*it] = it;
    }

    auto it = begin(c);
    for (int i = 0; i < 100; ++i) {
        playRound(c, it, direct_iterators, cups.size());
    }

    Cups ret(begin(c), end(c));
    std::rotate(begin(ret), std::find(begin(ret), end(ret), 1), end(ret));
    return order(ret);
}

Cups play10Mill(Cups const& cups_small)
{
    std::pmr::monotonic_buffer_resource mrc(1'000'000 * 24);
    std::pmr::forward_list<int> c(begin(cups_small), end(cups_small), &mrc);
    std::vector<std::pmr::forward_list<int>::iterator> direct_iterators(1'000'001);
    {
        auto it_end = begin(c);
        for (auto it = it_end; it != end(c); ++it) {
            it_end = it;
        }
        int const biggest_cup = *std::max_element(begin(cups_small), end(cups_small));
        it_end = c.insert_after(it_end, biggest_cup + 1);
        for (int i = biggest_cup + 1; i < 1'000'000; ++i) {
            it_end = c.insert_after(it_end, i + 1);
        }
    }
    for (std::forward_list<int>::iterator it = begin(c), it_end = end(c); it != it_end; ++it) {
        direct_iterators[*it] = it;
    }

    auto it = begin(c);
    for (int i = 0; i < 10'000'000; ++i) {
        playRound(c, it, direct_iterators, 1'000'000);
    }

    Cups ret(begin(c), end(c));
    std::rotate(begin(ret), std::find(begin(ret), end(ret), 1), end(ret));
    return ret;
}

int64_t solve2(Cups const& c)
{
    auto const res = play10Mill(c);
    assert(res[0] == 1);
    return static_cast<int64_t>(res[1]) * static_cast<int64_t>(res[2]);
}
