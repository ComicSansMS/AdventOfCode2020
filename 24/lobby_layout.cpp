#include <lobby_layout.hpp>

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

std::vector<Path> parseInput(std::string_view input)
{
    std::vector<Path> ret;
    bool new_line_pending = true;
    bool north_pending = false;
    bool south_pending = false;
    for (auto const& c : input) {
        if (c == '\n') {
            assert((!north_pending) && (!south_pending));
            new_line_pending = true;
            continue;
        } else if (new_line_pending) {
            ret.emplace_back();
            new_line_pending = false;
        }
        if (c == 'n') {
            assert((!north_pending) && (!south_pending));
            north_pending = true;
        } else if (c == 's') {
            assert((!north_pending) && (!south_pending));
            south_pending = true;
        } else if (c == 'e') {
            if (north_pending) {
                ret.back().push_back(Direction::NorthEast);
                north_pending = false;
            } else if (south_pending) {
                ret.back().push_back(Direction::SouthEast);
                south_pending = false;
            } else {
                ret.back().push_back(Direction::East);
            }
        } else {
            assert(c == 'w');
            if (north_pending) {
                ret.back().push_back(Direction::NorthWest);
                north_pending = false;
            } else if (south_pending) {
                ret.back().push_back(Direction::SouthWest);
                south_pending = false;
            } else {
                ret.back().push_back(Direction::West);
            }
        }
    }
    return ret;
}

AxialHexCoordinate moveInDirection(AxialHexCoordinate c, Direction dir)
{
    switch (dir) {
    case Direction::NorthEast:
        c.q += 1;
        c.r -= 1;
        break;
    case Direction::East:
        c.q += 1;
        break;
    case Direction::SouthEast:
        c.r += 1;
        break;
    case Direction::SouthWest:
        c.q -= 1;
        c.r += 1;
        break;
    case Direction::West:
        c.q -= 1;
        break;
    case Direction::NorthWest:
        c.r -= 1;
        break;
    }
    return c;
}

std::vector<AxialHexCoordinate> walkPaths(std::vector<Path> const& paths)
{
    std::vector<AxialHexCoordinate> ret;
    ret.reserve(paths.size());
    for (auto const& p : paths) {
        AxialHexCoordinate c{0, 0};
        for (auto const& dir : p) {
            c = moveInDirection(c, dir);
        }
        ret.push_back(c);
    }
    return ret;
}

std::unordered_set<AxialHexCoordinate> determineFlippedTiles(std::vector<Path> const& paths)
{
    auto coords = walkPaths(paths);
    std::unordered_set<AxialHexCoordinate> flipped_tiles;
    for (auto const c : coords) {
        if (flipped_tiles.contains(c)) {
            flipped_tiles.erase(c);
        } else {
            flipped_tiles.insert(c);
        }
    }
    return flipped_tiles;
}

int countFlippedTiles(std::vector<Path> const& paths)
{
    return static_cast<int>(determineFlippedTiles(paths).size());
}

std::unordered_set<AxialHexCoordinate> step(std::unordered_set<AxialHexCoordinate> const& tiles)
{
    int min_q = tiles.begin()->q;
    int max_q = tiles.begin()->q;
    int min_r = tiles.begin()->r;
    int max_r = tiles.begin()->r;
    for (auto const& t : tiles) {
        min_q = std::min(min_q, t.q);
        max_q = std::max(max_q, t.q);
        min_r = std::min(min_r, t.r);
        max_r = std::max(max_r, t.r);
    }

    auto countNeighbours = [&tiles](AxialHexCoordinate const& ic) -> int {
        int acc = 0;
        acc += tiles.contains(moveInDirection(ic, Direction::NorthEast)) ? 1 : 0;
        acc += tiles.contains(moveInDirection(ic, Direction::East)) ? 1 : 0;
        acc += tiles.contains(moveInDirection(ic, Direction::SouthEast)) ? 1 : 0;
        acc += tiles.contains(moveInDirection(ic, Direction::SouthWest)) ? 1 : 0;
        acc += tiles.contains(moveInDirection(ic, Direction::West)) ? 1 : 0;
        acc += tiles.contains(moveInDirection(ic, Direction::NorthWest)) ? 1 : 0;
        return acc;
    };

    std::unordered_set<AxialHexCoordinate> ret;
    for (int ir = min_r - 1; ir <= max_r + 1; ++ir) {
        for (int iq = min_q - 1; iq <= max_q + 1; ++iq) {
            AxialHexCoordinate ic{ iq, ir };
            bool is_black = tiles.contains(ic);
            int const neighbours = countNeighbours(ic);
            if (is_black) {
                if (!((neighbours == 0) || (neighbours > 2))) {
                    ret.insert(ic);
                }
            } else {
                if (neighbours == 2) {
                    ret.insert(ic);
                }
            }
        }
    }

    return ret;
}

int simulate(std::vector<Path> const& paths)
{
    std::unordered_set<AxialHexCoordinate> tiles = determineFlippedTiles(paths);

    for (int i = 0; i < 100; ++i) {
        tiles = step(tiles);
    }

    return static_cast<int>(tiles.size());
}
