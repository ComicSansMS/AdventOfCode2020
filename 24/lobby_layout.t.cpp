#include <lobby_layout.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Lobby Layout")
{
    char const sample_input[] = "sesenwnenenewseeswwswswwnenewsewsw\n"
                                "neeenesenwnwwswnenewnwwsewnenwseswesw\n"
                                "seswneswswsenwwnwse\n"
                                "nwnwneseeswswnenewneswwnewseswneseene\n"
                                "swweswneswnenwsewnwneneseenw\n"
                                "eesenwseswswnenwswnwnwsewwnwsene\n"
                                "sewnenenenesenwsewnenwwwse\n"
                                "wenwwweseeeweswwwnwwe\n"
                                "wsweesenenewnwwnwsenewsenwwsesesenwne\n"
                                "neeswseenwwswnwswswnw\n"
                                "nenwswwsewswnenenewsenwsenwnesesenew\n"
                                "enewnwewneswsewnwswenweswnenwsenwsw\n"
                                "sweneswneswneneenwnewenewwneswswnese\n"
                                "swwesenesewenwneswnwwneseswwne\n"
                                "enesenwswwswneneswsenwnewswseenwsese\n"
                                "wnwnesenesenenwwnenwsewesewsesesew\n"
                                "nenewswnwewswnenesenwnesewesw\n"
                                "eneswnwswnwsenenwnwnwwseeswneewsenese\n"
                                "neswnwewnwnwseenwseesewsenwsweewe\n"
                                "wseweeenwnesenwwwswnew\n";
    SECTION("Parse Input")
    {
        auto const paths = parseInput(sample_input);
        REQUIRE(paths.size() == 20);
        // sesenwnenenewseeswwswswwnenewsewsw
        CHECK(paths[0] == Path{ Direction::SouthEast, Direction::SouthEast, Direction::NorthWest, Direction::NorthEast,
                                Direction::NorthEast, Direction::NorthEast, Direction::West, Direction::SouthEast,
                                Direction::East, Direction::SouthWest, Direction::West, Direction::SouthWest,
                                Direction::SouthWest, Direction::West, Direction::NorthEast, Direction::NorthEast,
                                Direction::West, Direction::SouthEast, Direction::West, Direction::SouthWest });

        // neeswseenwwswnwswswnw
        CHECK(paths[9] == Path{ Direction::NorthEast, Direction::East, Direction::SouthWest, Direction::SouthEast,
                                Direction::East, Direction::NorthWest, Direction::West, Direction::SouthWest,
                                Direction::NorthWest, Direction::SouthWest, Direction::SouthWest, Direction::NorthWest });
    }

    SECTION("Walk paths")
    {
        auto const paths = parseInput(sample_input);
        auto const coords = walkPaths(paths);

        REQUIRE(coords.size() == 20);
    }

    SECTION("Count flipped tiles")
    {
        auto const paths = parseInput(sample_input);

        CHECK(countFlippedTiles(paths) == 10);
    }

    SECTION("Simulate")
    {
        auto const paths = parseInput(sample_input);
        CHECK(simulate(paths) == 2208);
    }
}
