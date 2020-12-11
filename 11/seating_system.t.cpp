#include <seating_system.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Handheld Halting")
{
    char const sample_input[] = 
        R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL
)";

    Map const map = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(map.width == 10);
        CHECK(map.height == 10);
        REQUIRE(map.grid.size() == 100);
        CHECK(map.grid[0] == Cell::Empty);
        CHECK(map.grid[1] == Cell::Floor);
        CHECK(map.grid[2] == Cell::Empty);
        CHECK(map.grid[3] == Cell::Empty);
        CHECK(map.grid[4] == Cell::Floor);
        CHECK(map.grid[5] == Cell::Empty);
        CHECK(map.grid[6] == Cell::Empty);
        CHECK(map.grid[7] == Cell::Floor);
        CHECK(map.grid[8] == Cell::Empty);
        CHECK(map.grid[9] == Cell::Empty);
        CHECK(map.grid[10] == Cell::Empty);
    }

    SECTION("Get Cell")
    {
        CHECK(getCell(map, 0, 0) == Cell::Empty);
        CHECK(getCell(map, 1, 0) == Cell::Floor);
        CHECK(getCell(map, 0, 2) == Cell::Empty);
        CHECK(getCell(map, 1, 2) == Cell::Floor);
        CHECK(getCell(map, 0, 6) == Cell::Floor);
        CHECK(getCell(map, 1, 6) == Cell::Floor);
        CHECK(getCell(map, 0, 9) == Cell::Empty);
        CHECK(getCell(map, 1, 9) == Cell::Floor);
        CHECK(getCell(map, 9, 9) == Cell::Empty);

        CHECK(getCell(map, 0, 10) == Cell::Floor);
        CHECK(getCell(map, 10, 0) == Cell::Floor);
        CHECK(getCell(map, 10, 10) == Cell::Floor);
        CHECK(getCell(map, -1, 0) == Cell::Floor);
        CHECK(getCell(map, 0, -1) == Cell::Floor);
        CHECK(getCell(map, -1, -1) == Cell::Floor);
    }

    SECTION("Step")
    {
        Map it_map = map;
        CHECK(fmt::format("{}\n", it_map) == "L.LL.LL.LL\n"
                                             "LLLLLLL.LL\n"
                                             "L.L.L..L..\n"
                                             "LLLL.LL.LL\n"
                                             "L.LL.LL.LL\n"
                                             "L.LLLLL.LL\n"
                                             "..L.L.....\n"
                                             "LLLLLLLLLL\n"
                                             "L.LLLLLL.L\n"
                                             "L.LLLLL.LL\n");

        it_map = step(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.##.##.##\n"
                                             "#######.##\n"
                                             "#.#.#..#..\n"
                                             "####.##.##\n"
                                             "#.##.##.##\n"
                                             "#.#####.##\n"
                                             "..#.#.....\n"
                                             "##########\n"
                                             "#.######.#\n"
                                             "#.#####.##\n");

        it_map = step(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.LL.L#.##\n"
                                             "#LLLLLL.L#\n"
                                             "L.L.L..L..\n"
                                             "#LLL.LL.L#\n"
                                             "#.LL.LL.LL\n"
                                             "#.LLLL#.##\n"
                                             "..L.L.....\n"
                                             "#LLLLLLLL#\n"
                                             "#.LLLLLL.L\n"
                                             "#.#LLLL.##\n");

        it_map = step(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.##.L#.##\n"
                                             "#L###LL.L#\n"
                                             "L.#.#..#..\n"
                                             "#L##.##.L#\n"
                                             "#.##.LL.LL\n"
                                             "#.###L#.##\n"
                                             "..#.#.....\n"
                                             "#L######L#\n"
                                             "#.LL###L.L\n"
                                             "#.#L###.##\n");

        it_map = step(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.#L.L#.##\n"
                                             "#LLL#LL.L#\n"
                                             "L.L.L..#..\n"
                                             "#LLL.##.L#\n"
                                             "#.LL.LL.LL\n"
                                             "#.LL#L#.##\n"
                                             "..L.L.....\n"
                                             "#L#LLLL#L#\n"
                                             "#.LLLLLL.L\n"
                                             "#.#L#L#.##\n");

        it_map = step(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.#L.L#.##\n"
                                             "#LLL#LL.L#\n"
                                             "L.#.L..#..\n"
                                             "#L##.##.L#\n"
                                             "#.#L.LL.LL\n"
                                             "#.#L#L#.##\n"
                                             "..L.L.....\n"
                                             "#L#L##L#L#\n"
                                             "#.LLLLLL.L\n"
                                             "#.#L#L#.##\n");


        it_map = step(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.#L.L#.##\n"
                                             "#LLL#LL.L#\n"
                                             "L.#.L..#..\n"
                                             "#L##.##.L#\n"
                                             "#.#L.LL.LL\n"
                                             "#.#L#L#.##\n"
                                             "..L.L.....\n"
                                             "#L#L##L#L#\n"
                                             "#.LLLLLL.L\n"
                                             "#.#L#L#.##\n");
    }

    SECTION("Count Occupied Seats")
    {
        CHECK(countOccupiedSeats(map) == 0);
        Map it_map = map;
        for (int i = 0; i < 5; ++i) { it_map = step(it_map); }
        CHECK(countOccupiedSeats(it_map) == 37);
    }

    SECTION("Simulate")
    {
        CHECK(simulate(map) == 37);
    }

    SECTION("getFirstCellInDirection")
    {
        Map const sample = parseInput(".......#.\n"
                                      "...#.....\n"
                                      ".#.......\n"
                                      ".........\n"
                                      "..#L....#\n"
                                      "....#....\n"
                                      ".........\n"
                                      "#........\n"
                                      "...#.....\n");
        CHECK(getCell(sample, 3, 4) == Cell::Empty);
        CHECK(getFirstCellInDirection(sample, 3, 4, -1, -1) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4,  0, -1) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4,  1, -1) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4, -1,  0) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4,  1,  0) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4, -1,  1) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4,  0,  1) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample, 3, 4,  1,  1) == Cell::Occupied);

        Map const sample2 = parseInput(".............\n"
                                       ".L.L.#.#.#.#.\n"
                                       ".............\n");
        CHECK(getCell(sample2, 1, 1) == Cell::Empty);
        CHECK(getCell(sample2, 3, 1) == Cell::Empty);

        CHECK(getFirstCellInDirection(sample2, 1, 1, -1, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 1, 1,  0, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 1, 1,  1, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 1, 1, -1,  0) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 1, 1,  1,  0) == Cell::Empty);
        CHECK(getFirstCellInDirection(sample2, 1, 1, -1,  1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 1, 1,  0,  1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 1, 1,  1,  1) == Cell::Floor);

        CHECK(getFirstCellInDirection(sample2, 3, 1, -1, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 3, 1,  0, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 3, 1,  1, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 3, 1, -1,  0) == Cell::Empty);
        CHECK(getFirstCellInDirection(sample2, 3, 1,  1,  0) == Cell::Occupied);
        CHECK(getFirstCellInDirection(sample2, 3, 1, -1,  1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 3, 1,  0,  1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample2, 3, 1,  1,  1) == Cell::Floor);

        Map const sample3 = parseInput(".##.##.\n"
                                       "#.#.#.#\n"
                                       "##...##\n"
                                       "...L...\n"
                                       "##...##\n"
                                       "#.#.#.#\n"
                                       ".##.##.\n");
        CHECK(getCell(sample3, 3, 3) == Cell::Empty);
        CHECK(getFirstCellInDirection(sample3, 3, 3, -1, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3,  0, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3,  1, -1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3, -1,  0) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3,  1,  0) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3, -1,  1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3,  0,  1) == Cell::Floor);
        CHECK(getFirstCellInDirection(sample3, 3, 3,  1,  1) == Cell::Floor);
    }

    SECTION("Step2")
    {
        Map it_map = map;
        CHECK(fmt::format("{}\n", it_map) == "L.LL.LL.LL\n"
                                             "LLLLLLL.LL\n"
                                             "L.L.L..L..\n"
                                             "LLLL.LL.LL\n"
                                             "L.LL.LL.LL\n"
                                             "L.LLLLL.LL\n"
                                             "..L.L.....\n"
                                             "LLLLLLLLLL\n"
                                             "L.LLLLLL.L\n"
                                             "L.LLLLL.LL\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.##.##.##\n"
                                             "#######.##\n"
                                             "#.#.#..#..\n"
                                             "####.##.##\n"
                                             "#.##.##.##\n"
                                             "#.#####.##\n"
                                             "..#.#.....\n"
                                             "##########\n"
                                             "#.######.#\n"
                                             "#.#####.##\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.LL.LL.L#\n"
                                             "#LLLLLL.LL\n"
                                             "L.L.L..L..\n"
                                             "LLLL.LL.LL\n"
                                             "L.LL.LL.LL\n"
                                             "L.LLLLL.LL\n"
                                             "..L.L.....\n"
                                             "LLLLLLLLL#\n"
                                             "#.LLLLLL.L\n"
                                             "#.LLLLL.L#\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.L#.##.L#\n"
                                             "#L#####.LL\n"
                                             "L.#.#..#..\n"
                                             "##L#.##.##\n"
                                             "#.##.#L.##\n"
                                             "#.#####.#L\n"
                                             "..#.#.....\n"
                                             "LLL####LL#\n"
                                             "#.L#####.L\n"
                                             "#.L####.L#\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.L#.L#.L#\n"
                                             "#LLLLLL.LL\n"
                                             "L.L.L..#..\n"
                                             "##LL.LL.L#\n"
                                             "L.LL.LL.L#\n"
                                             "#.LLLLL.LL\n"
                                             "..L.L.....\n"
                                             "LLLLLLLLL#\n"
                                             "#.LLLLL#.L\n"
                                             "#.L#LL#.L#\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.L#.L#.L#\n"
                                             "#LLLLLL.LL\n"
                                             "L.L.L..#..\n"
                                             "##L#.#L.L#\n"
                                             "L.L#.#L.L#\n"
                                             "#.L####.LL\n"
                                             "..#.#.....\n"
                                             "LLL###LLL#\n"
                                             "#.LLLLL#.L\n"
                                             "#.L#LL#.L#\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.L#.L#.L#\n"
                                             "#LLLLLL.LL\n"
                                             "L.L.L..#..\n"
                                             "##L#.#L.L#\n"
                                             "L.L#.LL.L#\n"
                                             "#.LLLL#.LL\n"
                                             "..#.L.....\n"
                                             "LLL###LLL#\n"
                                             "#.LLLLL#.L\n"
                                             "#.L#LL#.L#\n");

        it_map = step2(it_map);
        CHECK(fmt::format("{}\n", it_map) == "#.L#.L#.L#\n"
                                             "#LLLLLL.LL\n"
                                             "L.L.L..#..\n"
                                             "##L#.#L.L#\n"
                                             "L.L#.LL.L#\n"
                                             "#.LLLL#.LL\n"
                                             "..#.L.....\n"
                                             "LLL###LLL#\n"
                                             "#.LLLLL#.L\n"
                                             "#.L#LL#.L#\n");
    }

    SECTION("Simulate2")
    {
        CHECK(simulate2(map) == 26);
    }
}
