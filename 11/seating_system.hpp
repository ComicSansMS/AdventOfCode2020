#ifndef ADVENT_OF_CODE_11_SEATING_SYSTEM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_11_SEATING_SYSTEM_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

enum Cell {
    Floor,
    Empty,
    Occupied
};

template<>
struct fmt::formatter<Cell>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Cell const& cell, FormatContext& ctx) {
        char const c = (cell == Cell::Empty) ? 'L' : ((cell == Cell::Occupied) ? '#' : '.');
        return fmt::format_to(ctx.out(), "{}", c);
    }
};


struct Map {
    std::vector<Cell> grid;
    std::size_t width;
    std::size_t height;
};

template<>
struct fmt::formatter<Map>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Map const& map, FormatContext& ctx) {
        std::size_t row_count = 0;
        for(auto const& g : map.grid) {
            if (row_count == map.width) {
                fmt::format_to(ctx.out(), "\n");
                row_count = 0;
            }
            fmt::format_to(ctx.out(), "{}", g);
            ++row_count;
        }
        return ctx.out();
    }
};


Map parseInput(std::string_view input);

Cell getCell(Map const& m, int x, int y);

Map step(Map const& m);

int64_t countOccupiedSeats(Map const& m);

int64_t simulate(Map const& m);

Cell getFirstCellInDirection(Map const& m, int x, int y, int inc_x, int inc_y);

Map step2(Map const& m);

int64_t simulate2(Map const& m);

#endif
