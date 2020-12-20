#include <jurassic_jigsaw.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Jurassic Jigsaw")
{
    char const sample_input[] = R"(Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...

)";

    auto const tiles = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(tiles.size() == 9);
        CHECK(tiles[0].id == 2311);
        CHECK(tiles[1].id == 1951);
        CHECK(tiles[2].id == 1171);
        CHECK(tiles[3].id == 1427);
        CHECK(tiles[4].id == 1489);
        CHECK(tiles[5].id == 2473);
        CHECK(tiles[6].id == 2971);
        CHECK(tiles[7].id == 2729);
        CHECK(tiles[8].id == 3079);

        CHECK(fmt::format("{}", tiles[0]) == "..##.#..#.\n"
                                             "##..#.....\n"
                                             "#...##..#.\n"
                                             "####.#...#\n"
                                             "##.##.###.\n"
                                             "##...#.###\n"
                                             ".#.#.#..##\n"
                                             "..#....#..\n"
                                             "###...#.#.\n"
                                             "..###..###\n");

        CHECK(fmt::format("{}", tiles[1]) == "#.##...##.\n"
                                             "#.####...#\n"
                                             ".....#..##\n"
                                             "#...######\n"
                                             ".##.#....#\n"
                                             ".###.#####\n"
                                             "###.##.##.\n"
                                             ".###....#.\n"
                                             "..#.#..#.#\n"
                                             "#...##.#..\n");

        CHECK(fmt::format("{}", tiles[8]) == "#.#.#####.\n"
                                             ".#..######\n"
                                             "..#.......\n"
                                             "######....\n"
                                             "####.#..#.\n"
                                             ".#...#.##.\n"
                                             "#.#####.##\n"
                                             "..#.###...\n"
                                             "..#.......\n"
                                             "..#.###...\n");
    }

    SECTION("Compress Tile")
    {
        auto const compressed8 = compressTile(tiles[8]);
        CHECK(compressed8.top.to_ulong() == 0b1010111110);
        CHECK(compressed8.bottom.to_ulong() == 0b0010111000);
        CHECK(compressed8.left.to_ulong() == 0b1001101000);
        CHECK(compressed8.right.to_ulong() == 0b0100001000);
    }

    SECTION("Find Corner Tiles")
    {
        auto const corners = findCorners(tiles).corner;
        CHECK(corners.size() == 4);
        CHECK(corners[0].id == 1951);
        CHECK(corners[1].id == 1171);
        CHECK(corners[2].id == 2971);
        CHECK(corners[3].id == 3079);
    }

    SECTION("Solve 1")
    {
        CHECK(solve1(tiles) == 20899048083289);
    }
}
