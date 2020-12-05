#ifndef ADVENT_OF_CODE_05_BINARY_BOARDING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_05_BINARY_BOARDING_HPP_INCLUDE_GUARD

#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

std::vector<std::string> parseInput(std::string_view input);

struct Seat {
    int row;
    int column;
};

Seat computeSeat(std::string const& boarding_pass);

int computeSeatId(Seat const& seat);

int highestSeatId(std::vector<std::string> const& boarding_passes);

int findFreeSeat(std::vector<std::string> const& boarding_passes);

#endif
