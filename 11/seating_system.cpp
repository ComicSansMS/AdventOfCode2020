#include <seating_system.hpp>

#include <fmt/printf.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>


Map parseInput(std::string_view input)
{
    Map ret;
    ret.width = 0;
    ret.height = 1;
    std::size_t cell_width = 0;
    for (auto const& c : input) {
        if (c == '\n') {
            ++ret.height;
            if (ret.width == 0) {
                ret.width = cell_width;
            }
            assert(ret.width == cell_width);
            cell_width = 0;
        } else if (c == 'L') {
            ret.grid.push_back(Cell::Empty);
            ++cell_width;
        } else if (c == '#') {
            ret.grid.push_back(Cell::Occupied);
            ++cell_width;
        } else {
            assert(c == '.');
            ret.grid.push_back(Cell::Floor);
            ++cell_width;
        }
    }
    assert((cell_width == 0) || (cell_width == ret.width));
    if (cell_width == 0) { --ret.height; }
    return ret;
}

Cell getCell(Map const& m, int x, int y)
{
    if ((x < 0) || (x >= m.width) || (y < 0) || (y >= m.height)) {
        return Cell::Floor;
    }
    return m.grid[y * m.width + x];
}

Map step(Map const& m)
{
    Map next = m;
    for (int iy = 0; iy < m.height; ++iy) {
        for (int ix = 0; ix < m.width; ++ix) {
            Cell const c = getCell(m, ix, iy);
            std::array<Cell, 8> const neighbours = {
                getCell(m, ix - 1, iy - 1),
                getCell(m, ix,     iy - 1),
                getCell(m, ix + 1, iy - 1),
                getCell(m, ix - 1, iy),
                getCell(m, ix + 1, iy),
                getCell(m, ix - 1, iy + 1),
                getCell(m, ix,     iy + 1),
                getCell(m, ix + 1, iy + 1),
            };
            Cell next_cell = c;
            if (c == Cell::Empty) {
                if (std::count(begin(neighbours), end(neighbours), Cell::Occupied) == 0) {
                    next_cell = Cell::Occupied;
                }
            } else if (c == Cell::Occupied) {
                if (std::count(begin(neighbours), end(neighbours), Cell::Occupied) >= 4) {
                    next_cell = Cell::Empty;
                }
            }
            next.grid[iy * next.width + ix] = next_cell;
        }
    }
    return next;
}

int64_t countOccupiedSeats(Map const& m)
{
    return std::count(begin(m.grid), end(m.grid), Cell::Occupied);
}

int64_t simulate(Map const& m)
{
    Map it_map = m;
    for (;;) {
        Map const next = step(it_map);
        if (next.grid == it_map.grid) { break; }
        it_map = next;
    }
    return countOccupiedSeats(it_map);
}

Cell getFirstCellInDirection(Map const& m, int x, int y, int inc_x, int inc_y)
{
    assert((inc_x != 0) || (inc_y != 0));
    assert((inc_x >= -1) && (inc_x <= 1));
    assert((inc_y >= -1) && (inc_y <= 1));
    int ix = x;
    int iy = y;
    for (;;) {
        ix += inc_x;
        iy += inc_y;
        if ((ix < 0) || (ix >= m.width) || (iy < 0) || (iy >= m.height)) {
            return Cell::Floor;
        }
        Cell const c = getCell(m, ix, iy);
        if (c != Cell::Floor) {
            return c;
        }
    }
}

Map step2(Map const& m)
{
    Map next = m;
    for (int iy = 0; iy < m.height; ++iy) {
        for (int ix = 0; ix < m.width; ++ix) {
            Cell const c = getCell(m, ix, iy);
            std::array<Cell, 8> const neighbours = {
                getFirstCellInDirection(m, ix, iy, -1, -1),
                getFirstCellInDirection(m, ix, iy,  0, -1),
                getFirstCellInDirection(m, ix, iy,  1, -1),
                getFirstCellInDirection(m, ix, iy, -1,  0),
                getFirstCellInDirection(m, ix, iy,  1,  0),
                getFirstCellInDirection(m, ix, iy, -1,  1),
                getFirstCellInDirection(m, ix, iy,  0,  1),
                getFirstCellInDirection(m, ix, iy,  1,  1),
            };
            Cell next_cell = c;
            if (c == Cell::Empty) {
                if (std::count(begin(neighbours), end(neighbours), Cell::Occupied) == 0) {
                    next_cell = Cell::Occupied;
                }
            } else if (c == Cell::Occupied) {
                if (std::count(begin(neighbours), end(neighbours), Cell::Occupied) >= 5) {
                    next_cell = Cell::Empty;
                }
            }
            next.grid[iy * next.width + ix] = next_cell;
        }
    }
    return next;
}

int64_t simulate2(Map const& m)
{
    Map it_map = m;
    for (;;) {
        Map const next = step2(it_map);
        if (next.grid == it_map.grid) { break; }
        it_map = next;
    }
    return countOccupiedSeats(it_map);
}
