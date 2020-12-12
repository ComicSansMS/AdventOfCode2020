#ifndef ADVENT_OF_CODE_12_RAIN_RISK_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_RAIN_RISK_HPP_INCLUDE_GUARD

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

enum class Op {
    North,
    South,
    East,
    West,
    Left,
    Right,
    Forward
};

struct Command {
    Op op;
    int64_t arg;
};

struct Vec2 {
    int64_t x;
    int64_t y;
};

struct Ferry {
    Vec2 position;
    Vec2 direction;
};

std::vector<Command> parseInput(std::string_view input);

Vec2 rotate(Vec2 const& v, int64_t angle);

int64_t distance(Vec2 const& v);

std::vector<Ferry> plotCourse(std::vector<Command> const& commands);


struct WaypointFerry {
    Vec2 ferry;
    Vec2 waypoint;
};

std::vector<WaypointFerry> plotCourse2(std::vector<Command> const& commands);

#endif
