#include <rain_risk.hpp>

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


std::vector<Command> parseInput(std::string_view input)
{
    std::regex rx_line(R"((\w)(\d+)[\r\n])");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<Command> ret;
    std::transform(it_begin, it_end, std::back_inserter(ret),
                   [](std::match_results<std::string_view::iterator> const& match) -> Command {
                       Command cmd;
                       cmd.arg = std::stoi(match[2]);
                       if (match[1] == "N") {
                           cmd.op = Op::North;
                       } else if (match[1] == "S") {
                           cmd.op = Op::South;
                       } else if (match[1] == "E") {
                           cmd.op = Op::East;
                       } else if (match[1] == "W") {
                           cmd.op = Op::West;
                       } else if (match[1] == "L") {
                           assert(cmd.arg % 90 == 0);
                           cmd.op = Op::Left;
                       } else if (match[1] == "R") {
                           assert(cmd.arg % 90 == 0);
                           cmd.op = Op::Right;
                       } else {
                           assert(match[1] == "F");
                           cmd.op = Op::Forward;
                       }
                       return cmd;
                   });
    return ret;
}

Vec2 rotate(Vec2 const& v, int64_t angle)
{
    int64_t const direction = (angle < 0) ? -1 : 1;
    int64_t const iterations = (angle * direction) / 90;
    Vec2 ret = v;
    for (int64_t i = 0; i < iterations; ++i) {
        Vec2 next = ret;
        next.x = direction * ret.y;
        next.y = -1 * direction * ret.x;
        ret = next;
    }
    return ret;
};

int64_t distance(Vec2 const& v)
{
    return std::abs(v.x) + std::abs(v.y);
}

std::vector<Ferry> plotCourse(std::vector<Command> const& commands)
{
    Ferry ferry;
    ferry.direction = Vec2{ 1, 0 };
    ferry.position = Vec2{ 0, 0 };

    std::vector<Ferry> ret;
    for (auto const& cmd : commands) {
        if (cmd.op == Op::North) {
            ferry.position.y += cmd.arg;
        } else if (cmd.op == Op::South) {
            ferry.position.y -= cmd.arg;
        } else if (cmd.op == Op::East) {
            ferry.position.x += cmd.arg;
        } else if (cmd.op == Op::West) {
            ferry.position.x -= cmd.arg;
        } else if (cmd.op == Op::Left) {
            ferry.direction = rotate(ferry.direction, -cmd.arg);
        } else if (cmd.op == Op::Right) {
            ferry.direction = rotate(ferry.direction, cmd.arg);
        } else {
            assert(cmd.op == Op::Forward);
            ferry.position.x += ferry.direction.x * cmd.arg;
            ferry.position.y += ferry.direction.y * cmd.arg;
        }
        ret.push_back(ferry);
    }
    return ret;
}

std::vector<WaypointFerry> plotCourse2(std::vector<Command> const& commands)
{
    WaypointFerry wp;
    wp.ferry = Vec2{ 0, 0 };
    wp.waypoint = Vec2{ 10, 1 };

    std::vector<WaypointFerry> ret;

    for (auto const& cmd : commands) {
        if (cmd.op == Op::North) {
            wp.waypoint.y += cmd.arg;
        } else if (cmd.op == Op::South) {
            wp.waypoint.y -= cmd.arg;
        } else if (cmd.op == Op::East) {
            wp.waypoint.x += cmd.arg;
        } else if (cmd.op == Op::West) {
            wp.waypoint.x -= cmd.arg;
        } else if (cmd.op == Op::Left) {
            wp.waypoint = rotate(wp.waypoint, -cmd.arg);
        } else if (cmd.op == Op::Right) {
            wp.waypoint = rotate(wp.waypoint, cmd.arg);
        } else {
            assert(cmd.op == Op::Forward);
            wp.ferry.x += wp.waypoint.x * cmd.arg;
            wp.ferry.y += wp.waypoint.y * cmd.arg;
        }
        ret.push_back(wp);
    }
    return ret;
}
