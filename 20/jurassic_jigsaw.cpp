#include <jurassic_jigsaw.hpp>

#include <fmt/printf.h>

#include <range/v3/view/cartesian_product.hpp>
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

std::vector<RawTile> parseInput(std::string_view input)
{
    std::vector<RawTile> ret;
    std::stringstream sstr{std::string{input}};
    int line_count = 0;
    std::string line;
    RawTile t;
    while (std::getline(sstr, line)) {
        if (line_count == 0) {
            assert(line.starts_with("Tile "));
            assert(line[9] == ':');
            t.id = std::stoi(line.substr(5, 4));
        } else if (line_count == 11) {
            assert(line == "");
            ret.push_back(t);
            line_count = 0;
            continue;
        } else {
            assert(line.size() == 10);
            int start_index = (line_count - 1) * 10;
            for (int i = 0; i < 10; ++i) {
                assert((line[i] == '.') || (line[i] == '#'));
                t.field[start_index + i] = (line[i] == '.') ? 0 : 1;
            }
        }
        ++line_count;
    }
    assert(line_count == 0);
    return ret;
}

CompressedTile compressTile(RawTile const& t)
{
    CompressedTile ret;
    for (int i = 0; i < 10; ++i) { ret.top[9 - i]    = t.field[i]; }
    for (int i = 0; i < 10; ++i) { ret.bottom[9 - i] = t.field[i + 90]; }
    for (int i = 0; i < 10; ++i) { ret.left[9 - i]   = t.field[i * 10]; }
    for (int i = 0; i < 10; ++i) { ret.right[9 - i]  = t.field[(i * 10) + 9]; }
    return ret;
}

template<std::size_t N>
std::bitset<N> reverse(std::bitset<N> const& b)
{
    std::bitset<N> ret;
    for(std::size_t i = 0; i < N; ++i) { ret[i] = b[N - i - 1]; }
    return ret;
}

bool hasMatchingSide(CompressedTile const& candidate, CompressedTile const& match, std::bitset<10> CompressedTile::* side_to_check)
{
    std::bitset<10> const& s = candidate.*side_to_check;
    std::bitset<10> const s_inv = reverse(s);
    return ((match.top == s)    || (match.top == s_inv)    ||
            (match.bottom == s) || (match.bottom == s_inv) ||
            (match.left == s)   || (match.left == s_inv)   ||
            (match.right == s)  || (match.right == s_inv));
}

SortedTiles findCorners(std::vector<RawTile> const& t)
{
    std::vector<CompressedTile> ctiles;
    ctiles.reserve(t.size());
    std::transform(begin(t), end(t), std::back_inserter(ctiles), compressTile);
    auto const check_side = [&ctiles](std::size_t candidate_index, std::bitset<10> CompressedTile::* side_to_check) -> bool {
        CompressedTile const& candidate = ctiles[candidate_index];
        bool found_match = false;
        for (std::size_t ii = 0; ii < ctiles.size(); ++ii) {
            if (ii != candidate_index) {
                if(hasMatchingSide(candidate, ctiles[ii], side_to_check)) { found_match = true; break; }
            }
        }
        return found_match;
    };
    SortedTiles ret;
    for (std::size_t i = 0; i < t.size(); ++i) {
        int unmatched_sides = 0;
        if (!check_side(i, &CompressedTile::top)) { ++unmatched_sides; }
        if (!check_side(i, &CompressedTile::bottom)) { ++unmatched_sides; }
        if (!check_side(i, &CompressedTile::left)) { ++unmatched_sides; }
        if (!check_side(i, &CompressedTile::right)) { ++unmatched_sides; }
        assert(unmatched_sides < 3);
        if (unmatched_sides == 2) {
            ret.corner.push_back(t[i]);
        } else if(unmatched_sides == 1) {
            ret.edge.push_back(t[i]);
        } else {
            assert(unmatched_sides == 0);
            ret.middle.push_back(t[i]);
        }
    }
    return ret;
}

int64_t solve1(std::vector<RawTile> const& t)
{
    auto const corners = findCorners(t).corner;
    assert(corners.size() == 4);
    return std::accumulate(begin(corners), end(corners), int64_t{1},
                           [](int64_t acc, RawTile const& t) { return acc * t.id; });
}
