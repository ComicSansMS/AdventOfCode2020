#include <handheld_halting.hpp>

#include <catch.hpp>

TEST_CASE("Handheld Halting")
{
    char const sample_input[] = 
        R"(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)";

    auto gameboy = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(gameboy.acc == 0);
        CHECK(gameboy.pc == 0);

        REQUIRE(gameboy.ops.size() == 9);
        CHECK(std::get<Nop>(gameboy.ops[0]).arg == 0);
        CHECK(std::get<Acc>(gameboy.ops[1]).arg == 1);
        CHECK(std::get<Jmp>(gameboy.ops[2]).arg == 4);
        CHECK(std::get<Acc>(gameboy.ops[3]).arg == 3);
        CHECK(std::get<Jmp>(gameboy.ops[4]).arg == -3);
        CHECK(std::get<Acc>(gameboy.ops[5]).arg == -99);
        CHECK(std::get<Acc>(gameboy.ops[6]).arg == 1);
        CHECK(std::get<Jmp>(gameboy.ops[7]).arg == -4);
        CHECK(std::get<Acc>(gameboy.ops[8]).arg == 6);
    }

    SECTION("Nop")
    {
        Gameboy gb;
        gb.acc = 0;
        gb.pc = 0;
        Nop nop;
        nop.arg = 42;
        nop.execute(gb);
        CHECK(gb.acc == 0);
        CHECK(gb.pc == 1);
        nop.arg = -99;
        nop.execute(gb);
        CHECK(gb.acc == 0);
        CHECK(gb.pc == 2);
    }

    SECTION("Acc")
    {
        Gameboy gb;
        gb.acc = 0;
        gb.pc = 0;
        Acc acc;
        acc.arg = 42;
        acc.execute(gb);
        CHECK(gb.acc == 42);
        CHECK(gb.pc == 1);
        acc.arg = -19;
        acc.execute(gb);
        CHECK(gb.acc == 23);
        CHECK(gb.pc == 2);
        acc.execute(gb);
        CHECK(gb.acc == 4);
        CHECK(gb.pc == 3);
    }

    SECTION("Jmp")
    {
        Gameboy gb;
        gb.acc = 0;
        gb.pc = 0;
        Jmp jmp;
        jmp.arg = 42;
        jmp.execute(gb);
        CHECK(gb.acc == 0);
        CHECK(gb.pc == 42);
        jmp.arg = -19;
        jmp.execute(gb);
        CHECK(gb.acc == 0);
        CHECK(gb.pc == 23);
        jmp.execute(gb);
        CHECK(gb.acc == 0);
        CHECK(gb.pc == 4);
    }

    SECTION("Stepwise Execution")
    {
        CHECK(gameboy.acc == 0);
        CHECK(gameboy.pc == 0);
        
        CHECK(step(gameboy) == 1);
        CHECK(gameboy.acc == 0);
        CHECK(gameboy.pc == 1);

        CHECK(step(gameboy) == 2);
        CHECK(gameboy.acc == 1);
        CHECK(gameboy.pc == 2);

        CHECK(step(gameboy) == 6);
        CHECK(gameboy.acc == 1);
        CHECK(gameboy.pc == 6);

        CHECK(step(gameboy) == 7);
        CHECK(gameboy.acc == 2);
        CHECK(gameboy.pc == 7);

        CHECK(step(gameboy) == 3);
        CHECK(gameboy.acc == 2);
        CHECK(gameboy.pc == 3);

        CHECK(step(gameboy) == 4);
        CHECK(gameboy.acc == 5);
        CHECK(gameboy.pc == 4);

        CHECK(step(gameboy) == 1);
        CHECK(gameboy.acc == 5);
        CHECK(gameboy.pc == 1);
    }

    SECTION("Run Until Loop")
    {
        CHECK(runUntilLoop(gameboy) == 5);
    }

    SECTION("Fix and Run")
    {
        CHECK(fixAndRun(gameboy) == 8);
    }
}
