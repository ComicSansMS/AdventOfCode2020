#include <toboggan_trajectory.hpp>

#include <algorithm>
#include <cassert>
#include <limits>
#include <regex>
#include <sstream>
#include <string>

Map parseInput(std::string_view input)
{
    Map ret;
    ret.m.emplace_back();
    ret.map_width = -1;
    int i_col = 0;
    for (auto const& c : input) {
        if (c == '\n') {
            ret.m.emplace_back();
            if (ret.map_width == -1) {
                ret.map_width = i_col;
            }
            assert(i_col == ret.map_width);
            i_col = 0;
            continue;
        } else if (c == '#') {
            ret.m.back().insert(i_col);
        } else {
            assert(c == '.');
        }
        ++i_col;
    }
    return ret;
}

bool hasTreeAt(Map const& m, int x, int y)
{
    if ((y < 0) || (y >= static_cast<int>(m.m.size()))) { return false; }
    auto const& row = m.m[y];
    return row.find(x % m.map_width) != std::end(row);
}

std::vector<Vec2> slopeTraversal(Map const& m, int slope_x, int slope_y)
{
    std::vector<Vec2> ret;
    int ix = 0;
    for (int iy = 0; iy < static_cast<int>(m.m.size()); iy += slope_y) {
        if (hasTreeAt(m, ix, iy)) {
            ret.push_back(Vec2{ ix, iy });
        }
        ix += slope_x;
    }
    return ret;
}

int64_t countAllTraversals(Map const& m)
{
    int64_t acc = 1;
    acc *= static_cast<int64_t>(slopeTraversal(m, 1, 1).size());
    acc *= static_cast<int64_t>(slopeTraversal(m, 3, 1).size());
    acc *= static_cast<int64_t>(slopeTraversal(m, 5, 1).size());
    acc *= static_cast<int64_t>(slopeTraversal(m, 7, 1).size());
    acc *= static_cast<int64_t>(slopeTraversal(m, 1, 2).size());
    return acc;
}

