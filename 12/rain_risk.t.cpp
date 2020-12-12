#include <rain_risk.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Rain Risk")
{
    char const sample_input[] = 
        R"(F10
N3
F7
R90
F11
)";

    

    SECTION("Parse Input")
    {
        auto const commands = parseInput("F10\nN3\nF7\nR90\nF11\nS5\nE445\nW23\nL270\n");
        REQUIRE(commands.size() == 9);

        CHECK(commands[0].op == Op::Forward);
        CHECK(commands[0].arg == 10);
        CHECK(commands[1].op == Op::North);
        CHECK(commands[1].arg == 3);
        CHECK(commands[2].op == Op::Forward);
        CHECK(commands[2].arg == 7);
        CHECK(commands[3].op == Op::Right);
        CHECK(commands[3].arg == 90);
        CHECK(commands[4].op == Op::Forward);
        CHECK(commands[4].arg == 11);
        CHECK(commands[5].op == Op::South);
        CHECK(commands[5].arg == 5);
        CHECK(commands[6].op == Op::East);
        CHECK(commands[6].arg == 445);
        CHECK(commands[7].op == Op::West);
        CHECK(commands[7].arg == 23);
        CHECK(commands[8].op == Op::Left);
        CHECK(commands[8].arg == 270);
    }

    SECTION("Rotate Vector")
    {
        Vec2 v{ 1, 0 };
        v = rotate(v, 90);
        CHECK(v.x == 0);
        CHECK(v.y == -1);
        v = rotate(v, 90);
        CHECK(v.x == -1);
        CHECK(v.y == 0);
        v = rotate(v, 90);
        CHECK(v.x == 0);
        CHECK(v.y == 1);
        v = rotate(v, 90);
        CHECK(v.x == 1);
        CHECK(v.y == 0);
        v = rotate(v, -90);
        CHECK(v.x == 0);
        CHECK(v.y == 1);
        v = rotate(v, -90);
        CHECK(v.x == -1);
        CHECK(v.y == 0);
        v = rotate(v, -180);
        CHECK(v.x == 1);
        CHECK(v.y == 0);
        v = rotate(v, 270);
        CHECK(v.x == 0);
        CHECK(v.y == 1);
    }

    SECTION("Plot Course")
    {
        auto const commands = parseInput(sample_input);
        auto const course = plotCourse(commands);
        REQUIRE(course.size() == 5);
        CHECK(course[0].position.x == 10);
        CHECK(course[0].position.y == 0);
        CHECK(course[0].direction.x == 1);
        CHECK(course[0].direction.y == 0);

        CHECK(course[1].position.x == 10);
        CHECK(course[1].position.y == 3);
        CHECK(course[1].direction.x == 1);
        CHECK(course[1].direction.y == 0);

        CHECK(course[2].position.x == 17);
        CHECK(course[2].position.y == 3);
        CHECK(course[2].direction.x == 1);
        CHECK(course[2].direction.y == 0);

        CHECK(course[3].position.x == 17);
        CHECK(course[3].position.y == 3);
        CHECK(course[3].direction.x == 0);
        CHECK(course[3].direction.y == -1);

        CHECK(course[4].position.x == 17);
        CHECK(course[4].position.y == -8);
        CHECK(course[4].direction.x == 0);
        CHECK(course[4].direction.y == -1);

        CHECK(distance(course.back().position) == 25);
    }

    SECTION("Plot Course Waypoint")
    {
        auto const commands = parseInput(sample_input);
        auto const course = plotCourse2(commands);
        REQUIRE(course.size() == 5);
        CHECK(course[0].ferry.x == 100);
        CHECK(course[0].ferry.y == 10);
        CHECK(course[0].waypoint.x == 10);
        CHECK(course[0].waypoint.y == 1);

        CHECK(course[1].ferry.x == 100);
        CHECK(course[1].ferry.y == 10);
        CHECK(course[1].waypoint.x == 10);
        CHECK(course[1].waypoint.y == 4);

        CHECK(course[2].ferry.x == 170);
        CHECK(course[2].ferry.y == 38);
        CHECK(course[2].waypoint.x == 10);
        CHECK(course[2].waypoint.y == 4);

        CHECK(course[3].ferry.x == 170);
        CHECK(course[3].ferry.y == 38);
        CHECK(course[3].waypoint.x == 4);
        CHECK(course[3].waypoint.y == -10);

        CHECK(course[4].ferry.x == 214);
        CHECK(course[4].ferry.y == -72);
        CHECK(course[4].waypoint.x == 4);
        CHECK(course[4].waypoint.y == -10);

        CHECK(distance(course.back().ferry) == 286);
    }
}
