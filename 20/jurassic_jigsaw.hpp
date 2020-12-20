#ifndef ADVENT_OF_CODE_20_JURASSIC_JIGSAW_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_20_JURASSIC_JIGSAW_HPP_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

struct RawTile {
    std::array<int, 10*10> field;
    int32_t id;
};

template<>
struct fmt::formatter<RawTile>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(RawTile const& t, FormatContext& ctx) {
        for (int iy = 0; iy < 10; ++iy) {
            for (int ix = 0; ix < 10; ++ix) {
                fmt::format_to(ctx.out(), "{}", ((t.field[iy * 10 + ix] == 1) ? '#' : '.'));
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

struct CompressedTile {
    std::bitset<10> top;
    std::bitset<10> bottom;
    std::bitset<10> left;
    std::bitset<10> right;
    int32_t id;
};

std::vector<RawTile> parseInput(std::string_view input);

CompressedTile compressTile(RawTile const& t);

struct SortedTiles {
    std::vector<RawTile> corner;
    std::vector<RawTile> edge;
    std::vector<RawTile> middle;
};

SortedTiles findCorners(std::vector<RawTile> const& t);

int64_t solve1(std::vector<RawTile> const& t);

#endif
