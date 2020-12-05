#include <binary_boarding.hpp>

#include <catch.hpp>

TEST_CASE("Binary Boarding")
{
    SECTION("Parse Input")
    {
        CHECK(parseInput("FFFBBBFLRL\nBFBFBFBRRR") == std::vector<std::string>{"FFFBBBFLRL", "BFBFBFBRRR"});
    }

    SECTION("Compute Seat")
    {
        auto const s1 = computeSeat("FBFBBFFRLR");
        CHECK(s1.row == 44);
        CHECK(s1.column == 5);

        auto const s2 = computeSeat("BFFFBBFRRR");
        CHECK(s2.row == 70);
        CHECK(s2.column == 7);

        auto const s3 = computeSeat("FFFBBBFRRR");
        CHECK(s3.row == 14);
        CHECK(s3.column == 7);

        auto const s4 = computeSeat("BBFFBBFRLL");
        CHECK(s4.row == 102);
        CHECK(s4.column == 4);
    }

    SECTION("Compute Seat Id")
    {
        CHECK(computeSeatId(Seat{ 44, 5 }) == 357);
        CHECK(computeSeatId(Seat{ 70, 7 }) == 567);
        CHECK(computeSeatId(Seat{ 14, 7 }) == 119);
        CHECK(computeSeatId(Seat{ 102, 4 }) == 820);
    }

    SECTION("Highest Seat Id")
    {
        CHECK(highestSeatId(std::vector<std::string>{"FBFBBFFRLR", "BFFFBBFRRR", "FFFBBBFRRR", "BBFFBBFRLL"}) == 820);
        
    }

    SECTION("Find Free Seat")
    {
        CHECK(findFreeSeat(std::vector<std::string>{"FBFBBFFLLL", "FBFBBFFLLR", "FBFBBFFLRL", "FBFBBFFRLL"}) == 355);
    }
}
