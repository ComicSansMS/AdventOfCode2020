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
}
