#include <shuttle_search.hpp>

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


Timetable parseInput(std::string_view input)
{
    Timetable ret;
    std::string_view const line1 = input.substr(0, input.find("\n"));
    std::string_view const line2 = input.substr(line1.size() + 1);
    auto res = std::from_chars(line1.data(), line1.data() + line1.size(), ret.arrival_time);
    assert(res.ec == std::errc{});

    std::size_t it_offset = 0;
    while (it_offset < line2.size()) {
        auto end_of_entry = line2.find(",", it_offset);
        if (end_of_entry == std::string_view::npos) {
            end_of_entry = line2.find("\n");
            assert(end_of_entry != std::string_view::npos);
        }
        std::string_view entry = line2.substr(it_offset, end_of_entry - it_offset);
        int64_t t;
        if (entry == "x") {
            t = -1;
        } else {
            std::from_chars(line2.data() + it_offset, line2.data() + end_of_entry, t);
        }
        ret.departure_intervals.push_back(t);
        it_offset = end_of_entry + 1;
    }
    return ret;
}

Result findFirstBus(Timetable const& timetable)
{
    Result ret{};
    std::vector<int64_t> wait = timetable.departure_intervals;
    for (auto& t : wait) {
        if (t == -1) {
            t = std::numeric_limits<int64_t>::max();
        } else {
            t = t - (timetable.arrival_time % t);
        }
    }
    auto it_shortest_wait = std::min_element(begin(wait), end(wait));
    std::size_t index_shortest_wait = std::distance(begin(wait), it_shortest_wait);
    ret.bus_id = timetable.departure_intervals[index_shortest_wait];
    ret.departure = timetable.arrival_time + *it_shortest_wait;
    ret.answer = ret.bus_id * (ret.departure - timetable.arrival_time);
    return ret;
}

void modChain(Timetable const& timetable)
{
    int64_t chain_size = timetable.departure_intervals.front();
    std::vector<std::vector<int64_t>> chains;
    chains.reserve(timetable.departure_intervals.size());

    for (int in = 0; in < timetable.departure_intervals.size(); ++in) {
        chains.emplace_back();
        chains.back().reserve(chain_size);
        int64_t const n = timetable.departure_intervals[in];
        for (int id = 0; id < chain_size; ++id) {
            chains.back().push_back(((id + 1) * n) % timetable.departure_intervals.front());
        }
    }
    chains.emplace_back();
}
