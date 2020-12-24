#ifndef ADVENT_OF_CODE_24_LOBBY_LAYOUT_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_24_LOBBY_LAYOUT_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

enum class Direction {
    NorthEast = 0,
    East,
    SouthEast,
    SouthWest,
    West,
    NorthWest
};

using Path = std::vector<Direction>;

std::vector<Path> parseInput(std::string_view input);

struct AxialHexCoordinate {
    int q;
    int r;

    friend bool operator==(AxialHexCoordinate const&, AxialHexCoordinate const&) = default;
    friend bool operator<(AxialHexCoordinate const&, AxialHexCoordinate const&) = default;
};

namespace std {
template<> struct hash<AxialHexCoordinate>
{
    std::size_t operator()(AxialHexCoordinate const& c) const noexcept
    {
        return (static_cast<std::size_t>(c.q) << 32) + static_cast<std::size_t>(c.r);
    }
};
}

AxialHexCoordinate moveInDirection(AxialHexCoordinate c, Direction dir);

std::vector<AxialHexCoordinate> walkPaths(std::vector<Path> const& paths);

std::unordered_set<AxialHexCoordinate> determineFlippedTiles(std::vector<Path> const& paths);

int countFlippedTiles(std::vector<Path> const& paths);

std::unordered_set<AxialHexCoordinate> step(std::unordered_set<AxialHexCoordinate> const& tiles);

int simulate(std::vector<Path> const& paths);

#endif
