#include <docking_data.hpp>

#include <fmt/format.h>

#include <catch.hpp>

TEST_CASE("Docking Data")
{
    char const sample_input[] = R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
        mem[8] = 11
        mem[7] = 101
        mem[8] = 0
)";

    SECTION("Parse Input")
    {
        auto const instructions = parseInput(sample_input);
        REQUIRE(instructions.size() == 4);
        REQUIRE(instructions[0].index() == 0);
        CHECK(std::get<Mask>(instructions[0]).mask_0.to_ullong() == 0b000000000000000000000000000000000010);
        CHECK(std::get<Mask>(instructions[0]).mask_0 == std::bitset<36>(2));
        CHECK(std::get<Mask>(instructions[0]).mask_1.to_ullong() == 0b000000000000000000000000000001000000);
        CHECK(std::get<Mask>(instructions[0]).mask_1 == std::bitset<36>(64));
        CHECK(std::get<Mask>(instructions[0]).mask_x.to_ullong() == 0b111111111111111111111111111110111101);


        REQUIRE(instructions[1].index() == 1);
        CHECK(std::get<Mov>(instructions[1]).address == 8);
        CHECK(std::get<Mov>(instructions[1]).value == 11);

        REQUIRE(instructions[2].index() == 1);
        CHECK(std::get<Mov>(instructions[2]).address == 7);
        CHECK(std::get<Mov>(instructions[2]).value == 101);

        REQUIRE(instructions[3].index() == 1);
        CHECK(std::get<Mov>(instructions[3]).address == 8);
        CHECK(std::get<Mov>(instructions[3]).value == 0);
    }

    SECTION("Run Program")
    {
        auto const result = runProgram(parseInput(sample_input));
        CHECK(result.memory.size() == 2);
        REQUIRE(result.memory.find(7) != end(result.memory));
        CHECK(result.memory.find(7)->second.to_ullong() == 101);

        REQUIRE(result.memory.find(8) != end(result.memory));
        CHECK(result.memory.find(8)->second.to_ullong() == 64);

        CHECK(result.sum_of_values == 165);
    }

    SECTION("Run Program 2")
    {
        auto const res1 = runProgram2(parseInput("mask = 000000000000000000000000000000X1001X\nmem[42] = 100\n"));
        CHECK(res1.memory.size() == 4);
        REQUIRE(res1.memory.find(26) != end(res1.memory));
        CHECK(res1.memory.find(26)->second == 100);
        REQUIRE(res1.memory.find(27) != end(res1.memory));
        CHECK(res1.memory.find(27)->second == 100);
        REQUIRE(res1.memory.find(58) != end(res1.memory));
        CHECK(res1.memory.find(58)->second == 100);
        REQUIRE(res1.memory.find(59) != end(res1.memory));
        CHECK(res1.memory.find(59)->second == 100);
        CHECK(res1.sum_of_values == 400);

        auto const res2 = runProgram2(parseInput("mask = 000000000000000000000000000000X1001X\nmem[42] = 100\n"
                                                 "mask = 00000000000000000000000000000000X0XX\nmem[26] = 1\n"));
        CHECK(res2.memory.size() == 10);
        REQUIRE(res2.memory.find(16) != end(res2.memory));
        CHECK(res2.memory.find(16)->second == 1);
        REQUIRE(res2.memory.find(17) != end(res2.memory));
        CHECK(res2.memory.find(17)->second == 1);
        REQUIRE(res2.memory.find(18) != end(res2.memory));
        CHECK(res2.memory.find(18)->second == 1);
        REQUIRE(res2.memory.find(19) != end(res2.memory));
        CHECK(res2.memory.find(19)->second == 1);
        REQUIRE(res2.memory.find(24) != end(res2.memory));
        CHECK(res2.memory.find(24)->second == 1);
        REQUIRE(res2.memory.find(25) != end(res2.memory));
        CHECK(res2.memory.find(25)->second == 1);
        REQUIRE(res2.memory.find(26) != end(res2.memory));
        CHECK(res2.memory.find(26)->second == 1);
        REQUIRE(res2.memory.find(27) != end(res2.memory));
        CHECK(res2.memory.find(27)->second == 1);
        REQUIRE(res2.memory.find(58) != end(res2.memory));
        CHECK(res2.memory.find(58)->second == 100);
        REQUIRE(res2.memory.find(59) != end(res2.memory));
        CHECK(res2.memory.find(59)->second == 100);
        CHECK(res2.sum_of_values == 208);
    }
}
