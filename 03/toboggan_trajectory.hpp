#ifndef ADVENT_OF_CODE_03_TOGOGGAN_TRAJECTORY_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_TOGOGGAN_TRAJECTORY_HPP_INCLUDE_GUARD

#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Map {
    std::vector<std::unordered_set<int>> m;
    int map_width;
};

Map parseInput(std::string_view input);

bool hasTreeAt(Map const& m, int x, int y);

struct Vec2 {
    int x;
    int y;
};

std::vector<Vec2> slopeTraversal(Map const& m, int slope_x, int slope_y);

int64_t countAllTraversals(Map const& m);

#endif
