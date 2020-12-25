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

enum class TransformState {
    Straight = 0,
    Rot90,
    Rot180,
    Rot270,
    Flip,
    Flip90,
    Flip180,
    Flip270
};

struct CompressedTile {
    std::bitset<10> top;
    std::bitset<10> bottom;
    std::bitset<10> left;
    std::bitset<10> right;
    int32_t id;
    TransformState transform;
};

std::vector<RawTile> parseInput(std::string_view input);

CompressedTile compressTile(RawTile const& t);

[[nodiscard]] CompressedTile transformCompressed(CompressedTile ct);

[[nodiscard]] RawTile rot90(RawTile const& t);
[[nodiscard]] RawTile flip(RawTile const& t);

[[nodiscard]] RawTile transformTo(RawTile const& t, TransformState target_state);

struct SortedTiles {
    std::vector<RawTile> corner;
    std::vector<RawTile> edge;
    std::vector<RawTile> middle;
};

SortedTiles findCorners(std::vector<RawTile> const& t);

int64_t solve1(std::vector<RawTile> const& t);

std::vector<RawTile> solvePuzzle(SortedTiles const& sorted_tiles);

std::vector<RawTile> transformSolution(std::vector<RawTile> const& in, TransformState target_state);

struct GaplessField {
    int dimension;
    std::vector<int> field;
};

template<>
struct fmt::formatter<GaplessField>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(GaplessField const& f, FormatContext& ctx) {
        for (int iy = 0; iy < f.dimension; ++iy) {
            for (int ix = 0; ix < f.dimension; ++ix) {
                fmt::format_to(ctx.out(), "{}", ((f.field[iy * f.dimension + ix] == 1) ? '#' : '.'));
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

GaplessField makeGapless(std::vector<RawTile> const& tiles);

struct Pattern {
    int width;
    int height;
    std::vector<int> data;
};

Pattern getDragonPattern();

struct Vec2 {
    int x;
    int y;
};

std::vector<Vec2> findInField(std::vector<RawTile> const& field, Pattern const& p);

int64_t solve2(std::vector<RawTile> const& tiles);

#endif
