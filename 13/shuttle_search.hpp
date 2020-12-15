#ifndef ADVENT_OF_CODE_13_SHUTTLE_SEARCH_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_13_SHUTTLE_SEARCH_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

struct Timetable {
    int64_t arrival_time;
    std::vector<int64_t> departure_intervals;
};

Timetable parseInput(std::string_view input);

struct Result {
    int64_t bus_id;
    int64_t departure;
    int64_t answer;
};

Result findFirstBus(Timetable const& timetable);

void modChain(Timetable const& timetable);

int64_t solve(Timetable const& timetable);

#endif
