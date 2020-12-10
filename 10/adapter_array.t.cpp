#include <adapter_array.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>

#include <catch.hpp>

TEST_CASE("Handheld Halting")
{
    char const sample_input[] = 
        R"(16
10
15
5
1
11
7
19
6
12
4)";

    SECTION("Parse Input")
    {
        auto example = parseInput(sample_input);
        CHECK(example == std::vector<int>{ 16, 10, 15, 5, 1, 11, 7, 19, 6, 12, 4 });
    }

    char const sample_input2[] =
        R"(28
33
18
42
31
14
46
20
48
47
24
23
49
45
19
38
39
11
1
32
25
35
8
17
7
9
4
2
34
10
3)";

    SECTION("Chain Adapters")
    {
        AdapterChain const chain = chainAdapters(parseInput(sample_input));
        CHECK(chain.builtin_joltage == 22);
        CHECK(chain.jolt_diff_1 == 7);
        CHECK(chain.jolt_diff_3 == 5);

        AdapterChain const chain2 = chainAdapters(parseInput(sample_input2));
        CHECK(chain2.builtin_joltage == 52);
        CHECK(chain2.jolt_diff_1 == 22);
        CHECK(chain2.jolt_diff_3 == 10);
    }

    SECTION("Count Valid Chains")
    {
        CHECK(countValidChains(std::vector<int>{1}) == 1);
        CHECK(countValidChains(std::vector<int>{1, 2}) == 2);
        CHECK(countValidChains(std::vector<int>{1, 2, 3}) == 4);
        CHECK(countValidChains(std::vector<int>{1, 2, 3, 4}) == 7);
        CHECK(countValidChains(std::vector<int>{1, 2, 3, 4, 5}) == 13);
        CHECK(countValidChains(parseInput(sample_input)) == 8);
        CHECK(countValidChains(parseInput(sample_input2)) == 19208);
    }
}
