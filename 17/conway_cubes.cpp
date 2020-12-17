#include <conway_cubes.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

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


World parseInput(std::string_view input)
{
    World ret;
    ret.min = Vec4{ 0, 0, 0, 0 };
    ret.max = Vec4{ 0, 0, 1, 1 };
    Vec4 coord_iter = ret.min;
    for (char const& c : input) {
        if (c == '\n') {
            if (ret.max.x == 0) {
                ret.max.x = coord_iter.x;
            }
            assert(coord_iter.x == ret.max.x);
            coord_iter.x = 0;
            ++coord_iter.y;
        } else if (c == '#') {
            ret.map.insert(coord_iter);
            ++coord_iter.x;
        } else {
            assert(c == '.');
            ++coord_iter.x;
        }
    }
    ret.max.y = coord_iter.y;
    return ret;
}

int countNeighbours3(World const& w, Vec4 const& p)
{
    int acc = 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z - 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z - 1, 0 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z,     0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z,     0 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z + 1, 0 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z + 1, 0 })) ? 1 : 0;
    return acc;
}

World step3(World const& w)
{
    World ret;
    ret.min = ret.max = Vec4{ 0, 0, 0, 0 };
    auto updateBounds = [](World& w, Vec4 const& p) {
        w.min.x = std::min(w.min.x, p.x);
        w.min.y = std::min(w.min.y, p.y);
        w.min.z = std::min(w.min.z, p.z);

        w.max.x = std::max(w.max.x, p.x + 1);
        w.max.y = std::max(w.max.y, p.y + 1);
        w.max.z = std::max(w.max.z, p.z + 1);
    };
    Vec4 min{ w.min.x - 1, w.min.y - 1, w.min.z - 1, 0 };
    Vec4 max{ w.max.x + 1, w.max.y + 1, w.max.z + 1, 0 };
    for (int iz = min.z; iz < max.z; ++iz) {
        for (int iy = min.y; iy < max.y; ++iy) {
            for (int ix = min.x; ix < max.x; ++ix) {
                Vec4 const p{ ix, iy, iz, 0 };
                int const neighbours = countNeighbours3(w, p);
                if (w.map.contains(p)) {
                    // cube is active
                    if ((neighbours == 2) || (neighbours == 3)) {
                        ret.map.insert(p);
                        updateBounds(ret, p);
                    }
                } else {
                    // cube is inactive
                    if (neighbours == 3) {
                        ret.map.insert(p);
                        updateBounds(ret, p);
                    }
                }
            }
        }
    }
    return ret;
}

int bootCycle3(World w)
{
    for (int i = 0; i < 6; ++i) {
        w = step3(w);
    }
    return static_cast<int>(w.map.size());
}

int countNeighbours4(World const& w, Vec4 const& p)
{
    int acc = 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z - 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z - 1, p.w - 1 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z,     p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z,     p.w - 1 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z + 1, p.w - 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z + 1, p.w - 1 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z - 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z - 1, p.w })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z,     p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z,     p.w })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z + 1, p.w })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z + 1, p.w })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z - 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z - 1, p.w + 1 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z,     p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z,     p.w + 1 })) ? 1 : 0;

    acc += (w.map.contains(Vec4{ p.x - 1, p.y - 1, p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y - 1, p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y - 1, p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y,     p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y,     p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y,     p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x - 1, p.y + 1, p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x,     p.y + 1, p.z + 1, p.w + 1 })) ? 1 : 0;
    acc += (w.map.contains(Vec4{ p.x + 1, p.y + 1, p.z + 1, p.w + 1 })) ? 1 : 0;
    return acc;
}

World step4(World const& w)
{
    World ret;
    ret.min = ret.max = Vec4{ 0, 0, 0, 0 };
    auto updateBounds = [](World& w, Vec4 const& p) {
        w.min.x = std::min(w.min.x, p.x);
        w.min.y = std::min(w.min.y, p.y);
        w.min.z = std::min(w.min.z, p.z);
        w.min.w = std::min(w.min.w, p.w);

        w.max.x = std::max(w.max.x, p.x + 1);
        w.max.y = std::max(w.max.y, p.y + 1);
        w.max.z = std::max(w.max.z, p.z + 1);
        w.max.w = std::max(w.max.w, p.w + 1);
    };
    Vec4 min{ w.min.x - 1, w.min.y - 1, w.min.z - 1, w.min.w - 1 };
    Vec4 max{ w.max.x + 1, w.max.y + 1, w.max.z + 1, w.max.w + 1 };
    for (int iw = min.w; iw < max.w; ++iw) {
        for (int iz = min.z; iz < max.z; ++iz) {
            for (int iy = min.y; iy < max.y; ++iy) {
                for (int ix = min.x; ix < max.x; ++ix) {
                    Vec4 const p{ ix, iy, iz, iw };
                    int const neighbours = countNeighbours4(w, p);
                    if (w.map.contains(p)) {
                        // cube is active
                        if ((neighbours == 2) || (neighbours == 3)) {
                            ret.map.insert(p);
                            updateBounds(ret, p);
                        }
                    } else {
                        // cube is inactive
                        if (neighbours == 3) {
                            ret.map.insert(p);
                            updateBounds(ret, p);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

int bootCycle4(World w)
{
    for (int i = 0; i < 6; ++i) {
        w = step4(w);
    }
    return static_cast<int>(w.map.size());
}

