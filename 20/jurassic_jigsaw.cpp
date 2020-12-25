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
    ret.id = t.id;
    ret.transform = TransformState::Straight;
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
    return ((match.top == s)    || (reverse(match.top) == s)    ||
            (match.bottom == s) || (reverse(match.bottom) == s) ||
            (match.left == s)   || (reverse(match.left) == s)   ||
            (match.right == s)  || (reverse(match.right) == s));
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

[[nodiscard]] CompressedTile transformCompressed(CompressedTile ct)
{
    auto rot90 = [](CompressedTile const& ct) -> CompressedTile {
        CompressedTile ret = ct;
        ret.right = ct.top;
        ret.bottom = reverse(ct.right);
        ret.left = ct.bottom;
        ret.top = reverse(ct.left);
        return ret;
    };
    auto flip = [](CompressedTile const& ct) -> CompressedTile {
        CompressedTile ret = ct;
        ret.right = reverse(ct.right);
        ret.bottom = ct.top;
        ret.top = ct.bottom;
        ret.left = reverse(ct.left);
        return ret;
    };
    switch (ct.transform)
    {
    default: assert(false);
    case TransformState::Straight:
        ct.transform = TransformState::Rot90;
        return rot90(ct);
    case TransformState::Rot90:
        ct.transform = TransformState::Rot180;
        return rot90(ct);
    case TransformState::Rot180:
        ct.transform = TransformState::Rot270;
        return rot90(ct);
    case TransformState::Rot270:
        ct.transform = TransformState::Flip;
        return flip(rot90(ct));
    case TransformState::Flip:
        ct.transform = TransformState::Flip90;
        return rot90(ct);
    case TransformState::Flip90:
        ct.transform = TransformState::Flip180;
        return rot90(ct);
    case TransformState::Flip180:
        ct.transform = TransformState::Flip270;
        return rot90(ct);
    case TransformState::Flip270:
        ct.transform = TransformState::Straight;
        return flip(rot90(ct));
    }
}

[[nodiscard]] RawTile rot90(RawTile const& t)
{
    RawTile ret;
    ret.id = t.id;
    for (int j = 0; j < 10; ++j) {
        for (int i = 0; i < 10; ++i) {
            ret.field[i*10 + j] = t.field[(10 - j - 1)*10 + i];
        }
    }
    return ret;
}

[[nodiscard]] RawTile flip(RawTile const& t)
{
    RawTile ret;
    ret.id = t.id;
    for (int j = 0; j < 10; ++j) {
        for (int i = 0; i < 10; ++i) {
            ret.field[i*10 + j] = t.field[(10 - i - 1)*10 + j];
        }
    }
    return ret;
}

[[nodiscard]] RawTile transformTo(RawTile const& t, TransformState target_state)
{
    switch (target_state)
    {
    default: assert(false);
    case TransformState::Straight:
        return t;
    case TransformState::Rot90:
        return rot90(t);
    case TransformState::Rot180:
        return rot90(rot90(t));
    case TransformState::Rot270:
        return rot90(rot90(rot90(t)));
    case TransformState::Flip:
        return flip(t);
    case TransformState::Flip90:
        return rot90(flip(t));
        break;
    case TransformState::Flip180:
        return rot90(rot90(flip(t)));
        break;
    case TransformState::Flip270:
        return flip(rot90(t));
        break;
    }
}

bool hasAnyMatch(CompressedTile const& source_tile, std::vector<CompressedTile> const& haystack, std::bitset<10> CompressedTile::* source_edge)
{
    return std::any_of(begin(haystack), end(haystack),
        [&source_tile, source_edge](CompressedTile const& m) { return (&m != &source_tile) && hasMatchingSide(source_tile, m, source_edge); });
}

std::vector<std::vector<CompressedTile>::iterator> findMatchesFor(CompressedTile const& source_tile, std::vector<CompressedTile>& haystack, std::bitset<10> CompressedTile::* source_edge)
{
    std::vector<std::vector<CompressedTile>::iterator> ret;
    for (auto it = haystack.begin(), it_end = haystack.end(); it != it_end; ++it) {
        if (hasMatchingSide(source_tile, *it, source_edge)) {
            ret.push_back(it);
        }
    }
    return ret;
}

/*
1951    2311    3079
2729    1427    2473
2971    1489    1171
*/

std::vector<RawTile> solvePuzzle(SortedTiles const& sorted_tiles)
{
    assert(sorted_tiles.corner.size() == 4);
    assert(sorted_tiles.edge.size() % 4 == 0);
    int const dimension = static_cast<int>(sorted_tiles.edge.size() / 4)  + 2;
    assert(sorted_tiles.middle.size() == (dimension - 2) * (dimension - 2));

    std::vector<CompressedTile> corner;
    std::transform(begin(sorted_tiles.corner), end(sorted_tiles.corner), std::back_inserter(corner), compressTile);
    std::vector<CompressedTile> edge;
    std::transform(begin(sorted_tiles.edge), end(sorted_tiles.edge), std::back_inserter(edge), compressTile);
    std::vector<CompressedTile> middle;
    std::transform(begin(sorted_tiles.middle), end(sorted_tiles.middle), std::back_inserter(middle), compressTile);

    std::vector<CompressedTile> solution;
    solution.reserve(dimension * dimension);
    // orient first corner correctly
    solution.push_back(corner.back());
    corner.pop_back();
    CompressedTile& corner_ul = solution.front();
    while (!(hasAnyMatch(corner_ul, edge, &CompressedTile::right) &&
             hasAnyMatch(corner_ul, edge, &CompressedTile::bottom)))
    {
        corner_ul = transformCompressed(corner_ul);
    }

    // solve top edge
    for (int i = 0; i < dimension - 2; ++i) {
        CompressedTile const& left_neighbour = solution.back();
        auto matches = findMatchesFor(left_neighbour, edge, &CompressedTile::right);
        assert(matches.size() == 1);
        auto it_next = matches.back();
        assert(it_next != end(edge));
        CompressedTile edge_piece = *it_next;
        edge.erase(it_next);
        // find correct orientation
        while (! ((left_neighbour.right == edge_piece.left) &&
                  (hasAnyMatch(edge_piece, (i == (dimension - 3) ? corner : edge), &CompressedTile::right)) &&
                  (hasAnyMatch(edge_piece, middle, &CompressedTile::bottom))) )
        {
            edge_piece = transformCompressed(edge_piece);
            assert(edge_piece.transform != TransformState::Straight);
        }
        solution.push_back(edge_piece);
    }

    // solve top-left corner
    {
        CompressedTile const& left_neighbour = solution.back();
        auto matches = findMatchesFor(left_neighbour, corner, &CompressedTile::right);
        assert(matches.size() == 1);
        auto it_next = matches.back();
        CompressedTile next_piece = *it_next;
        corner.erase(it_next);
        // find correct orientation
        while (! ((left_neighbour.right == next_piece.left) &&
                 (hasAnyMatch(next_piece, edge, &CompressedTile::bottom))) )
        {
            next_piece = transformCompressed(next_piece);
            assert(next_piece.transform != TransformState::Straight);
        }
        solution.push_back(next_piece);
    }

    // solve center pieces
    for (int i_row = 0; i_row < dimension - 2; ++i_row) {
        // left edge
        {
            CompressedTile const& top_neighbour = solution[i_row * dimension];
            auto matches = findMatchesFor(top_neighbour, edge, &CompressedTile::bottom);
            assert(matches.size() == 1);
            auto it_next = matches.back();
            CompressedTile next_piece = *it_next;
            edge.erase(it_next);
            // find correct orientation
            while (! ((top_neighbour.bottom == next_piece.top) &&
                      (hasAnyMatch(next_piece, middle, &CompressedTile::right))) )
            {
                next_piece = transformCompressed(next_piece);
                assert(next_piece.transform != TransformState::Straight);
            }
            solution.push_back(next_piece);
        }

        // middle
        for (int i_col = 0; i_col < dimension - 2; ++i_col) {
            CompressedTile const& top_neighbour = solution[(i_row * dimension) + i_col + 1];
            CompressedTile const& left_neighbour = solution.back();
            auto matches = findMatchesFor(top_neighbour, middle, &CompressedTile::bottom);
            assert(matches.size() == 1);
            auto it_next = matches.back();
            CompressedTile next_piece = *it_next;
            middle.erase(it_next);
            // find correct orientation
            while (! ((top_neighbour.bottom == next_piece.top) &&
                      (left_neighbour.right == next_piece.left) &&
                      (hasAnyMatch(next_piece, ((i_col == dimension - 3) ? edge : middle), &CompressedTile::right))) )
            {
                next_piece = transformCompressed(next_piece);
                assert(next_piece.transform != TransformState::Straight);
            }
            solution.push_back(next_piece);
        }

        // right edge
        {
            CompressedTile const& top_neighbour = solution[(i_row + 1) * dimension - 1];
            CompressedTile const& left_neighbour = solution.back();
            auto matches = findMatchesFor(top_neighbour, edge, &CompressedTile::bottom);
            assert(matches.size() == 1);
            auto it_next = matches.back();
            CompressedTile next_piece = *it_next;
            edge.erase(it_next);
            // find correct orientation
            while (! ((top_neighbour.bottom == next_piece.top) &&
                      (left_neighbour.right == next_piece.left)) )
            {
                next_piece = transformCompressed(next_piece);
                assert(next_piece.transform != TransformState::Straight);
            }
            solution.push_back(next_piece);
        }
    }
    assert(middle.empty());

    // solve bottom-left corner
    {
        CompressedTile const& top_neighbour = solution[(dimension - 2) * dimension];
        auto matches = findMatchesFor(top_neighbour, corner, &CompressedTile::bottom);
        assert(matches.size() == 1);
        auto it_next = matches.back();
        CompressedTile next_piece = *it_next;
        corner.erase(it_next);
        // find correct orientation
        while (! ((top_neighbour.bottom == next_piece.top) &&
                  (hasAnyMatch(next_piece, edge, &CompressedTile::right))) )
        {
            next_piece = transformCompressed(next_piece);
            assert(next_piece.transform != TransformState::Straight);
        }
        solution.push_back(next_piece);
    }

    // solve bottom edge
    for (int i = 0; i < dimension - 2; ++i) {
        CompressedTile const& top_neighbour = solution[(dimension - 2) * dimension + i + 1];
        CompressedTile const& left_neighbour = solution.back();
        auto matches = findMatchesFor(left_neighbour, edge, &CompressedTile::right);
        assert(matches.size() == 1);
        auto it_next = matches.back();
        assert(it_next != end(edge));
        CompressedTile edge_piece = *it_next;
        edge.erase(it_next);
        // find correct orientation
        while (! ((left_neighbour.right == edge_piece.left) &&
                  (top_neighbour.bottom == edge_piece.top) &&
                  (hasAnyMatch(edge_piece, (i == (dimension - 3) ? corner : edge), &CompressedTile::right))) )
        {
            edge_piece = transformCompressed(edge_piece);
            assert(edge_piece.transform != TransformState::Straight);
        }
        solution.push_back(edge_piece);
    }
    assert(edge.empty());

    // place final corner
    {
        assert(corner.size() == 1);
        CompressedTile final_corner = corner.back();
        corner.pop_back();
        CompressedTile const& top_neighbour = solution[(dimension - 1) * dimension - 1];
        CompressedTile const& left_neighbour = solution.back();
        // find correct orientation
        while (! ((left_neighbour.right == final_corner.left) &&
                  (top_neighbour.bottom == final_corner.top)) )
        {
            final_corner = transformCompressed(final_corner);
            assert(final_corner.transform != TransformState::Straight);
        }
        solution.push_back(final_corner);
    }
    assert(corner.empty());


    return {};
}
