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

    SECTION("Tile Transform")
    {
        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Straight)) ==
            "..##.#..#.\n"
            "##..#.....\n"
            "#...##..#.\n"
            "####.#...#\n"
            "##.##.###.\n"
            "##...#.###\n"
            ".#.#.#..##\n"
            "..#....#..\n"
            "###...#.#.\n"
            "..###..###\n");


        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Rot90)) ==
            ".#..#####.\n"
            ".#.####.#.\n"
            "###...#..#\n"
            "#..#.##..#\n"
            "#....#.##.\n"
            "...##.##.#\n"
            ".#...#....\n"
            "#.#.##....\n"
            "##.###.#.#\n"
            "#..##.#...\n");

        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Rot180)) ==
            "###..###..\n"
            ".#.#...###\n"
            "..#....#..\n"
            "##..#.#.#.\n"
            "###.#...##\n"
            ".###.##.##\n"
            "#...#.####\n"
            ".#..##...#\n"
            ".....#..##\n"
            ".#..#.##..\n");

        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Rot270)) ==
            "...#.##..#\n"
            "#.#.###.##\n"
            "....##.#.#\n"
            "....#...#.\n"
            "#.##.##...\n"
            ".##.#....#\n"
            "#..##.#..#\n"
            "#..#...###\n"
            ".#.####.#.\n"
            ".#####..#.\n");

        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Flip)) ==
            "..###..###\n"
            "###...#.#.\n"
            "..#....#..\n"
            ".#.#.#..##\n"
            "##...#.###\n"
            "##.##.###.\n"
            "####.#...#\n"
            "#...##..#.\n"
            "##..#.....\n"
            "..##.#..#.\n");

        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Flip90)) ==
            ".#####..#.\n"
            ".#.####.#.\n"
            "#..#...###\n"
            "#..##.#..#\n"
            ".##.#....#\n"
            "#.##.##...\n"
            "....#...#.\n"
            "....##.#.#\n"
            "#.#.###.##\n"
            "...#.##..#\n");

        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Flip180)) ==
            ".#..#.##..\n"
            ".....#..##\n"
            ".#..##...#\n"
            "#...#.####\n"
            ".###.##.##\n"
            "###.#...##\n"
            "##..#.#.#.\n"
            "..#....#..\n"
            ".#.#...###\n"
            "###..###..\n");

        CHECK(fmt::format("{}", transformTo(tiles[0], TransformState::Flip270)) ==
            "#..##.#...\n"
            "##.###.#.#\n"
            "#.#.##....\n"
            ".#...#....\n"
            "...##.##.#\n"
            "#....#.##.\n"
            "#..#.##..#\n"
            "###...#..#\n"
            ".#.####.#.\n"
            ".#..#####.\n");
    }

    SECTION("Compressed Transform")
    {
        auto ct = compressTile(tiles[0]);
        auto const equal_sides = [](CompressedTile const& t1, CompressedTile const& t2) {
            return (t1.left == t2.left) && (t1.bottom == t2.bottom) && (t1.right == t2.right) && (t1.top == t2.top);
        };
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Rot90);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Rot90))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Rot180);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Rot180))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Rot270);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Rot270))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Flip);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Flip))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Flip90);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Flip90))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Flip180);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Flip180))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Flip270);
        CHECK(equal_sides(ct, compressTile(transformTo(tiles[0], TransformState::Flip270))));
        ct = transformCompressed(ct);
        CHECK(ct.transform == TransformState::Straight);
        CHECK(equal_sides(ct, compressTile(tiles[0])));
    }

    SECTION("Solve Puzzle")
    {
        auto const sorted_tiles = findCorners(tiles);
        solvePuzzle(sorted_tiles);
    }
}
