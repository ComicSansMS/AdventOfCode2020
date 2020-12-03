#include <toboggan_trajectory.hpp>

#include <catch.hpp>

TEST_CASE("Toboggan Trajectory")
{
    char const sample_map[] =
R"(..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#
)";

    auto const map = parseInput(sample_map);

    SECTION("Parse Input")
    {
        REQUIRE(map.m.size() == 12);
        CHECK(map.map_width == 11);
        CHECK(map.m[0].size() == 2);
        CHECK(map.m[1].size() == 3);
        CHECK(map.m[2].size() == 3);
        CHECK(map.m[3].size() == 4);
        CHECK(map.m[11].size() == 0);

        CHECK(map.m[0].find(2) != std::end(map.m[0]));
        CHECK(map.m[0].find(3) != std::end(map.m[0]));

        CHECK(map.m[1].find(0) != std::end(map.m[1]));
        CHECK(map.m[1].find(4) != std::end(map.m[1]));
        CHECK(map.m[1].find(8) != std::end(map.m[1]));
    }

    SECTION("Has Tree At")
    {
        CHECK(!hasTreeAt(map, 0, 0));
        CHECK(hasTreeAt(map, 2, 0));
        CHECK(hasTreeAt(map, 3, 0));
        CHECK(!hasTreeAt(map, 0, 4));
        CHECK(!hasTreeAt(map, 0, 255));
        CHECK(!hasTreeAt(map, 0, -1));

        CHECK(!hasTreeAt(map, 1, 1));
        CHECK(!hasTreeAt(map, 2, 1));
        CHECK(!hasTreeAt(map, 3, 1));
        CHECK(hasTreeAt(map, 4, 1));
        CHECK(!hasTreeAt(map, 5, 1));
        CHECK(!hasTreeAt(map, 6, 1));
        CHECK(!hasTreeAt(map, 7, 1));
        CHECK(hasTreeAt(map, 8, 1));
        CHECK(!hasTreeAt(map, 9, 1));
        CHECK(!hasTreeAt(map, 10, 1));
        CHECK(hasTreeAt(map, 11, 1));
        CHECK(!hasTreeAt(map, 12, 1));
        CHECK(!hasTreeAt(map, 13, 1));
        CHECK(!hasTreeAt(map, 14, 1));
        CHECK(hasTreeAt(map, 15, 1));
        CHECK(!hasTreeAt(map, 16, 1));
    }

    SECTION("Slope Traversal")
    {
        auto const track = slopeTraversal(map, 3, 1);
        REQUIRE(track.size() == 7);
        CHECK(track[0].x == 6);
        CHECK(track[0].y == 2);

        CHECK(track[1].x == 12);
        CHECK(track[1].y == 4);

        CHECK(track[2].x == 15);
        CHECK(track[2].y == 5);

        CHECK(track[3].x == 21);
        CHECK(track[3].y == 7);

        CHECK(track[4].x == 24);
        CHECK(track[4].y == 8);

        CHECK(track[5].x == 27);
        CHECK(track[5].y == 9);

        CHECK(track[6].x == 30);
        CHECK(track[6].y == 10);

        CHECK(slopeTraversal(map, 1, 1).size() == 2);
        CHECK(slopeTraversal(map, 5, 1).size() == 3);
        CHECK(slopeTraversal(map, 7, 1).size() == 4);
        CHECK(slopeTraversal(map, 1, 2).size() == 2);
    }

    SECTION("Count all traversals")
    {
        CHECK(countAllTraversals(map) == 336);
    }
}
